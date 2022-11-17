/****************************************************************************************************************************
  ISR_Timers_Array_Simple.ino

  For RP2040-based boards such as RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040.
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

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     1

#include "TimerInterrupt_Generic.h"
#include "ISR_Timer_Generic.h"

#include <SimpleTimer.h>              // https://github.com/schinken/SimpleTimer

// Init RPI_PICO_Timer
RPI_PICO_Timer ITimer1(1);

ISR_Timer RPI_PICO_ISR_Timer;

#ifndef LED_BUILTIN
	#define LED_BUILTIN       25
#endif

#define LED_TOGGLE_INTERVAL_MS        1000L

// You have to use longer time here if having problem because Arduino AVR clock is low, 16MHz => lower accuracy.
// Tested OK with 1ms when not much load => higher accuracy.
#define TIMER_INTERVAL_MS            1L

volatile uint32_t startMillis = 0;

volatile uint32_t deltaMillis2s = 0;
volatile uint32_t deltaMillis5s = 0;

volatile uint32_t previousMillis2s = 0;
volatile uint32_t previousMillis5s = 0;


bool TimerHandler(struct repeating_timer *t)
{
	(void) t;

	static bool toggle  = false;
	static int timeRun  = 0;

	RPI_PICO_ISR_Timer.run();

	// Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
	if (++timeRun == ((LED_TOGGLE_INTERVAL_MS) / TIMER_INTERVAL_MS) )
	{
		timeRun = 0;

		//timer interrupt toggles pin LED_BUILTIN
		digitalWrite(LED_BUILTIN, toggle);
		toggle = !toggle;
	}

	return true;
}

void doingSomething2s()
{
	unsigned long currentMillis  = millis();

	deltaMillis2s    = currentMillis - previousMillis2s;
	previousMillis2s = currentMillis;
}

void doingSomething5s()
{
	unsigned long currentMillis  = millis();

	deltaMillis5s    = currentMillis - previousMillis5s;
	previousMillis5s = currentMillis;
}

/////////////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
	static unsigned long previousMillis = startMillis;

	unsigned long currMillis = millis();

	Serial.print(F("SimpleTimer : programmed "));
	Serial.print(SIMPLE_TIMER_MS);
	Serial.print(F("ms, current time ms : "));
	Serial.print(currMillis);
	Serial.print(F(", Delta ms : "));
	Serial.println(currMillis - previousMillis);

	Serial.print(F("Timer2s actual : "));
	Serial.println(deltaMillis2s);
	Serial.print(F("Timer5s actual : "));
	Serial.println(deltaMillis5s);

	previousMillis = currMillis;
}

////////////////////////////////////////////////

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting ISR_Timers_Array_Simple on "));
	Serial.println(BOARD_NAME);
	Serial.println(RPI_PICO_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	if (ITimer1.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler))
	{
		Serial.print(F("Starting ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

	previousMillis5s = previousMillis2s = millis();

	RPI_PICO_ISR_Timer.setInterval(2000L, doingSomething2s);
	RPI_PICO_ISR_Timer.setInterval(5000L, doingSomething5s);

	// You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
	simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
	// This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
	// You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
	// The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
	// While that of ISR_Timer is still prefect.
	delay(BLOCKING_TIME_MS);

	// You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
	// You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
	simpleTimer.run();
}
