/****************************************************************************************************************************
  TimerInterruptTest.ino
  For SAM DUE boards
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

#if !( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
	#error This code is designed to run on SAM DUE board / platform! Please check your Tools->Board setting.
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
	#define LED_BLUE          2
#endif

#ifndef LED_RED
	#define LED_RED           8
#endif

unsigned int SWPin = 7;

// Index in SAMDUETimerInterrupt. To use to retrieve SAMDUETimerInterrupt Info if necessary
uint16_t Timer0_Index = 0;
uint16_t Timer1_Index = 0;

#define TIMER0_INTERVAL_MS        1000
#define TIMER0_DURATION_MS        5000

#define TIMER1_INTERVAL_MS        3000
#define TIMER1_DURATION_MS        15000

volatile uint32_t preMillisTimer0 = 0;
volatile uint32_t preMillisTimer1 = 0;

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
		Serial.print("ITimer0: millis() = ");
		Serial.print(curMillis);
		Serial.print(", delta = ");
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
		pinMode(LED_BLUE, OUTPUT);
	}

#if (TIMER_INTERRUPT_DEBUG > 0)
	static uint32_t curMillis = 0;

	curMillis = millis();

	if (curMillis > TIMER1_INTERVAL_MS)
	{
		Serial.print("ITimer1: millis() = ");
		Serial.print(curMillis);
		Serial.print(", delta = ");
		Serial.println(curMillis - preMillisTimer1);
	}

	preMillisTimer1 = curMillis;
#endif

	//timer interrupt toggles outputPin
	digitalWrite(LED_BLUE, toggle1);
	toggle1 = !toggle1;
}

uint16_t attachDueInterrupt(double microseconds, timerCallback callback, const char* TimerName)
{
	DueTimerInterrupt dueTimerInterrupt = DueTimer.getAvailable();

	dueTimerInterrupt.attachInterruptInterval(microseconds, callback);

	uint16_t timerNumber = dueTimerInterrupt.getTimerNumber();

	Serial.print(TimerName);
	Serial.print(F(" attached to Timer("));
	Serial.print(timerNumber);
	Serial.println(F(")"));

	return timerNumber;
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting TimerInterruptTest on "));
	Serial.println(BOARD_NAME);
	Serial.println(SAMDUE_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));
	Serial.print(F("Timer Frequency = "));
	Serial.print(SystemCoreClock / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	Timer0_Index = attachDueInterrupt(TIMER0_INTERVAL_MS * 1000, TimerHandler0, "ITimer0");

	// Interval in microsecs
	Timer1_Index = attachDueInterrupt(TIMER1_INTERVAL_MS * 1000, TimerHandler1, "ITimer1");
}

void loop()
{
	static unsigned long lastTimer0   = 0;
	static bool timer0Stopped         = false;


	if (millis() - lastTimer0 > TIMER0_DURATION_MS)
	{
		lastTimer0 = millis();

		if (timer0Stopped)
		{
			preMillisTimer0 = millis();

			Serial.print(F("Start ITimer0, millis() = "));
			Serial.println(preMillisTimer0);

			DueTimerPtr[Timer0_Index].restartTimer();
		}
		else
		{
			Serial.print(F("Stop ITimer0, millis() = "));
			Serial.println(millis());

			DueTimerPtr[Timer0_Index].stopTimer();
		}

		timer0Stopped = !timer0Stopped;
	}

	static unsigned long lastTimer1   = 0;
	static bool timer1Stopped         = false;

	if (millis() - lastTimer1 > TIMER1_DURATION_MS)
	{
		lastTimer1 = millis();

		if (timer1Stopped)
		{
			preMillisTimer1 = millis();

			Serial.print(F("Start ITimer1, millis() = "));
			Serial.println(preMillisTimer1);

			DueTimerPtr[Timer1_Index].restartTimer();
		}
		else
		{
			Serial.print(F("Stop ITimer1, millis() = "));
			Serial.println(millis());

			DueTimerPtr[Timer1_Index].stopTimer();
		}

		timer1Stopped = !timer1Stopped;
	}
}
