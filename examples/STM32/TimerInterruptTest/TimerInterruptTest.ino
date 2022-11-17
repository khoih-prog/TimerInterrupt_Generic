/****************************************************************************************************************************
  TimerInterruptTest.ino
  For STM32 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt_Generic
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one STM32 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
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

#if !( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) || defined(STM32L5) )
#error This code is designed to run on STM32F/L/H/G/WB/MP1 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "TimerInterrupt_Generic.h"

#ifndef LED_BUILTIN
	#define LED_BUILTIN       PB0               // Pin 33/PB0 control on-board LED_GREEN on F767ZI
#endif

#ifndef LED_BLUE
	#define LED_BLUE          PB7               // Pin 73/PB7 control on-board LED_BLUE on F767ZI
#endif

#ifndef LED_RED
	#define LED_RED           PB14              // Pin 74/PB14 control on-board LED_BLUE on F767ZI
#endif

unsigned int SWPin = D7;

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
	Serial.print("ITimer0 called, millis() = ");
	Serial.println(millis());
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
		pinMode(SWPin, OUTPUT);
	}

#if (TIMER_INTERRUPT_DEBUG > 0)
	Serial.print("ITimer1 called, millis() = ");
	Serial.println(millis());
#endif

	//timer interrupt toggles outputPin
	digitalWrite(LED_BLUE, toggle1);
	digitalWrite(SWPin, toggle1);
	toggle1 = !toggle1;
}

#define TIMER0_INTERVAL_MS        1000
#define TIMER0_DURATION_MS        5000

#define TIMER1_INTERVAL_MS        3000
#define TIMER1_DURATION_MS        15000

// Depending on the board, you can select STM32 Hardware Timer from TIM1-TIM22
// For example, F767ZI can select Timer from TIM1-TIM14
// If you select a Timer not correctly, you'll get a message from ci[ompiler
// 'TIMxx' was not declared in this scope; did you mean 'TIMyy'?

// Init STM32 timer TIM1
STM32Timer ITimer0(TIM1);
// Init STM32 timer TIM2
STM32Timer ITimer1(TIM2);

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting TimerInterruptTest on "));
	Serial.println(BOARD_NAME);
	Serial.println(STM32_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Using ESP32  => 80 / 160 / 240MHz CPU clock ,
	// For 64-bit timer counter
	// For 16-bit timer prescaler up to 1024

	// Interval in microsecs
	if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
	{
		Serial.print(F("Starting ITimer0 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

	// Interval in microsecs
	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
	{
		Serial.print(F("Starting  ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

	Serial.flush();
}

void loop()
{
	static unsigned long lastTimer0 = 0;
	static unsigned long lastTimer1 = 0;

	static bool timer0Stopped         = false;
	static bool timer1Stopped         = false;

	if (millis() - lastTimer0 > TIMER0_DURATION_MS)
	{
		lastTimer0 = millis();

		if (timer0Stopped)
		{
			Serial.print(F("Start ITimer0, millis() = "));
			Serial.println(millis());
			ITimer0.restartTimer();
		}
		else
		{
			Serial.print(F("Stop ITimer0, millis() = "));
			Serial.println(millis());
			ITimer0.stopTimer();
		}

		timer0Stopped = !timer0Stopped;
	}

	if (millis() - lastTimer1 > TIMER1_DURATION_MS)
	{
		lastTimer1 = millis();

		if (timer1Stopped)
		{
			Serial.print(F("Start ITimer1, millis() = "));
			Serial.println(millis());
			ITimer1.restartTimer();
		}
		else
		{
			Serial.print(F("Stop ITimer1, millis() = "));
			Serial.println(millis());
			ITimer1.stopTimer();
		}

		timer1Stopped = !timer1Stopped;
	}
}
