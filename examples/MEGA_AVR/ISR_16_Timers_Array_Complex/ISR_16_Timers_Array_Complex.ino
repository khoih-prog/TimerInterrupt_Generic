/****************************************************************************************************************************
  ISR_16_Timers_Array_Complex.ino
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
#define USE_TIMER_2     true
#define USE_TIMER_3     false

#include "TimerInterrupt_Generic.h"
#include "ISR_Timer_Generic.h"

#include <SimpleTimer.h>              // https://github.com/schinken/SimpleTimer

#ifndef LED_BUILTIN
	#define LED_BUILTIN       13
#endif

ISR_Timer ISR_Timer1;

#define LED_TOGGLE_INTERVAL_MS        1000L

// You have to use longer time here if having problem because Arduino AVR clock is low, 16MHz => lower accuracy.
// Tested OK with 1ms when not much load => higher accuracy.
#define TIMER1_INTERVAL_MS            5L

volatile uint32_t startMillis = 0;

void TimerHandler1()
{
	static bool toggle  = false;
	static int timeRun  = 0;

	ISR_Timer1.run();

	// Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
	if (++timeRun == ((LED_TOGGLE_INTERVAL_MS) / TIMER1_INTERVAL_MS) )
	{
		timeRun = 0;

		//timer interrupt toggles pin LED_BUILTIN
		digitalWrite(LED_BUILTIN, toggle);
		toggle = !toggle;
	}
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  (void);

/////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#if USE_COMPLEX_STRUCT

typedef struct
{
	irqCallback   irqCallbackFunc;
	uint32_t      TimerInterval;
	unsigned long deltaMillis;
	unsigned long previousMillis;
} ISRTimerData;

// In NRF52, avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

void doingSomething(int index);

#else

volatile unsigned long deltaMillis    [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile unsigned long previousMillis [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// You can assign any interval for any timer here, in milliseconds
uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
{
	5000L,  10000L,  15000L,  20000L,  25000L,  30000L,  35000L,  40000L,
	45000L, 50000L,  55000L,  60000L,  65000L,  70000L,  75000L,  80000L
};

void doingSomething(int index)
{
	unsigned long currentMillis  = millis();

	deltaMillis[index]    = currentMillis - previousMillis[index];
	previousMillis[index] = currentMillis;
}

#endif

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomething0()
{
	doingSomething(0);
}

void doingSomething1()
{
	doingSomething(1);
}

void doingSomething2()
{
	doingSomething(2);
}

void doingSomething3()
{
	doingSomething(3);
}

void doingSomething4()
{
	doingSomething(4);
}

void doingSomething5()
{
	doingSomething(5);
}

void doingSomething6()
{
	doingSomething(6);
}

void doingSomething7()
{
	doingSomething(7);
}

void doingSomething8()
{
	doingSomething(8);
}

void doingSomething9()
{
	doingSomething(9);
}

void doingSomething10()
{
	doingSomething(10);
}

void doingSomething11()
{
	doingSomething(11);
}

void doingSomething12()
{
	doingSomething(12);
}

void doingSomething13()
{
	doingSomething(13);
}

void doingSomething14()
{
	doingSomething(14);
}

void doingSomething15()
{
	doingSomething(15);
}

#if USE_COMPLEX_STRUCT

ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
{
	//irqCallbackFunc, TimerInterval, deltaMillis, previousMillis
	{ doingSomething0,    5000L, 0, 0 },
	{ doingSomething1,   10000L, 0, 0 },
	{ doingSomething2,   15000L, 0, 0 },
	{ doingSomething3,   20000L, 0, 0 },
	{ doingSomething4,   25000L, 0, 0 },
	{ doingSomething5,   30000L, 0, 0 },
	{ doingSomething6,   35000L, 0, 0 },
	{ doingSomething7,   40000L, 0, 0 },
	{ doingSomething8,   45000L, 0, 0 },
	{ doingSomething9,   50000L, 0, 0 },
	{ doingSomething10,  55000L, 0, 0 },
	{ doingSomething11,  60000L, 0, 0 },
	{ doingSomething12,  65000L, 0, 0 },
	{ doingSomething13,  70000L, 0, 0 },
	{ doingSomething14,  75000L, 0, 0 },
	{ doingSomething15,  80000L, 0, 0 }
};

void doingSomething(int index)
{
	unsigned long currentMillis  = millis();

	curISRTimerData[index].deltaMillis    = currentMillis - curISRTimerData[index].previousMillis;
	curISRTimerData[index].previousMillis = currentMillis;
}

#else

irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
{
	doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
	doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
	doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
	doingSomething12, doingSomething13, doingSomething14, doingSomething15
};

#endif

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

	unsigned long currMillis = millis();

	Serial.print(F("SimpleTimer : "));
	Serial.print(SIMPLE_TIMER_MS / 1000);
	Serial.print(F(", ms : "));
	Serial.print(currMillis);
	Serial.print(F(", Dms : "));
	Serial.println(currMillis - previousMillis);

	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
#if USE_COMPLEX_STRUCT
		Serial.print(F("Timer : "));
		Serial.print(i);
		Serial.print(F(", programmed : "));
		Serial.print(curISRTimerData[i].TimerInterval);
		Serial.print(F(", actual : "));
		Serial.println(curISRTimerData[i].deltaMillis);
#else
		Serial.print(F("Timer : "));
		Serial.print(i);
		Serial.print(F(", programmed : "));
		Serial.print(TimerInterval[i]);
		Serial.print(F(", actual : "));
		Serial.println(deltaMillis[i]);
#endif
	}

	previousMillis = currMillis;
}

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting ISR_16_Timers_Array_Complex on "));
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

	ITimer1.init();

	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
	{
		Serial.print(F("Starting  ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

	//ISR_Timer1.setInterval(2000L, doingSomething2s);
	//ISR_Timer1.setInterval(5000L, doingSomething5s);

	// Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
	// You can use up to 16 timer for each ISR_Timer
	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
#if USE_COMPLEX_STRUCT
		curISRTimerData[i].previousMillis = startMillis;
		ISR_Timer1.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
		previousMillis[i] = startMillis;
		ISR_Timer1.setInterval(TimerInterval[i], irqCallbackFunc[i]);
#endif
	}

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
