/****************************************************************************************************************************
  Argument_Simple.ino
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

#ifndef LED_BUILTIN
	#define LED_BUILTIN       25        // Pin LED_BUILTIN mapped to pin GPIO25 of RPI_PICO, control on-board LED
#endif

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin2 = 10;

// Init RPI_PICO_Timer
MBED_RPI_PICO_Timer ITimer1(1);
MBED_RPI_PICO_Timer ITimer2(2);

#define TIMER1_INTERVAL_MS    1000

// Never use Serial.print inside this mbed ISR. Will hang the system
void TimerHandler1(uint alarm_num)
{
	static bool toggle1 = false;

	///////////////////////////////////////////////////////////
	// Always call this for MBED RP2040 before processing ISR
	TIMER_ISR_START(alarm_num);
	///////////////////////////////////////////////////////////

	digitalWrite(outputPin1, toggle1);
	toggle1 = !toggle1;

	////////////////////////////////////////////////////////////
	// Always call this for MBED RP2040 after processing ISR
	TIMER_ISR_END(alarm_num);
	////////////////////////////////////////////////////////////
}

#define TIMER2_INTERVAL_MS    2000

void TimerHandler2(uint alarm_num)
{
	static bool toggle2 = false;

	///////////////////////////////////////////////////////////
	// Always call this for MBED RP2040 before processing ISR
	TIMER_ISR_START(alarm_num);
	///////////////////////////////////////////////////////////

	//timer interrupt toggles pin outputPin2
	digitalWrite(outputPin2, toggle2);
	toggle2 = !toggle2;

	////////////////////////////////////////////////////////////
	// Always call this for MBED RP2040 after processing ISR
	TIMER_ISR_END(alarm_num);
	////////////////////////////////////////////////////////////
}

void setup()
{
	pinMode(outputPin1, OUTPUT);
	pinMode(outputPin2, OUTPUT);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting Argument_Simple on "));
	Serial.println(BOARD_NAME);
	Serial.println(MBED_RPI_PICO_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);

	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
	{
		Serial.print(F("Starting ITimer1 OK, millis() = "));
		Serial.println(millis());

#if (TIMER_INTERRUPT_DEBUG > 1)
		Serial.print(F("OutputPin1 = "));
		Serial.print(outputPin1);
#endif
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));


	if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS * 1000, TimerHandler2))
	{
		Serial.print(F("Starting  ITimer2 OK, millis() = "));
		Serial.println(millis());

#if (TIMER_INTERRUPT_DEBUG > 1)
		Serial.print(F("OutputPin2 = "));
		Serial.print(outputPin2);
#endif
	}
	else
		Serial.println(F("Can't set ITimer2. Select another freq. or timer"));
}

void loop()
{
}
