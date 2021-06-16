/****************************************************************************************************************************
  ISR_Timer_Complex.ino
  For ESP8266 boards
  Written by Khoi Hoang
  
  The ESP8266 timers are badly designed, using only 23-bit counter along with maximum 256 prescaler. They're only better than UNO / Mega.
  The ESP8266 has two hardware timers, but timer0 has been used for WiFi and it's not advisable to use. Only timer1 is available.
  The timer1's 23-bit counter terribly can count only up to 8,388,607. So the timer1 maximum interval is very short.
  Using 256 prescaler, maximum timer1 interval is only 26.843542 seconds !!!
  
  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one Hardware timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
  
  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy
  
  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy
  
  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt_Generic
  Licensed under MIT license
*****************************************************************************************************************************/
/* Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.

   
   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written
*/

#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG true

#include <ESP8266WiFi.h>

#define USE_SSL     false

#if USE_SSL
  #include <BlynkSimpleEsp8266_SSL.h>
  #define BLYNK_HARDWARE_PORT     9443
#else
  #include <BlynkSimpleEsp8266.h>
  #define BLYNK_HARDWARE_PORT     8080
#endif

#define USE_LOCAL_SERVER    true

// If local server
#if USE_LOCAL_SERVER
  char blynk_server[]   = "account.duckdns.org";
  //char blynk_server[]   = "192.168.2.110";
#else
  char blynk_server[]   = "";
#endif

char auth[]     = "****";
char ssid[]     = "****";
char pass[]     = "****";

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "TimerInterrupt_Generic.h"
#include "ISR_Timer_Generic.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2         // Pin D4 mapped to pin GPIO2/TXD1 of ESP8266, NodeMCU and WeMoS, control on-board LED
#endif

#define HW_TIMER_INTERVAL_MS        50

#define WIFI_TIMEOUT      20000L

volatile uint32_t lastMillis = 0;

// Init ESP32 timer 0
ESP8266Timer ITimer;

// Init BlynkTimer
ISR_Timer ESP8266_ISR_Timer;

BlynkTimer blynkTimer;

void IRAM_ATTR TimerHandler()
{
  static bool toggle = false;
  static bool started = false;

  static int timeRun      = 0;

  ESP8266_ISR_Timer.run();

  // Toggle LED every 50 x 100 = 5000ms = 5s
  if (++timeRun == 100)
  { 
    timeRun = 0;

    if (!started)
    {
      started = true;
      pinMode(LED_BUILTIN, OUTPUT);
    }
  
#if (TIMER_INTERRUPT_DEBUG > 0)
    Serial.print("Delta ms = "); Serial.println(millis() - lastMillis);
    lastMillis = millis();
#endif
    
    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

void IRAM_ATTR doingSomething2s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)  
  static unsigned long previousMillis = lastMillis;

  Serial.print("doingSomething2s: Delta ms = "); Serial.println(millis() - previousMillis);

  previousMillis = millis();
#endif
}

void IRAM_ATTR doingSomething5s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)  
  static unsigned long previousMillis = lastMillis;

  Serial.print("doingSomething5s: Delta ms = "); Serial.println(millis() - previousMillis);

  previousMillis = millis();
#endif
}

void IRAM_ATTR doingSomething10s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)  
  static unsigned long previousMillis = lastMillis;

  Serial.print("doingSomething10s: Delta ms = "); Serial.println(millis() - previousMillis);

  previousMillis = millis();
#endif
}

void IRAM_ATTR doingSomething50s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)  
  static unsigned long previousMillis = lastMillis;

  Serial.print("doingSomething50s: Delta ms = "); Serial.println(millis() - previousMillis);

  previousMillis = millis();
#endif
}

#define BLYNK_TIMER_MS        2000L
  
void blynkDoingSomething2s()
{
  static unsigned long previousMillis = lastMillis;
  
  Serial.print(F("blynkDoingSomething2s: Delta programmed ms = ")); Serial.print(BLYNK_TIMER_MS);
  Serial.print(F(", actual = ")); Serial.println(millis() - previousMillis);
  
  previousMillis = millis();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
  delay(200);

  Serial.print(F("\nStarting ISR_Timer_Complex on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP8266_TIMER_INTERRUPT_VERSION);
  Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));
  
  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.print(F("Starting  ITimer OK, millis() = ")); Serial.println(lastMillis);
  }
  else
    Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  ESP8266_ISR_Timer.setInterval(2000L, doingSomething2s);  
  ESP8266_ISR_Timer.setInterval(5000L, doingSomething5s);  
  ESP8266_ISR_Timer.setInterval(10000L, doingSomething10s);  
  ESP8266_ISR_Timer.setInterval(50000L, doingSomething50s);

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  blynkTimer.setInterval(BLYNK_TIMER_MS, blynkDoingSomething2s);  

  unsigned long startWiFi = millis();
  
  WiFi.begin(ssid, pass);
  
  do
  {
    delay(200);
    
    if ( (WiFi.status() == WL_CONNECTED) || (millis() > startWiFi + WIFI_TIMEOUT) )
      break;
  } while (WiFi.status() != WL_CONNECTED);
  
  Blynk.config(auth, blynk_server, BLYNK_HARDWARE_PORT);
  Blynk.connect();

  if (Blynk.connected())
    Serial.println(F("Blynk connected"));
  else
    Serial.println(F("Blynk not connected yet"));
}

#define BLOCKING_TIME_MS      3000L

void loop()
{ 
  Blynk.run();

  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ESP8266_ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ESP8266_ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);
  
  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ESP8266_ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  blynkTimer.run();
}
