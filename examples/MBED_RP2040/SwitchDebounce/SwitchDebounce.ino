/****************************************************************************************************************************
  SwitchDebounce.ino
  For MBED RP2040-based boards such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040.
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt_Generic
  Licensed under MIT license

  The RPI_PICO system timer peripheral provides a global microsecond timebase for the system, and generates
  interrupts based on this timebase. It supports the following features:
    • A single 64-bit counter, incrementing once per microsecond
    • This counter can be read from a pair of latching registers, for race-free reads over a 32-bit bus.
    • Four alarms: match on the lower 32 bits of counter, IRQ on match: TIMER_IRQ_0-TIMER_IRQ_3

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one RPI_PICO timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy
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

#if ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
      defined(ARDUINO_GENERIC_RP2040) ) && defined(ARDUINO_ARCH_MBED)
#define USING_MBED_RPI_PICO_TIMER_INTERRUPT        true
#else
#error This code is intended to run on the MBED RASPBERRY_PI_PICO platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
#define _TIMERINTERRUPT_LOGLEVEL_     4

#include "TimerInterrupt_Generic.h"

#define PIN_D1              1         // Pin D1 mapped to pin GPIO1 of RPI_PICO

unsigned int SWPin = PIN_D1;

#define TIMER1_INTERVAL_MS        1
#define DEBOUNCING_INTERVAL_MS    100
#define LONG_PRESS_INTERVAL_MS    5000

#define LOCAL_DEBUG               0

// Init MBED_RPI_PICO_Timer, can use any from 0-15 pseudo-hardware timers
MBED_RPI_PICO_Timer ITimer1(1);

volatile bool SWPressed     = false;
volatile bool SWLongPressed = false;

volatile unsigned long SWPressedTime;
volatile unsigned long SWReleasedTime;

volatile bool newData = false;

// Never use Serial.print inside this mbed ISR. Will hang the system
void TimerHandler1(uint alarm_num)
{
	static unsigned int debounceCountSWPressed  = 0;
	static unsigned int debounceCountSWReleased = 0;

	unsigned long currentMillis = millis();

	///////////////////////////////////////////////////////////
	// Always call this for MBED RP2040 before processing ISR
	TIMER_ISR_START(alarm_num);
	///////////////////////////////////////////////////////////

	if ( (!digitalRead(SWPin)) )
	{
		// Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
		debounceCountSWReleased = 0;

		if (++debounceCountSWPressed >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS)
		{
			// Call and flag SWPressed
			if (!SWPressed)
			{
				SWPressedTime = currentMillis;

				SWPressed = true;
				// Do something for SWPressed here in ISR
				// But it's better to use outside software timer to do your job instead of inside ISR
				//Your_Response_To_Press();

				// flag newData
				newData = true;
			}

			if (debounceCountSWPressed >= LONG_PRESS_INTERVAL_MS / TIMER1_INTERVAL_MS)
			{
				// Call and flag SWLongPressed
				if (!SWLongPressed)
				{
					SWLongPressed = true;
					// Do something for SWLongPressed here in ISR
					// But it's better to use outside software timer to do your job instead of inside ISR
					//Your_Response_To_Long_Press();
				}

				// flag newData
				newData = true;
			}
		}
	}
	else
	{
		debounceCountSWPressed = 0;

		// Start debouncing counting debounceCountSWReleased and clear debounceCountSWPressed
		if ( SWPressed && (++debounceCountSWReleased >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS))
		{
			SWReleasedTime = currentMillis;

			SWPressed     = false;
			SWLongPressed = false;

			// Do something for !SWPressed here in ISR
			// But it's better to use outside software timer to do your job instead of inside ISR
			//Your_Response_To_Release();

			// flag newData
			newData = true;
		}
	}

	////////////////////////////////////////////////////////////
	// Always call this for MBED RP2040 after processing ISR
	TIMER_ISR_END(alarm_num);
	////////////////////////////////////////////////////////////
}

void check_status()
{
	static unsigned long checkstatus_timeout = 0;
	static unsigned long currentMillis;

#define STATUS_CHECK_INTERVAL     50L

	// Send status report every STATUS_REPORT_INTERVAL (1) seconds: we don't need to send updates frequently if there is no status change.
	if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
	{
		if (newData)
		{
			if ( !digitalRead(SWPin) && SWPressed )
			{
				if (!SWLongPressed)
				{
					Serial.print("SW Pressed, from millis() = ");
					Serial.println(SWPressedTime);
				}
				else
				{
					currentMillis = millis();

					Serial.print("SW Long Pressed, total time ms = ");
					Serial.print(currentMillis);
					Serial.print(" - ");
					Serial.print(SWPressedTime);
					Serial.print(" = ");
					Serial.println(currentMillis - SWPressedTime);
				}
			}
			else if ( digitalRead(SWPin) && !SWPressed && !SWLongPressed)
			{
				Serial.print("SW Released, from millis() = ");
				Serial.println(SWReleasedTime);

				Serial.print("SW Pressed total time ms = ");
				Serial.println(SWReleasedTime - SWPressedTime);
			}

			newData = false;
		}

		checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
	}
}

void setup()
{
	pinMode(SWPin, INPUT_PULLUP);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting SwitchDebounce on "));
	Serial.println(BOARD_NAME);
	Serial.println(MBED_RPI_PICO_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);

	// Interval in microsecs
	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
	{
		Serial.print(F("Starting ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

void loop()
{
	check_status();
}
