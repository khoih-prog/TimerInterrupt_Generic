/****************************************************************************************************************************
  ISR_Timer_Complex.ino
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

   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written
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
#include "ISR_Timer_Generic.h"

#include <SimpleTimer.h>              // https://github.com/schinken/SimpleTimer

#ifndef LED_BUILTIN
	#define LED_BUILTIN       13
#endif

#ifndef LED_BLUE
	#define LED_BLUE          2
#endif

#ifndef LED_RED
	#define LED_RED           3
#endif

#define HW_TIMER_INTERVAL_MS      1L

volatile uint32_t lastMillis = 0;

// You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

// Init Teensy timer TEENSY_TIMER_1
TeensyTimer ITimer(TEENSY_TIMER_1);

// Init Teensy_ISR_Timer
// Each Teensy_ISR_Timer can service 16 different ISR-based timers
ISR_Timer Teensy_ISR_Timer;

#ifndef LED_BUILTIN
	#define LED_BUILTIN       13
#endif

#define LED_TOGGLE_INTERVAL_MS        2000L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

void TimerHandler()
{
	static bool toggle  = false;
	static bool started = false;
	static int timeRun  = 0;

	Teensy_ISR_Timer.run();

	// Toggle LED every LED_TOGGLE_INTERVAL_MS = 5000ms = 5s
	if (++timeRun == (LED_TOGGLE_INTERVAL_MS / HW_TIMER_INTERVAL_MS) )
	{
		timeRun = 0;

		if (!started)
		{
			started = true;
			pinMode(LED_BUILTIN, OUTPUT);
		}

		//timer interrupt toggles pin LED_BUILTIN
		digitalWrite(LED_BUILTIN, toggle);
		toggle = !toggle;
	}
}

// In Teensy, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)
	static unsigned long previousMillis = lastMillis;
	unsigned long deltaMillis = millis() - previousMillis;


	if (previousMillis > TIMER_INTERVAL_2S)
	{
		Serial.print("2s: Delta ms = ");
		Serial.println(deltaMillis);
	}

	previousMillis = millis();
#endif
}

// In Teensy, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething5s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)
	static unsigned long previousMillis = lastMillis;
	unsigned long deltaMillis = millis() - previousMillis;


	if (previousMillis > TIMER_INTERVAL_5S)
	{
		Serial.print("5s: Delta ms = ");
		Serial.println(deltaMillis);
	}

	previousMillis = millis();
#endif
}

// In Teensy, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething11s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)
	static unsigned long previousMillis = lastMillis;
	unsigned long deltaMillis = millis() - previousMillis;


	if (previousMillis > TIMER_INTERVAL_11S)
	{
		Serial.print("11s: Delta ms = ");
		Serial.println(deltaMillis);
	}

	previousMillis = millis();
#endif
}

// In Teensy, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething101s()
{
#if (TIMER_INTERRUPT_DEBUG > 0)
	static unsigned long previousMillis = lastMillis;
	unsigned long deltaMillis = millis() - previousMillis;


	if (previousMillis > TIMER_INTERVAL_101S)
	{
		Serial.print("101s: Delta ms = ");
		Serial.println(deltaMillis);
	}

	previousMillis = millis();
#endif
}

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
	static unsigned long previousMillis = lastMillis;

	Serial.print(F("blynkDoingSomething2s: Delta programmed ms = "));
	Serial.print(SIMPLE_TIMER_MS);
	Serial.print(F(", actual = "));
	Serial.println(millis() - previousMillis);

	previousMillis = millis();
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting ISR_Timer_Complex on "));
	Serial.println(BOARD_NAME);
	Serial.println(TEENSY_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
	{
		lastMillis = millis();
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(lastMillis);
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));

	// Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
	// You can use up to 16 timer for each Teensy_ISR_Timer
	Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
	Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
	Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
	Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);

	// You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
	simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
	// This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
	// You see the time elapse of Teensy_ISR_Timer still accurate, whereas very unaccurate for Software Timer
	// The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
	// While that of Teensy_ISR_Timer is still prefect.
	delay(BLOCKING_TIME_MS);

	// You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
	// You don't need to and never call Teensy_ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
	simpleTimer.run();
}
