/****************************************************************************************************************************
  ISR_RPM_Measure.ino
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
/* RPM Measuring uses high frequency hardware timer 1Hz == 1ms) to measure the time from of one rotation, in ms
   then convert to RPM. One rotation is detected by reading the state of a magnetic REED SW or IR LED Sensor
   Asssuming LOW is active.
   For example: Max speed is 600RPM => 10 RPS => minimum 100ms a rotation. We'll use 80ms for debouncing
   If the time between active state is less than 8ms => consider noise.
   RPM = 60000 / (rotation time in ms)

   We use interrupt to detect whenever the SW is active, set a flag
   then use timer to count the time between active state
*/

// These define's must be placed at the beginning before #include "megaAVR_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

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

#if !defined(LED_BUILTIN)
	#define LED_BUILTIN     13
#endif

unsigned int interruptPin = 2;

#define TIMER1_INTERVAL_MS        1
#define DEBOUNCING_INTERVAL_MS    80

#define LOCAL_DEBUG      1

volatile unsigned long rotationTime = 0;
float RPM       = 0.00;
float avgRPM    = 0.00;

volatile int debounceCounter;

#define KAVG      100

volatile bool activeState = false;

void detectRotation(void)
{
	activeState = true;
}

void TimerHandler1()
{
	if ( activeState )
	{
		// Reset to prepare for next round of interrupt
		activeState = false;

		if (debounceCounter >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS )
		{

			//min time between pulses has passed
			RPM = (float) ( 60000.0f / ( rotationTime * TIMER1_INTERVAL_MS ) );

			avgRPM = ( 2 * avgRPM + RPM) / 3,

#if (TIMER_INTERRUPT_DEBUG > 1)
			Serial.print("RPM = ");
			Serial.print(avgRPM);
			Serial.print(", rotationTime ms = ");
			Serial.println(rotationTime * TIMER1_INTERVAL_MS);
#endif

			rotationTime = 0;
			debounceCounter = 0;
		}
		else
			debounceCounter++;
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

	Serial.print(F("\nStarting ISR_RPM_Measure on "));
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

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(interruptPin, INPUT_PULLUP);

	// Timer0 is used for micros(), millis(), delay(), etc and can't be used
	// Select Timer 1-2 for UNO, 0-5 for MEGA
	// Timer 2 is 8-bit timer, only for higher frequency

	ITimer1.init();

	// Using ATmega328 used in UNO => 16MHz CPU clock ,

	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
	{
		Serial.print(F("Starting  ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

	// Assumming the interruptPin will go LOW
	attachInterrupt(digitalPinToInterrupt(interruptPin), detectRotation, FALLING);
}

void loop()
{

}
