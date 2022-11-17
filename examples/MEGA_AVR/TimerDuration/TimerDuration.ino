/****************************************************************************************************************************
  TimerDuration.ino
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
#define TIMER_INTERRUPT_DEBUG         4
#define _TIMERINTERRUPT_LOGLEVEL_     4

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

void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
	static bool toggle1 = false;
	static bool started = false;

	if (!started)
	{
		started = true;
		pinMode(outputPin, OUTPUT);
	}

#if (TIMER_INTERRUPT_DEBUG > 1)
	Serial.print("ITimer1 called, millis() = ");
	Serial.println(millis());
#endif

	//timer interrupt toggles pin LED_BUILTIN
	digitalWrite(outputPin, toggle1);
	toggle1 = !toggle1;
}

#if !( TIMER_INTERRUPT_USING_ATMEGA_32U4 )

void TimerHandler2(unsigned int outputPin = LED_BUILTIN)
{
	static bool toggle2 = false;
	static bool started = false;

	if (!started)
	{
		started = true;
		pinMode(outputPin, OUTPUT);
	}

#if (TIMER_INTERRUPT_DEBUG > 1)
	Serial.print("ITimer2 called, millis() = ");
	Serial.println(millis());
#endif

	//timer interrupt toggles outputPin
	digitalWrite(outputPin, toggle2);
	toggle2 = !toggle2;
}
#endif

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin2 = A0;

#define TIMER1_INTERVAL_MS    1000
#define TIMER1_FREQUENCY      (float) (1000.0f / TIMER1_INTERVAL_MS)
#define TIMER1_DURATION_MS    (10 * TIMER1_INTERVAL_MS)

#define TIMER2_INTERVAL_MS    1300
#define TIMER2_FREQUENCY      (float) (1000.0f / TIMER2_INTERVAL_MS)
#define TIMER2_DURATION_MS    (20 * TIMER2_INTERVAL_MS)

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting TimerDuration on "));
	Serial.println(BOARD_NAME);
	Serial.println(MEGA_AVR_TIMER_INTERRUPT_VERSION);
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

	//if (ITimer1.attachInterrupt(TIMER1_FREQUENCY, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
	{
		Serial.print(F("Starting  ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

#if !( TIMER_INTERRUPT_USING_ATMEGA_32U4 )

	ITimer2.init();

	//if (ITimer2.attachInterrupt(TIMER2_FREQUENCY, TimerHandler2, outputPin2, TIMER2_DURATION_MS))
	if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2, outputPin2, TIMER2_DURATION_MS))
	{
		Serial.print(F("Starting  ITimer2 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer2. Select another freq. or timer"));

#endif
}

void loop()
{
}
