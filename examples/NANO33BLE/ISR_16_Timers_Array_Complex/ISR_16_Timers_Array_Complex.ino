/****************************************************************************************************************************
   ISR_16_Timers_Array.ino
   For NRF52 boards using mbed-RTOS such as Nano-33-BLE
   Written by Khoi Hoang

   Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
   unsigned long miliseconds), you just consume only one NRF52 timer and avoid conflicting with other cores' tasks.
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

   Version: 1.3.0

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
   1.1.0   K Hoang      10/11/2020 Initial Super-Library coding to merge all TimerInterrupt Libraries
   1.2.0   K Hoang      12/11/2020 Add STM32_TimerInterrupt Library
   1.3.0   K Hoang      01/12/2020 Add Mbed Nano-33-BLE Library. Add support to AVR UNO, Nano, Arduino Mini, Ethernet, BT. etc.
*****************************************************************************************************************************/
/*
   Notes:
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

#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #error This code is designed to run on nRF52-based Nano-33-BLE boards using mbed-RTOS platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "NRF52_MBED_TimerInterrupt.h"
// For Nano33-BLE, don't use Serial.print() in ISR as system will definitely hang.
#define TIMER_INTERRUPT_DEBUG      1

#include "TimerInterrupt_Generic.h"

#include "ISR_Timer_Generic.h"

#include <SimpleTimer.h>              // https://github.com/schinken/SimpleTimer

#ifndef LED_BUILTIN
#define LED_BUILTIN       D13
#endif

#ifndef LED_BLUE
#define LED_BLUE          D7
#endif

#ifndef LED_RED
#define LED_RED           D8
#endif

#define HW_TIMER_INTERVAL_US      10000L

volatile uint32_t startMillis = 0;

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1,NRF_TIMER_3,NRF_TIMER_4 (1,3 and 4)
// If you select the already-used NRF_TIMER_0 or NRF_TIMER_2, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER3
NRF52_MBED_Timer ITimer(NRF_TIMER_3);

// Init NRF52_ISR_Timer
// Each NRF52_ISR_Timer can service 16 different ISR-based timers
ISR_Timer NRF52_ISR_Timer;

#define LED_TOGGLE_INTERVAL_MS        2000L

void TimerHandler(void)
{
  static bool toggle  = false;
  static int timeRun  = 0;

  NRF52_ISR_Timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * 1000) / HW_TIMER_INTERVAL_US) )
  {
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  (void);

/////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      false

#if USE_COMPLEX_STRUCT

  typedef struct 
  {
    irqCallback   irqCallbackFunc;
    uint32_t      TimerInterval;
    unsigned long deltaMillis;
    unsigned long previousMillis;
  } ISRTimerData;
  
  // In NRF52, avoid doing something fancy in ISR, for example Serial.print()
  // The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
  // Or you can get this run-time error / crash
  
  void doingSomething(int index);

#else

  volatile unsigned long deltaMillis    [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  volatile unsigned long previousMillis [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  // You can assign any interval for any timer here, in milliseconds
  uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
  {
    5000L,  10000L,  15000L,  20000L,  25000L,  30000L,  35000L,  40000L,
    45000L, 50000L,  55000L,  60000L,  65000L,  70000L,  75000L,  80000L
  };
  
  void doingSomething(int index)
  {
    unsigned long currentMillis  = millis();
    
    deltaMillis[index]    = currentMillis - previousMillis[index];
    previousMillis[index] = currentMillis;
  }

#endif

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomething0()
{
  doingSomething(0);
}

void doingSomething1()
{
  doingSomething(1);
}

void doingSomething2()
{
  doingSomething(2);
}

void doingSomething3()
{
  doingSomething(3);
}

void doingSomething4()
{
  doingSomething(4);
}

void doingSomething5()
{
  doingSomething(5);
}

void doingSomething6()
{
  doingSomething(6);
}

void doingSomething7()
{
  doingSomething(7);
}

void doingSomething8()
{
  doingSomething(8);
}

void doingSomething9()
{
  doingSomething(9);
}

void doingSomething10()
{
  doingSomething(10);
}

void doingSomething11()
{
  doingSomething(11);
}

void doingSomething12()
{
  doingSomething(12);
}

void doingSomething13()
{
  doingSomething(13);
}

void doingSomething14()
{
  doingSomething(14);
}

void doingSomething15()
{
  doingSomething(15);
}

#if USE_COMPLEX_STRUCT

  ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
  {
    //irqCallbackFunc, TimerInterval, deltaMillis, previousMillis
    { doingSomething0,    5000L, 0, 0 },
    { doingSomething1,   10000L, 0, 0 },
    { doingSomething2,   15000L, 0, 0 },
    { doingSomething3,   20000L, 0, 0 },
    { doingSomething4,   25000L, 0, 0 },
    { doingSomething5,   30000L, 0, 0 },
    { doingSomething6,   35000L, 0, 0 },
    { doingSomething7,   40000L, 0, 0 },
    { doingSomething8,   45000L, 0, 0 },
    { doingSomething9,   50000L, 0, 0 },
    { doingSomething10,  55000L, 0, 0 },
    { doingSomething11,  60000L, 0, 0 },
    { doingSomething12,  65000L, 0, 0 },
    { doingSomething13,  70000L, 0, 0 },
    { doingSomething14,  75000L, 0, 0 },
    { doingSomething15,  80000L, 0, 0 }
  };
  
  void doingSomething(int index)
  {
    unsigned long currentMillis  = millis();
    
    curISRTimerData[index].deltaMillis    = currentMillis - curISRTimerData[index].previousMillis;
    curISRTimerData[index].previousMillis = currentMillis;
  }

#else

  irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
  {
    doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
    doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
    doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
    doingSomething12, doingSomething13, doingSomething14, doingSomething15
  };

#endif
///////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static unsigned long previousMillis = startMillis;

  unsigned long currMillis = millis();

  Serial.printf("SimpleTimer : %lus, ms = %lu, Dms : %lu\n", SIMPLE_TIMER_MS / 1000, currMillis, currMillis - previousMillis);

  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT    
    Serial.printf("Timer : %d, programmed : %lu, actual : %lu\n", i, curISRTimerData[i].TimerInterval, curISRTimerData[i].deltaMillis);
#else
    Serial.printf("Timer : %d, programmed : %lu, actual : %lu\n", i, TimerInterval[i], deltaMillis[i]);
#endif    
  }

  previousMillis = currMillis;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  Serial.printf("\nStarting ISR_16_Timers_Array_Complex on %s\n", BOARD_NAME);
  Serial.printf("Version : v%s\n", TIMER_INTERRUPT_GENERIC_VERSION);

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    startMillis = millis();
    Serial.printf("Starting  ITimer OK, millis() = %ld\n", startMillis);
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISRTimerData[i].previousMillis = startMillis;
    NRF52_ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
    previousMillis[i] = startMillis;
    NRF52_ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
#endif    
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      3000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}