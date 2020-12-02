/****************************************************************************************************************************
   Argument_Simple.ino
   For Arduino AVR boards (UNO, Nano, Mega, etc. )
   Written by Khoi Hoang
  
   TCNTx - Timer/Counter Register. The actual timer value is stored here.
   OCRx - Output Compare Register
   ICRx - Input Capture Register (only for 16bit timer)
   TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.
   TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt. 

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

   Version: 1.3.0

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
   1.1.0   K Hoang      10/11/2020 Initial Super-Library coding to merge all TimerInterrupt Libraries
   1.2.0   K Hoang      12/11/2020 Add STM32_TimerInterrupt Library
   1.3.0   K Hoang      01/12/2020 Add Mbed Mano-33-BLE Library. Add support to AVR UNO, Nano, Arduino Mini, Ethernet, BT. etc.
*****************************************************************************************************************************/
//These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include "TimerInterrupt_Generic.h"

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin2 = A0;

void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }

  //timer interrupt toggles pin outputPin, default LED_BUILTIN
  Serial.println("pin1 = " + String(outputPin) + " address: " + String((uint32_t) &outputPin) );
  digitalWrite(outputPin, toggle1);
  toggle1 = !toggle1;
}

void TimerHandler2(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle2 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }

  //timer interrupt toggles pin outputPin, default LED_BUILTIN
  digitalWrite(outputPin, toggle2);
  toggle2 = !toggle2;
}

#define TIMER1_INTERVAL_MS    1000

#define TIMER2_INTERVAL_MS    2000

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting Argument_Simple on Arduino AVR board");
  Serial.println("Version : " + String(TIMER_INTERRUPT_GENERIC_VERSION));
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,
  // For 16-bit timer 1, 3, 4 and 5, set frequency from 0.2385 to some KHz
  // For 8-bit timer 2 (prescaler up to 1024, set frequency from 61.5Hz to some KHz

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1))
  {
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
    Serial.println("OutputPin1 = " + String(outputPin1) + ", address = " + String((uint32_t) &outputPin1) );
  }
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");

  ITimer2.init();

  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2, outputPin2))
    Serial.println("Starting  ITimer2 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer2. Select another freq. or timer");
}

void loop()
{
}
