/****************************************************************************************************************************
  ISR_Timer_Complex.ino
  For Arduino megaAVR ATMEGA4809-based boards (UNO WiFi Rev2, NANO_EVERY, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt_Generic
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
*****************************************************************************************************************************/

/****************************************************************************************************************************
   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written
*****************************************************************************************************************************/

// To be used with UNO WiFi Rev2 and Blynk v0.6.1 only
// Compiler error with Blynk v1.0.0

#if !defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #error To be used with UNO WiFi Rev2 only.
#else
  #warning To be used with Blynk v0.6.1 only. Compiler error with Blynk v1.0.0
#endif

#if ( defined(__AVR_ATmega328__) || defined(__AVR_ATmega168__) )
  #error This is designed only for Arduino or Adafruit AVR board with more memory! Please check your Tools->Board setting.
#endif

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG true

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_16MHZ     true
#define USING_8MHZ      false
#define USING_250KHZ    false

#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false

#include "TimerInterrupt_Generic.h"
#include "ISR_Timer_Generic.h"

#include <BlynkSimpleWiFiNINA_UNO_WiFi.h>     // https://github.com/khoih-prog/Blynk_WiFiNINA_WM

#define BLYNK_HARDWARE_PORT     8080

#define USE_LOCAL_SERVER        true

// If local server
#if USE_LOCAL_SERVER
char blynk_server[]   = "yourname.duckdns.org";
//char blynk_server[]   = "192.168.2.110";
#else
char blynk_server[]   = "";
#endif

char auth[]     = "****";
char ssid[]     = "****";
char pass[]     = "****";

#define OUTPUT_PIN       2         // Connect LED to this output pin

#define HW_TIMER_INTERVAL_MS        10

#define WIFI_TIMEOUT      20000L

volatile uint32_t lastMillis = 0;

// Init BlynkTimer
ISR_Timer ISR_Timer1;

// Init BlynkTimer
BlynkTimer blynkTimer;

#define LED_TOGGLE_INTERVAL_MS      2000L

void TimerHandler(void)
{
  static bool toggle = false;
  static bool started = false;

  static int timeRun      = 0;

  ISR_Timer1.run();

  // Toggle LED every 10 x 200 = 2000ms = 2s
  if (++timeRun == (LED_TOGGLE_INTERVAL_MS / HW_TIMER_INTERVAL_MS) )
  {
    timeRun = 0;

    if (!started)
    {
      started = true;
      pinMode(OUTPUT_PIN, OUTPUT);
    }

#if (TIMER_INTERRUPT_DEBUG > 0)
    Serial.print("Delta ms = "); Serial.println(millis() - lastMillis);
    lastMillis = millis();
#endif

    //timer interrupt toggles pin OUTPUT_PIN
    digitalWrite(OUTPUT_PIN, toggle);
    toggle = !toggle;
  }
}

void doingSomething2s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)  
  static unsigned long previousMillis = lastMillis;

  Serial.print("doingSomething2s: Delta ms = "); Serial.println(millis() - previousMillis);

  previousMillis = millis();
#endif  
}

void doingSomething5s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)  
  static unsigned long previousMillis = lastMillis;

  Serial.print("doingSomething5s: Delta ms = "); Serial.println(millis() - previousMillis);

  previousMillis = millis();
#endif
}

void doingSomething10s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)  
  static unsigned long previousMillis = lastMillis;

  Serial.print("doingSomething10s: Delta ms = "); Serial.println(millis() - previousMillis);

  previousMillis = millis();
#endif  
}

void doingSomething50s()
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

  Serial.print(F("\nStarting ISR_Timer_Complex on ")); Serial.println(BOARD_NAME);
  Serial.println(MEGA_AVR_TIMER_INTERRUPT_VERSION);
  Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  Serial.print(F("TCB Clock Frequency = ")); 

#if USING_16MHZ  
  Serial.println(F("16MHz for highest accuracy"));
#elif USING_8MHZ  
  Serial.println(F("8MHz for very high accuracy"));
#else
  Serial.println(F("250KHz for lower accuracy but longer time"));
#endif

  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency
  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,
  // For 16-bit timer 1, 3, 4 and 5, set frequency from 0.2385 to some KHz
  // For 8-bit timer 2 (prescaler up to 1024, set frequency from 61.5Hz to some KHz

  // Interval in millisecs
  if (ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    lastMillis = millis();
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(lastMillis);
  }
  else
    Serial.println(F("Can't set ITimer1 correctly. Select another freq. or interval"));

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  ISR_Timer1.setInterval(2000L, doingSomething2s);
  ISR_Timer1.setInterval(5000L, doingSomething5s);
  ISR_Timer1.setInterval(10000L, doingSomething10s);
  ISR_Timer1.setInterval(50000L, doingSomething50s);

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  blynkTimer.setInterval(BLYNK_TIMER_MS, blynkDoingSomething2s);

  Blynk.begin(auth, ssid, pass, blynk_server, BLYNK_HARDWARE_PORT);

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
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  blynkTimer.run();
}
