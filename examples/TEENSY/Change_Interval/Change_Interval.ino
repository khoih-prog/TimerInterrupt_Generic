/****************************************************************************************************************************
  Change_Interval.ino
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
*/

#if !( defined(CORE_TEENSY) || defined(TEENSYDUINO) )
	#error This code is designed to run on Teensy platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
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

// For Teensy 4.0/4.1, F_BUS_ACTUAL = 150 MHz => max period is only 55922 us (~17.9 Hz)

#define TIMER_INTERVAL_MS        10

volatile uint32_t TimerCount = 0;

// You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

// Init Teensy timer TEENSY_TIMER_1
TeensyTimer ITimer(TEENSY_TIMER_1);

void printResult(uint32_t currTime)
{
	Serial.print(F("Time = "));
	Serial.print(currTime);
	Serial.print(F(", TimerCount = "));
	Serial.println(TimerCount);
}

void TimerHandler()
{
	static bool toggle = false;

	// Flag for checking to be sure ISR is working as SErial.print is not OK here in ISR
	TimerCount++;

	//timer interrupt toggles pin LED_BUILTIN
	digitalWrite(LED_BUILTIN, toggle);
	toggle = !toggle;
}

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting Change_Interval on "));
	Serial.println(BOARD_NAME);
	Serial.println(TEENSY_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler))
	{
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));
}

#define CHECK_INTERVAL_MS     10000L
#define CHANGE_INTERVAL_MS    20000L

void loop()
{
	static uint32_t lastTime = 0;
	static uint32_t lastChangeTime = 0;
	static uint32_t currTime;
	static uint32_t multFactor = 0;

	currTime = millis();

	if (currTime - lastTime > CHECK_INTERVAL_MS)
	{
		printResult(currTime);
		lastTime = currTime;

		if (currTime - lastChangeTime > CHANGE_INTERVAL_MS)
		{
			//setInterval(unsigned long interval, timerCallback callback)
			multFactor = (multFactor + 1) % 2;

			ITimer.setInterval(TIMER_INTERVAL_MS * 1000 * (multFactor + 1), TimerHandler);

			Serial.print(F("Changing Interval, Timer = "));
			Serial.println(TIMER_INTERVAL_MS * (multFactor + 1));

			lastChangeTime = currTime;
		}
	}
}
