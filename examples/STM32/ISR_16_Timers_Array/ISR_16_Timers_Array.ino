/****************************************************************************************************************************
  ISR_16_Timers_Array.ino
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

#define HW_TIMER_INTERVAL_US      100L

volatile uint32_t startMillis = 0;

// Depending on the board, you can select STM32 Hardware Timer from TIM1-TIM22
// For example, F767ZI can select Timer from TIM1-TIM14
// If you select a Timer not correctly, you'll get a message from ci[ompiler
// 'TIMxx' was not declared in this scope; did you mean 'TIMyy'?

// Init STM32 timer TIM1
STM32Timer ITimer(TIM1);

// Init STM32_ISR_Timer
// Each STM32_ISR_Timer can service 16 different ISR-based timers
ISR_Timer STM32_ISR_Timer;

#define LED_TOGGLE_INTERVAL_MS        2000L

void TimerHandler()
{
	static bool toggle  = false;
	static bool started = false;
	static int timeRun  = 0;

	STM32_ISR_Timer.run();

	// Toggle LED every LED_TOGGLE_INTERVAL_MS = 5000ms = 5s
	if (++timeRun == ( (LED_TOGGLE_INTERVAL_MS * 1000 ) / HW_TIMER_INTERVAL_US) )
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

#define NUMBER_ISR_TIMERS         16

// You can assign any interval for any timer here, in milliseconds
uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
{
	1000L,  2000L,  3000L,  4000L,  5000L,  6000L,  7000L,  8000L,
	9000L, 10000L, 11000L, 12000L, 13000L, 14000L, 15000L, 16000L
};

typedef void (*irqCallback)  ();

#if (TIMER_INTERRUPT_DEBUG > 0)
void printStatus(uint16_t index, unsigned long deltaMillis, unsigned long currentMillis)
{
	Serial.print(TimerInterval[index] / 1000);
	Serial.print("s: Delta ms = ");
	Serial.print(deltaMillis);
	Serial.print(", ms = ");
	Serial.println(currentMillis);
}
#endif

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething0()
{
#if (TIMER_INTERRUPT_DEBUG > 0)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(0, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething1()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(1, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething2()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(2, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething3()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(3, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething4()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(4, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething5()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(5, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething6()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(6, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething7()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(7, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething8()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(8, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething9()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(9, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething10()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(10, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething11()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(11, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething12()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(12, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething13()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(13, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething14()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(14, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

void doingSomething15()
{
#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long previousMillis = startMillis;

	unsigned long currentMillis = millis();
	unsigned long deltaMillis   = currentMillis - previousMillis;

	printStatus(15, deltaMillis, currentMillis);

	previousMillis = currentMillis;
#endif
}

irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
{
	doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
	doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
	doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
	doingSomething12, doingSomething13, doingSomething14, doingSomething15
};

////////////////////////////////////////////////


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

	Serial.print(F("simpleTimerDoingSomething2s: Delta programmed ms = "));
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

	Serial.print(F("\nStarting ISR_16_Timers_Array on "));
	Serial.println(BOARD_NAME);
	Serial.println(STM32_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
	{
		startMillis = millis();
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(startMillis);
	}
	else
		Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));

	// Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
	// You can use up to 16 timer for each STM32_ISR_Timer
	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
		STM32_ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
	}

	// You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
	simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
	// This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
	// You see the time elapse of STM32_ISR_Timer still accurate, whereas very unaccurate for Software Timer
	// The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
	// While that of STM32_ISR_Timer is still prefect.
	delay(BLOCKING_TIME_MS);

	// You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
	// You don't need to and never call STM32_ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
	simpleTimer.run();
}
