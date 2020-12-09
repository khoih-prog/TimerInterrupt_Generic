/****************************************************************************************************************************
   Argument_Complex.ino
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
*****************************************************************************************************************************/

//These define's must be placed at the beginning before #include "TimerInterrupt.h"
#define TIMER_INTERRUPT_DEBUG      0

#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include "TimerInterrupt_Generic.h"

struct pinStruct
{
  unsigned int Pin1;
  unsigned int Pin2;
  unsigned int Pin3;
};

volatile pinStruct myOutputPins = { LED_BUILTIN, A0, A1 };

void TimerHandler1(unsigned int outputPinsAddress)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(((pinStruct *) outputPinsAddress)->Pin1, OUTPUT);
    pinMode(((pinStruct *) outputPinsAddress)->Pin2, INPUT_PULLUP);
    pinMode(((pinStruct *) outputPinsAddress)->Pin3, INPUT_PULLUP);
  }

  //timer interrupt toggles pins
  Serial.println("Toggle pin1 = " + String(((pinStruct *) outputPinsAddress)->Pin1) );
  digitalWrite(((pinStruct *) outputPinsAddress)->Pin1, toggle1);

  Serial.println("Read pin2 A0 (" + String(((pinStruct *) outputPinsAddress)->Pin2) + ") = " +
                 String( digitalRead(((pinStruct *) outputPinsAddress)->Pin2) ? "HIGH" : "LOW" ) );

  Serial.println("Read pin3 A1 (" + String(((pinStruct *) outputPinsAddress)->Pin3) + ") = " +
                 String( digitalRead(((pinStruct *) outputPinsAddress)->Pin3) ? "HIGH" : "LOW" ) );

  toggle1 = !toggle1;
}

#define TIMER1_INTERVAL_MS    1000

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStarting Argument_Complex on Arduino AVR board");
  Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,
  // For 16-bit timer 1, 3, 4 and 5, set frequency from 0.2385 to some KHz
  // For 8-bit timer 2 (prescaler up to 1024, set frequency from 61.5Hz to some KHz

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, (unsigned int) &myOutputPins))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");
}

void loop()
{
}
