/****************************************************************************************************************************
  Argument_Complex.ino
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
#define USE_TIMER_2     false
#define USE_TIMER_3     false

#include "TimerInterrupt_Generic.h"

#if !defined(LED_BUILTIN)
	#define LED_BUILTIN     13
#endif

struct pinStruct
{
	unsigned int Pin1;
	unsigned int Pin2;
	unsigned int Pin3;
};

volatile pinStruct myOutputPins = { LED_BUILTIN, A0, A1 };

void TimerHandler1(unsigned int outputPinsAddress)
{
	static bool toggle1 = false;
	static bool started = false;

	if (!started)
	{
		started = true;
		pinMode(((pinStruct *) outputPinsAddress)->Pin1, OUTPUT);
		pinMode(((pinStruct *) outputPinsAddress)->Pin2, INPUT_PULLUP);
		pinMode(((pinStruct *) outputPinsAddress)->Pin3, INPUT_PULLUP);
	}

	//timer interrupt toggles pins
#if (TIMER_INTERRUPT_DEBUG > 1)
	Serial.print("Toggle pin1 = ");
	Serial.println( ((pinStruct *) outputPinsAddress)->Pin1 );
#endif

	digitalWrite(((pinStruct *) outputPinsAddress)->Pin1, toggle1);

#if (TIMER_INTERRUPT_DEBUG > 1)
	Serial.print("Read pin2 A0 (");
	Serial.print(((pinStruct *) outputPinsAddress)->Pin2 );
	Serial.print(") = ");
	Serial.println(digitalRead(((pinStruct *) outputPinsAddress)->Pin2) ? "HIGH" : "LOW" );

	Serial.print("Read pin3 A1 (");
	Serial.print(((pinStruct *) outputPinsAddress)->Pin3 );
	Serial.print(") = ");
	Serial.println(digitalRead(((pinStruct *) outputPinsAddress)->Pin3) ? "HIGH" : "LOW" );
#endif

	toggle1 = !toggle1;
}

#define TIMER1_INTERVAL_MS    5000

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting Argument_Complex on "));
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

	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, (unsigned int) &myOutputPins))
	{
		Serial.print(F("Starting  ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

void loop()
{
}
