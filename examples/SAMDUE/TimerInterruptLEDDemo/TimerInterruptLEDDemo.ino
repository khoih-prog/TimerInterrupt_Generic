/****************************************************************************************************************************
  TimerInterruptLEDDemo.ino
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
#include "ISR_Timer_Generic.h"

//#ifndef LED_BUILTIN
//  #define LED_BUILTIN       13
//#endif

#ifndef LED_BLUE
	#define LED_BLUE          2
#endif

#ifndef LED_RED
	#define LED_RED           8
#endif

#define HW_TIMER_INTERVAL_MS      10

// Init SAMDUE_ISR_Timer
// Each SAMDUE_ISR_Timer can service 16 different ISR-based timers
ISR_Timer SAMDUE_ISR_Timer;

#define TIMER_INTERVAL_1S             1000L
#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L

void TimerHandler()
{
	SAMDUE_ISR_Timer.run();
}

// In SAM-DUE, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething1()
{
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	Serial.println("G");
}

void doingSomething2()
{
	digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
	Serial.println("B");
}
void doingSomething3()
{
	digitalWrite(LED_RED, !digitalRead(LED_RED));
	Serial.println("R");
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

	Serial.print(F("\nStarting TimerInterruptLEDDemo on "));
	Serial.println(BOARD_NAME);
	Serial.println(SAMDUE_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));
	Serial.print(F("Timer Frequency = "));
	Serial.print(SystemCoreClock / 1000000);
	Serial.println(F(" MHz"));

	// Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
	//HardwareTimer *MyTim = new HardwareTimer(Instance);

	// configure pin in output mode
	pinMode(LED_BUILTIN,  OUTPUT);
	pinMode(LED_BLUE,     OUTPUT);
	pinMode(LED_RED,      OUTPUT);

	// Interval in microsecs
	attachDueInterrupt(HW_TIMER_INTERVAL_MS * 1000, TimerHandler, "ITimer");

	// Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
	// You can use up to 16 timer for each SAMDUE_ISR_Timer
	SAMDUE_ISR_Timer.setInterval(TIMER_INTERVAL_1S,  doingSomething1);
	SAMDUE_ISR_Timer.setInterval(TIMER_INTERVAL_2S,  doingSomething2);
	SAMDUE_ISR_Timer.setInterval(TIMER_INTERVAL_5S,  doingSomething3);
}


void loop()
{
	/* Nothing to do all is done by hardware. Even no interrupt required. */
}
