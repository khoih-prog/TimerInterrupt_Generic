/****************************************************************************************************************************
  SwitchDebounce.ino
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
/****************************************************************************************************************************
   Switch Debouncing uses high frequency hardware timer 50Hz == 20ms) to measure the time from the SW is pressed,
   debouncing time is 100ms => SW is considered pressed if timer count is > 5, then call / flag SW is pressed
   When the SW is released, timer will count (debounce) until more than 50ms until consider SW is released.
   We can set to flag or call a function whenever SW is pressed more than certain predetermined time, even before
   SW is released.
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
#define USE_TIMER_2     true
#define USE_TIMER_3     false

#include "TimerInterrupt_Generic.h"

unsigned int SWPin = A0;

#define TIMER1_INTERVAL_MS        20
#define DEBOUNCING_INTERVAL_MS    100
#define LONG_PRESS_INTERVAL_MS    5000

volatile bool SWPressed     = false;
volatile bool SWLongPressed = false;

void TimerHandler1()
{
	static unsigned int debounceCountSWPressed  = 0;
	static unsigned int debounceCountSWReleased = 0;

#if (TIMER_INTERRUPT_DEBUG > 1)
	static unsigned long SWPressedTime;
	static unsigned long SWReleasedTime;

	unsigned long currentMillis = millis();
#endif

	static bool started = false;

	if (!started)
	{
		started = true;
		pinMode(SWPin, INPUT_PULLUP);
	}

	if ( (!digitalRead(SWPin)) )
	{
		// Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
		debounceCountSWReleased = 0;

		if (++debounceCountSWPressed >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS)
		{
			// Call and flag SWPressed
			if (!SWPressed)
			{
#if (TIMER_INTERRUPT_DEBUG > 1)
				SWPressedTime = currentMillis;

				Serial.print("SW Press, from millis() = ");
				Serial.println(SWPressedTime);
#endif

				SWPressed = true;
				// Do something for SWPressed here in ISR
				// But it's better to use outside software timer to do your job instead of inside ISR
				//Your_Response_To_Press();
			}

			if (debounceCountSWPressed >= LONG_PRESS_INTERVAL_MS / TIMER1_INTERVAL_MS)
			{
				// Call and flag SWLongPressed
				if (!SWLongPressed)
				{
#if (TIMER_INTERRUPT_DEBUG > 1)
					Serial.print("SW Long Pressed, total time ms = ");
					Serial.print(currentMillis);
					Serial.print(" - ");
					Serial.print(SWPressedTime);
					Serial.print(" = ");
					Serial.println(currentMillis - SWPressedTime);
#endif

					SWLongPressed = true;
					// Do something for SWLongPressed here in ISR
					// But it's better to use outside software timer to do your job instead of inside ISR
					//Your_Response_To_Long_Press();
				}
			}
		}
	}
	else
	{
		// Start debouncing counting debounceCountSWReleased and clear debounceCountSWPressed
		if ( SWPressed && (++debounceCountSWReleased >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS))
		{
#if (TIMER_INTERRUPT_DEBUG > 1)
			SWReleasedTime = currentMillis;

			// Call and flag SWPressed
			Serial.print("SW Released, from millis() = ");
			Serial.println(SWReleasedTime);
#endif

			SWPressed     = false;
			SWLongPressed = false;

			// Do something for !SWPressed here in ISR
			// But it's better to use outside software timer to do your job instead of inside ISR
			//Your_Response_To_Release();

			// Call and flag SWPressed
#if (TIMER_INTERRUPT_DEBUG > 1)
			Serial.print("SW Pressed total time ms = ");
			Serial.println(SWReleasedTime - SWPressedTime);
#endif

			debounceCountSWPressed = 0;
		}
	}
}


void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting SwitchDebounce on "));
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

	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
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
