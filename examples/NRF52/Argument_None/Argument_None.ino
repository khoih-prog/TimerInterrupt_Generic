/****************************************************************************************************************************
  Argument_None.ino
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
	#define LED_BLUE_PIN          7
#endif

#ifndef LED_RED
	#define LED_RED               8
#endif

#define TIMER0_INTERVAL_MS        1000
#define TIMER1_INTERVAL_MS        5000

volatile uint32_t preMillisTimer0 = 0;
volatile uint32_t preMillisTimer1 = 0;

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER1
NRF52Timer ITimer0(NRF_TIMER_1);

// Init NRF52 timer NRF_TIMER2
NRF52Timer ITimer1(NRF_TIMER_2);

void TimerHandler0()
{
	static bool toggle0 = false;
	static bool started = false;

	if (!started)
	{
		started = true;
		pinMode(LED_BUILTIN, OUTPUT);
	}

#if (TIMER_INTERRUPT_DEBUG > 0)
	static uint32_t curMillis = 0;

	curMillis = millis();

	if (curMillis > TIMER0_INTERVAL_MS)
	{
		Serial.print(F("ITimer0: millis() = "));
		Serial.print(curMillis);
		Serial.print(F(", delta = "));
		Serial.println(curMillis - preMillisTimer0);
	}

	preMillisTimer0 = curMillis;
#endif

	//timer interrupt toggles pin LED_BUILTIN
	digitalWrite(LED_BUILTIN, toggle0);
	toggle0 = !toggle0;
}

void TimerHandler1()
{
	static bool toggle1 = false;
	static bool started = false;

	if (!started)
	{
		started = true;
		pinMode(LED_BLUE_PIN, OUTPUT);
	}

#if (TIMER_INTERRUPT_DEBUG > 0)
	static uint32_t curMillis = 0;

	curMillis = millis();

	if (curMillis > TIMER1_INTERVAL_MS)
	{
		Serial.print(F("ITimer1: millis() = "));
		Serial.print(curMillis);
		Serial.print(F(", delta = "));
		Serial.println(curMillis - preMillisTimer1);
	}

	preMillisTimer1 = curMillis;
#endif

	//timer interrupt toggles outputPin
	digitalWrite(LED_BLUE_PIN, toggle1);
	toggle1 = !toggle1;
}

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LED_BLUE_PIN, OUTPUT);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting Argument_None on "));
	Serial.println(BOARD_NAME);
	Serial.println(NRF52_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
	{
		preMillisTimer0 = millis();
		Serial.print(F("Starting  ITimer0 OK, millis() = "));
		Serial.println(preMillisTimer0);
	}
	else
		Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

	// Interval in microsecs
	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
	{
		preMillisTimer1 = millis();
		Serial.print(F("Starting ITimer1 OK, millis() = "));
		Serial.println(preMillisTimer1);
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

void loop()
{

}
