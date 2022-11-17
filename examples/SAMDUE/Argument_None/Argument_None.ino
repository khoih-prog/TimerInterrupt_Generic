/****************************************************************************************************************************
  Argument_None.ino
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

#define TIMER0_INTERVAL_MS        1000L
#define TIMER1_INTERVAL_MS        2000L
#define TIMER2_INTERVAL_MS        5000L
#define TIMER3_INTERVAL_MS        8000L

volatile uint32_t preMillisTimer0 = 0;
volatile uint32_t preMillisTimer1 = 0;
volatile uint32_t preMillisTimer2 = 0;
volatile uint32_t preMillisTimer3 = 0;

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

void TimerHandler2()
{
#if (TIMER_INTERRUPT_DEBUG > 0)
	static uint32_t curMillis = 0;

	curMillis = millis();

	if (curMillis > TIMER2_INTERVAL_MS)
	{
		Serial.print("ITimer2: millis() = ");
		Serial.print(curMillis);
		Serial.print(", delta = ");
		Serial.println(curMillis - preMillisTimer2);
	}

	preMillisTimer2 = curMillis;
#endif
}

void TimerHandler3()
{
#if (TIMER_INTERRUPT_DEBUG > 0)
	static uint32_t curMillis = 0;

	curMillis = millis();

	if (curMillis > TIMER3_INTERVAL_MS)
	{
		Serial.print("ITimer3: millis() = ");
		Serial.print(curMillis);
		Serial.print(", delta = ");
		Serial.println(curMillis - preMillisTimer3);
	}

	preMillisTimer3 = curMillis;
#endif
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

	Serial.print(F("\nStarting Argument_None on "));
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
	uint32_t curMillis = millis();
	attachDueInterrupt(TIMER0_INTERVAL_MS * 1000, TimerHandler0, "ITimer0");
	// Just to have precise start time for the first time
	preMillisTimer0 = (curMillis + millis()) / 2;

	curMillis = millis();
	attachDueInterrupt(TIMER1_INTERVAL_MS * 1000, TimerHandler1, "ITimer1");
	// Just to have precise start time for the first time
	preMillisTimer1 = (curMillis + millis()) / 2;

	curMillis = millis();
	attachDueInterrupt(TIMER2_INTERVAL_MS * 1000, TimerHandler2, "ITimer2");
	// Just to have precise start time for the first time
	preMillisTimer2 = (curMillis + millis()) / 2;

	curMillis = millis();
	attachDueInterrupt(TIMER3_INTERVAL_MS * 1000, TimerHandler3, "ITimer3");
	// Just to have precise start time for the first time
	preMillisTimer3 = (curMillis + millis()) / 2;
}

void loop()
{

}
