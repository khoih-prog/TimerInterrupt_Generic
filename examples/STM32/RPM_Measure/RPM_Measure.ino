/****************************************************************************************************************************
  RPM_Measure.ino
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

   RPM Measuring uses high frequency hardware timer 1Hz == 1ms) to measure the time from of one rotation, in ms
   then convert to RPM. One rotation is detected by reading the state of a magnetic REED SW or IR LED Sensor
   Asssuming LOW is active.
   For example: Max speed is 600RPM => 10 RPS => minimum 100ms a rotation. We'll use 80ms for debouncing
   If the time between active state is less than 8ms => consider noise.
   RPM = 60000 / (rotation time in ms)

   You can also use interrupt to detect whenever the SW is active, set a flag then use timer to count the time between active state
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

#define TIMER0_INTERVAL_MS        1
#define DEBOUNCING_INTERVAL_MS    80

#define LOCAL_DEBUG               1

// Depending on the board, you can select STM32 Hardware Timer from TIM1-TIM22
// For example, F767ZI can select Timer from TIM1-TIM14
// If you select a Timer not correctly, you'll get a message from ci[ompiler
// 'TIMxx' was not declared in this scope; did you mean 'TIMyy'?

// Init STM32 timer TIM1
STM32Timer ITimer(TIM1);

volatile unsigned long rotationTime = 0;
float RPM       = 0.00;
float avgRPM    = 0.00;

volatile int debounceCounter;

void TimerHandler()
{
	static bool started = false;

	if (!started)
	{
		started = true;
		pinMode(SWPin, INPUT_PULLUP);
	}

	if ( !digitalRead(SWPin) && (debounceCounter >= DEBOUNCING_INTERVAL_MS / TIMER0_INTERVAL_MS ) )
	{
		//min time between pulses has passed
		RPM = (float) ( 60000.0f / ( rotationTime * TIMER0_INTERVAL_MS ) );

		avgRPM = ( 2 * avgRPM + RPM) / 3,

#if (TIMER_INTERRUPT_DEBUG > 1)
		Serial.print("RPM = ");
		Serial.print(avgRPM);
		Serial.print(", rotationTime ms = ");
		Serial.println(rotationTime * TIMER0_INTERVAL_MS);
#endif

		rotationTime = 0;
		debounceCounter = 0;
	}
	else
	{
		debounceCounter++;
	}

	if (rotationTime >= 5000)
	{
		// If idle, set RPM to 0, don't increase rotationTime
		RPM = 0;

#if (TIMER_INTERRUPT_DEBUG > 1)
		Serial.print("RPM = ");
		Serial.print(RPM);
		Serial.print(", rotationTime = ");
		Serial.println(rotationTime);
#endif

		rotationTime = 0;
	}
	else
	{
		rotationTime++;
	}
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting RPM_Measure on "));
	Serial.println(BOARD_NAME);
	Serial.println(STM32_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler))
	{
		Serial.print(F("Starting  ITimer OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));

	Serial.flush();
}

void loop()
{

}
