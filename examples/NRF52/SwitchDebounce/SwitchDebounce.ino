/****************************************************************************************************************************
  SwitchDebounce.ino
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

   Switch Debouncing uses high frequency hardware timer 50Hz == 20ms) to measure the time from the SW is pressed,
   debouncing time is 100ms => SW is considered pressed if timer count is > 5, then call / flag SW is pressed
   When the SW is released, timer will count (debounce) until more than 50ms until consider SW is released.
   We can set to flag or call a function whenever SW is pressed more than certain predetermined time, even before
   SW is released.
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
//  #define LED_BUILTIN       13
//#endif

#ifndef LED_BLUE
	#define LED_BLUE          7
#endif

#ifndef LED_RED
	#define LED_RED           8
#endif

unsigned int SWPin = 11;

#define TIMER1_INTERVAL_MS        20
#define DEBOUNCING_INTERVAL_MS    100
#define LONG_PRESS_INTERVAL_MS    5000

#define LOCAL_DEBUG               1

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER1
NRF52Timer ITimer(NRF_TIMER_1);

volatile bool SWPressed     = false;
volatile bool SWLongPressed = false;

void TimerHandler()
{
	static unsigned int debounceCountSWPressed  = 0;
	static unsigned int debounceCountSWReleased = 0;

#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long SWPressedTime;
	static unsigned long SWReleasedTime;

	unsigned long currentMillis = millis();
#endif

	static bool started = false;

	if (!started)
	{
		started = true;
		pinMode(SWPin, INPUT_PULLUP);
	}

	if ( (!digitalRead(SWPin)) )
	{
		// Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
		debounceCountSWReleased = 0;

		if (++debounceCountSWPressed >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS)
		{
			// Call and flag SWPressed
			if (!SWPressed)
			{
#if (TIMER_INTERRUPT_DEBUG > 1)
				SWPressedTime = currentMillis;

				Serial.print("SW Press, from millis() = ");
				Serial.println(SWPressedTime - DEBOUNCING_INTERVAL_MS);
#endif

				SWPressed = true;
				// Do something for SWPressed here in ISR
				// But it's better to use outside software timer to do your job instead of inside ISR
				//Your_Response_To_Press();
			}

			if (debounceCountSWPressed >= LONG_PRESS_INTERVAL_MS / TIMER1_INTERVAL_MS)
			{
				// Call and flag SWLongPressed
				if (!SWLongPressed)
				{
#if (TIMER_INTERRUPT_DEBUG > 1)
					Serial.print("SW Long Pressed, total time ms = ");
					Serial.print(currentMillis);
					Serial.print(" - ");
					Serial.print(SWPressedTime - DEBOUNCING_INTERVAL_MS);
					Serial.print(" = ");
					Serial.println(currentMillis - SWPressedTime + DEBOUNCING_INTERVAL_MS);
#endif

					SWLongPressed = true;
					// Do something for SWLongPressed here in ISR
					// But it's better to use outside software timer to do your job instead of inside ISR
					//Your_Response_To_Long_Press();
				}
			}
		}
	}
	else
	{
		// Start debouncing counting debounceCountSWReleased and clear debounceCountSWPressed
		if ( SWPressed && (++debounceCountSWReleased >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS))
		{
#if (TIMER_INTERRUPT_DEBUG > 1)
			SWReleasedTime = currentMillis;

			// Call and flag SWPressed
			Serial.print("SW Released, from millis() = ");
			Serial.println(SWReleasedTime);
#endif

			SWPressed     = false;
			SWLongPressed = false;

			// Do something for !SWPressed here in ISR
			// But it's better to use outside software timer to do your job instead of inside ISR
			//Your_Response_To_Release();

			// Call and flag SWPressed
#if (TIMER_INTERRUPT_DEBUG > 1)
			Serial.print("SW Pressed total time ms = ");
			Serial.println(SWReleasedTime - SWPressedTime);
#endif

			debounceCountSWPressed = 0;
		}
	}
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting SwitchDebounce on "));
	Serial.println(BOARD_NAME);
	Serial.println(NRF52_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler))
	{
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));
}

void loop()
{

}
