/****************************************************************************************************************************
  SAMD21_MultiTimers.ino
  For SAMD boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/SAMD_TimerInterrupt
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one SAMD timer and avoid conflicting with other cores' tasks.
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

#if !( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E15A__) || defined(__SAMD21E16A__) || defined(__SAMD21E17A__) \
      || defined(__SAMD21E18A__) || defined(__SAMD21G15A__) || defined(__SAMD21G16A__) || defined(__SAMD21G17A__) || defined(__SAMD21G18A__) \
      || defined(__SAMD21J15A__) || defined(__SAMD21J16A__) || defined(__SAMD21J17A__) || defined(__SAMD21J18A__) )
#error This code is designed to run on SAMD21 platform! Please check your Tools->Board setting.
#endif

/////////////////////////////////////////////////////////////////

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Select only one to be true for SAMD21. Must must be placed at the beginning before #include "TimerInterrupt_Generic.h"
#define USING_TIMER_TC3         true      // Only TC3 can be used for SAMD51
#define USING_TIMER_TC4         true     // Not to use with Servo library
#define USING_TIMER_TC5         true
#define USING_TIMER_TCC         true
#define USING_TIMER_TCC1        true
#define USING_TIMER_TCC2        false     // Don't use this, can crash on some boards

// Uncomment To test if conflict with Servo library
//#include "Servo.h"

/////////////////////////////////////////////////////////////////

#include "TimerInterrupt_Generic.h"

// Depending on the board, you can select SAMD21 Hardware Timer from TC3, TC4, TC5, TCC, TCC1 or TCC2
// SAMD51 Hardware Timer only TC3

SAMDTimer ITimer0(TIMER_TC3);
SAMDTimer ITimer1(TIMER_TC4);
SAMDTimer ITimer2(TIMER_TC5);
SAMDTimer ITimer3(TIMER_TCC);
SAMDTimer ITimer4(TIMER_TCC1);

volatile uint32_t checkTimer[MAX_TIMER] = { 0, 0, 0, 0, 0 };

// TC3, TC4, TC5 max permissible TIMER_INTERVAL_MS is 1398.101 ms, larger will overflow, therefore not permitted
// Use TCC, TCC1, TCC2 for longer TIMER_INTERVAL_MS
uint32_t TIMER_INTERVAL_MS[MAX_TIMER]   = { 100, 200, 500, 1000, 2000 };

const char* TIMER_NAME[MAX_TIMER]       = { "TC3 ", "TC4 ", "TC5 ", "TCC ", "TCC1" };

void TimerHandler_TIMER_TC3()
{
	checkTimer[TIMER_TC3]++;
}

void TimerHandler_TIMER_TC4()
{
	checkTimer[TIMER_TC4]++;
}

void TimerHandler_TIMER_TC5()
{
	checkTimer[TIMER_TC5]++;
}

void TimerHandler_TIMER_TCC()
{
	checkTimer[TIMER_TCC]++;
}

void TimerHandler_TIMER_TCC1()
{
	checkTimer[TIMER_TCC1]++;
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	Serial.print(F("\nStarting SAMD21_MultiTimers on "));
	Serial.println(BOARD_NAME);
	Serial.println(SAMD_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

#if USING_TIMER_TC3

	if (ITimer0.attachInterruptInterval_MS(TIMER_INTERVAL_MS[TIMER_TC3], TimerHandler_TIMER_TC3))
		Serial.println("Starting  TIMER_TC3 OK, millis() = " + String(millis()));
	else
		Serial.println("Can't set TIMER_TC3. Select another freq. or timer");

#endif

#if USING_TIMER_TC4

	if (ITimer1.attachInterruptInterval_MS(TIMER_INTERVAL_MS[TIMER_TC4], TimerHandler_TIMER_TC4))
		Serial.println("Starting  TIMER_TC4 OK, millis() = " + String(millis()));
	else
		Serial.println("Can't set TIMER_TC4. Select another freq. or timer");

#endif

#if USING_TIMER_TC5

	if (ITimer2.attachInterruptInterval_MS(TIMER_INTERVAL_MS[TIMER_TC5], TimerHandler_TIMER_TC5))
		Serial.println("Starting  TIMER_TC5 OK, millis() = " + String(millis()));
	else
		Serial.println("Can't set TIMER_TC5. Select another freq. or timer");

#endif

#if USING_TIMER_TCC

	if (ITimer3.attachInterruptInterval_MS(TIMER_INTERVAL_MS[TIMER_TCC], TimerHandler_TIMER_TCC))
		Serial.println("Starting  TIMER_TCC OK, millis() = " + String(millis()));
	else
		Serial.println("Can't set TIMER_TCC. Select another freq. or timer");

#endif

#if USING_TIMER_TCC1

	if (ITimer4.attachInterruptInterval_MS(TIMER_INTERVAL_MS[TIMER_TCC1], TimerHandler_TIMER_TCC1))
		Serial.println("Starting  TIMER_TCC1 OK, millis() = " + String(millis()));
	else
		Serial.println("Can't set TIMER_TCC1. Select another freq. or timer");

#endif
}

#define DELAY_TIME_MS     60000

void loop()
{
	delay(DELAY_TIME_MS);

	Serial.println("========================================");

	for (uint8_t index = TIMER_TC3; index < MAX_TIMER; index++)
	{
		if (checkTimer[index] > 0)
		{
			// Be sure timer is enabled
			Serial.print(TIMER_NAME[index]);
			Serial.print(" Actual/Programmed (ms) ");
			Serial.print(DELAY_TIME_MS / checkTimer[index]);
			Serial.print("/");
			Serial.println(TIMER_INTERVAL_MS[index]);
			checkTimer[index] = 0;
		}
	}
}
