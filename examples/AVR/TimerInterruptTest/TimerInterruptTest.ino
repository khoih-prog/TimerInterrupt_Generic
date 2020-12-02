/****************************************************************************************************************************
   TimerInterruptTest.ino
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

//These define's must be placed at the beginning before #include "TimerInterrupt.h"
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false


#include "TimerInterrupt_Generic.h"

void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }

  Serial.println("ITimer1 called, millis() = " + String(millis()));

  //timer interrupt toggles pin LED_BUILTIN
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

  Serial.println("ITimer2 called, millis() = " + String(millis()));

  //timer interrupt toggles outputPin
  digitalWrite(outputPin, toggle2);
  toggle2 = !toggle2;
}

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin2 = A0;

#define TIMER1_INTERVAL_MS    10000
#define TIMER1_FREQUENCY      (float) (1000.0f / TIMER1_INTERVAL_MS)
#define TIMER1_DURATION_MS    0 //(10 * TIMER1_INTERVAL_MS)

#define TIMER2_INTERVAL_MS    13000
#define TIMER2_FREQUENCY      (float) (1000.0f / TIMER2_INTERVAL_MS)
#define TIMER2_DURATION_MS    0   //(20 * TIMER2_INTERVAL_MS)

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting TimerInterruptTest on Arduino AVR board");
  Serial.println("Version : " + String(TIMER_INTERRUPT_GENERIC_VERSION));
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency

  ITimer1.init();
  //ITimer3.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  //if (ITimer3.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
    Serial.println("Starting  ITimer3 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer3. Select another freq., duration or timer");

  ITimer2.init();
  //ITimer4.init();

  //if (ITimer4.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2, outputPin2, TIMER2_DURATION_MS))
  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2, outputPin2, TIMER2_DURATION_MS))
    Serial.println("Starting  ITimer4 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer4. Select another freq., duration or timer");
}

void loop()
{

#if 0
  static unsigned long lastTimer1 = 0;
  static unsigned long lastTimer2 = 0;

  static bool timerPaused         = false;
  static bool timerResumed        = false;

  if (millis() - lastTimer1 > TIMER1_DURATION_MS * 3)
  {
    if (millis() - lastTimer2 > TIMER2_DURATION_MS * 3)
    {
      lastTimer2 = millis();
      Serial.println("Re-enable ITimer2, millis() = " + String(millis()));
      ITimer2.reattachInterrupt(TIMER2_DURATION_MS);
    }

    lastTimer1 = millis();
    // try reinit timer
    Serial.println("Re-enable ITimer1, millis() = " + String(millis()) + " count = " + String(ITimer1.getCount()));
    ITimer1.reattachInterrupt(TIMER1_DURATION_MS);
    timerPaused   = false;
    timerResumed  = false;
  }
  else if ( !timerPaused && (millis() - lastTimer1 > TIMER1_DURATION_MS / 2) )
  {
    timerPaused = true;
    Serial.println("Pause ITimer1, millis() = " + String(millis()) + " count = " + String(ITimer1.getCount()));
    ITimer1.pauseTimer();
  }
  else if ( !timerResumed && (millis() - lastTimer1 > ( TIMER1_DURATION_MS * 3 ) / 2) )
  {
    timerResumed = true;
    Serial.println("Resume ITimer1, millis() = " + String(millis()) + " count = " + String(ITimer1.getCount()));
    ITimer1.resumeTimer();
  }
#endif

}
