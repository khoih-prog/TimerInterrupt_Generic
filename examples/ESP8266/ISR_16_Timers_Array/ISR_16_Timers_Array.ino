/****************************************************************************************************************************
  ISR_16_Timers_Array.ino
  For ESP8266 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt_Generic
  Licensed under MIT license
  Licensed under MIT license

  The ESP8266 timers are badly designed, using only 23-bit counter along with maximum 256 prescaler. They're only better than UNO / Mega.
  The ESP8266 has two hardware timers, but timer0 has been used for WiFi and it's not advisable to use. Only timer1 is available.
  The timer1's 23-bit counter terribly can count only up to 8,388,607. So the timer1 maximum interval is very short.
  Using 256 prescaler, maximum timer1 interval is only 26.843542 seconds !!!

  Now with these new 16 ISR-based timers, the maximum interval is practically unlimited (limited only by unsigned long miliseconds)
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

#if !defined(ESP8266)
	#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         2
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "TimerInterrupt_Generic.h"
#include "ISR_Timer_Generic.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#ifndef LED_BUILTIN
	#define LED_BUILTIN       2
#endif

#ifndef LED_BLUE
	#define LED_BLUE          25
#endif

#ifndef LED_RED
	#define LED_RED           27
#endif

#define HW_TIMER_INTERVAL_MS      1L

volatile uint32_t startMillis = 0;

// Init ESP8266 timer 1
ESP8266Timer ITimer;

// Init BlynkTimer
ISR_Timer ESP8266_ISR_Timer;

#define LED_TOGGLE_INTERVAL_MS        2000L

void IRAM_ATTR TimerHandler()
{
	static bool toggle  = false;
	static bool started = false;
	static int timeRun  = 0;

	ESP8266_ISR_Timer.run();

	// Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
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

// In SAMD, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
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
	Serial.println(ARDUINO_BOARD);
	Serial.println(ESP8266_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
	{
		startMillis = millis();
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(startMillis);
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));

	// Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
	// You can use up to 16 timer for each ESP8266_ISR_Timer
	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
		ESP8266_ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
	}

	// You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
	simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
	// This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
	// You see the time elapse of ESP8266_ISR_Timer still accurate, whereas very unaccurate for Software Timer
	// The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
	// While that of ESP8266_ISR_Timer is still prefect.
	delay(BLOCKING_TIME_MS);

	// You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
	// You don't need to and never call ESP8266_ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
	simpleTimer.run();
}
