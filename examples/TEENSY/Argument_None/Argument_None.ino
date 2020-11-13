/****************************************************************************************************************************
   Argument_None.ino
   For Teensy boards
   Written by Khoi Hoang

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

   Version: 1.2.0

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
   1.1.0   K Hoang      10/11/2020 Initial Super-Library coding to merge all TimerInterrupt Libraries
   1.2.0   K Hoang      12/11/2020 Add STM32_TimerInterrupt Library
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
*/

#if !( defined(CORE_TEENSY) || defined(TEENSYDUINO) )
  #error This code is designed to run on Teensy platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// Don't define Teensy_TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG      1

#include "TimerInterrupt_Generic.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

#ifndef LED_BLUE
  #define LED_BLUE          2
#endif

#ifndef LED_RED
  #define LED_RED           3
#endif


void  TimerHandler0(void)
{
  static bool toggle0 = false;
  static bool started = false;
  static uint32_t preMillis = 0;
  static uint32_t curMillis = 0;

  if (!started)
  {
    started = true;
    pinMode(LED_BUILTIN, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 0)
  curMillis = millis();
  Serial.println("ITimer0: millis() = " + String(curMillis) + ", delta = " + String(curMillis - preMillis));
  preMillis = curMillis;
#endif

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle0);
  toggle0 = !toggle0;
}

// For Teensy 4.0/4.1, F_BUS_ACTUAL = 150 MHz => max period is only 55922 us (~17.9 Hz)
#define TIMER0_INTERVAL_MS        50L

// You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

// Init Teensy timer TEENSY_TIMER_1
TeensyTimer ITimer0(TEENSY_TIMER_1);

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(100);
  
  Serial.println("\nStarting Argument_None on " + String(BOARD_NAME));
  Serial.println("Version : " + String(TIMER_INTERRUPT_GENERIC_VERSION));
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");
}

void loop()
{

}
