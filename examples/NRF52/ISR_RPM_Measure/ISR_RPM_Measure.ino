/****************************************************************************************************************************
  ISR_RPM_Measure.ino
  For NRF52 boards
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

   We use interrupt to detect whenever the SW is active, set a flag
   then use timer to count the time between active state
*/

#if !(defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || \
      defined(NRF52840_CLUE) || defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || \
      defined(MDBT50Q_RX) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
#error This code is designed to run on nRF52 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "TimerInterrupt_Generic.h"

//#ifndef LED_BUILTIN
//  #define LED_BUILTIN         3
//#endif

#ifndef LED_BLUE_PIN
	#define LED_BLUE_PIN          9
#endif

#ifndef LED_RED
	#define LED_RED               8
#endif

unsigned int interruptPin = 7;

#define TIMER1_INTERVAL_MS          1
#define DEBOUNCING_INTERVAL_MS      80

#define LOCAL_DEBUG                 1

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER1
NRF52Timer ITimer1(NRF_TIMER_1);

// Init NRF52 timer NRF_TIMER2
//NRF52Timer ITimer1(NRF_TIMER_4);

volatile unsigned long rotationTime = 0;
float RPM       = 0.00;
float avgRPM    = 0.00;

volatile int debounceCounter;

volatile bool activeState = false;

void detectRotation()
{
	activeState = true;
}

void TimerHandler1()
{
	if ( activeState )
	{
		// Reset to prepare for next round of interrupt
		activeState = false;

		if (debounceCounter >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS )
		{

			//min time between pulses has passed
			RPM = (float) ( 60000.0f / ( rotationTime * TIMER1_INTERVAL_MS ) );

			avgRPM = ( 2 * avgRPM + RPM) / 3,

#if (LOCAL_DEBUG > 0)
			Serial.print("RPM = ");
			Serial.print(avgRPM);
			Serial.print(", rotationTime ms = ");
			Serial.println(rotationTime * TIMER1_INTERVAL_MS);
#endif

			rotationTime = 0;
			debounceCounter = 0;
		}
		else
			debounceCounter++;
	}
	else
	{
		debounceCounter++;
	}

	if (rotationTime >= 5000)
	{
		// If idle, set RPM to 0, don't increase rotationTime
		RPM = 0;

#if (LOCAL_DEBUG > 0)
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
	pinMode(interruptPin, INPUT_PULLUP);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting ISR_RPM_Measure on "));
	Serial.println(BOARD_NAME);
	Serial.println(NRF52_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs, must multiply to 1000 here or crash
	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
	{
		Serial.print(F("Starting  ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

	// Assumming the interruptPin will go LOW
	attachInterrupt(digitalPinToInterrupt(interruptPin), detectRotation, FALLING);
}

void loop()
{

}
