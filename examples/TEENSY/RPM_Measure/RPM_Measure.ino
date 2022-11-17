/****************************************************************************************************************************
  RPM_Measure.ino
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

   RPM Measuring uses high frequency hardware timer 1Hz == 1ms) to measure the time from of one rotation, in ms
   then convert to RPM. One rotation is detected by reading the state of a magnetic REED SW or IR LED Sensor
   Asssuming LOW is active.
   For example: Max speed is 600RPM => 10 RPS => minimum 100ms a rotation. We'll use 80ms for debouncing
   If the time between active state is less than 8ms => consider noise.
   RPM = 60000 / (rotation time in ms)

   We use interrupt to detect whenever the SW is active, set a flag then use timer to count the time between active state

   RPM Measuring uses high frequency hardware timer 1Hz == 1ms) to measure the time from of one rotation, in ms
   then convert to RPM. One rotation is detected by reading the state of a magnetic REED SW or IR LED Sensor
   Asssuming LOW is active.
   For example: Max speed is 600RPM => 10 RPS => minimum 100ms a rotation. We'll use 80ms for debouncing
   If the time between active state is less than 8ms => consider noise.
   RPM = 60000 / (rotation time in ms)

   You can also use interrupt to detect whenever the SW is active, set a flag then use timer to count the time between active state
*/

#if !( defined(CORE_TEENSY) || defined(TEENSYDUINO) )
	#error This code is designed to run on Teensy platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         2
#define _TIMERINTERRUPT_LOGLEVEL_     0

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

unsigned int SWPin = 7;

#define TIMER_INTERVAL_US           100L
#define DEBOUNCING_INTERVAL_MS      80L

#define LOCAL_DEBUG                 1

// You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

// Init Teensy timer TEENSY_TIMER_3
TeensyTimer ITimer(TEENSY_TIMER_3);

volatile unsigned long rotationTime = 0;
float RPM       = 0.00;
float avgRPM    = 0.00;

volatile int debounceCounter;

void TimerHandler()
{
	if ( !digitalRead(SWPin) && (debounceCounter >= (DEBOUNCING_INTERVAL_MS * 1000 ) / TIMER_INTERVAL_US ) )
	{
		//min time between pulses has passed
		RPM = (float) ( ( 60000.0f * 1000 ) / ( rotationTime * TIMER_INTERVAL_US ) );

		avgRPM = ( 2 * avgRPM + RPM) / 3,

#if (TIMER_INTERRUPT_DEBUG > 1)
		Serial.print("RPM = ");
		Serial.print(avgRPM);
		Serial.print(", rotationTime ms = ");
		Serial.println( (rotationTime * TIMER_INTERVAL_US) / 1000);
#endif

		rotationTime = 0;
		debounceCounter = 0;
	}
	else
	{
		debounceCounter++;
	}

	if (rotationTime >= 5000)
	{
		// If idle, set RPM to 0, don't increase rotationTime
		RPM = 0;

#if (TIMER_INTERRUPT_DEBUG > 1)
		Serial.print("RPM = ");
		Serial.print(RPM);
		Serial.print(", rotationTime = ");
		Serial.println(rotationTime);
#endif

		rotationTime = 0;
	}
	else
	{
		rotationTime++;
	}
}

void setup()
{
	pinMode(SWPin, INPUT_PULLUP);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting RPM_Measure on "));
	Serial.println(BOARD_NAME);
	Serial.println(TEENSY_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer.attachInterruptInterval(TIMER_INTERVAL_US, TimerHandler))
	{
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));

	Serial.flush();
}

void loop()
{

}
