/****************************************************************************************************************************
  Change_Interval_HF.ino
  For Arduino megaAVR ATMEGA4809-based boards (UNO WiFi Rev2, NANO_EVERY, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/megaAVR_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
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

// These define's must be placed at the beginning before #include "megaAVR_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     3

// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_16MHZ     true
#define USING_8MHZ      false
#define USING_250KHZ    false

#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false

#include "TimerInterrupt_Generic.h"

#if !defined(LED_BUILTIN)
	#define LED_BUILTIN     13
#endif

#ifndef LED_BLUE
	#define LED_BLUE              7
#endif

#define TIMER1_FREQUENCY            1000UL

volatile uint32_t Timer1Count = 0;

void printResult(uint32_t currTime)
{
	Serial.print(F("Time = "));
	Serial.print(currTime);
	Serial.print(F(", Timer1Count = "));
	Serial.println(Timer1Count);
}

void TimerHandler1()
{
	Timer1Count++;
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting Change_Interval_HF on "));
	Serial.println(BOARD_NAME);
	Serial.println(MEGA_AVR_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	Serial.print(F("TCB Clock Frequency = "));

#if USING_16MHZ
	Serial.println(F("16MHz for highest accuracy"));
#elif USING_8MHZ
	Serial.println(F("8MHz for very high accuracy"));
#else
	Serial.println(F("250KHz for lower accuracy but longer time"));
#endif

	// Timer0 is used for micros(), millis(), delay(), etc and can't be used
	// Select Timer 1-2 for UNO, 0-5 for MEGA
	// Timer 2 is 8-bit timer, only for higher frequency

	ITimer1.init();

	// Using ATmega328 used in UNO => 16MHz CPU clock ,
	// For 16-bit timer 1, 3, 4 and 5, set frequency from 0.2385 to some KHz
	// For 8-bit timer 2 (prescaler up to 1024, set frequency from 61.5Hz to some KHz

	if (ITimer1.attachInterrupt(TIMER1_FREQUENCY, TimerHandler1))
	{
		Serial.print(F("Starting ITimer1 OK, millis() = "));
		Serial.println(millis());
		Serial.print(F("Frequency, Timer1 = "));
		Serial.println(TIMER1_FREQUENCY);
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
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

			// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
			// bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration)

			ITimer1.setFrequency(TIMER1_FREQUENCY / (multFactor + 1), TimerHandler1);

			Serial.print(F("Changing Frequency, Timer1 = "));
			Serial.println(TIMER1_FREQUENCY / (multFactor + 1));

			lastChangeTime = currTime;
		}
	}
}
