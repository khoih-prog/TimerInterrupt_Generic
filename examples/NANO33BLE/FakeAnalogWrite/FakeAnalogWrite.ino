/****************************************************************************************************************************
  FakeAnalogWrite.ino
  For NRF52 boards using mbed-RTOS such as Nano-33-BLE
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt_Generic
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one NRF52 timer and avoid conflicting with other cores' tasks.
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

   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written
*/

#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
	#error This code is designed to run on nRF52-based Nano-33-BLE boards using mbed-RTOS platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// For Nano33-BLE, don't use Serial.print() in ISR as system will definitely hang.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#define LOCAL_DEBUG                   1

#include "TimerInterrupt_Generic.h"

#ifndef LED_BUILTIN
	#define LED_BUILTIN       D13
#endif

#ifndef LED_BLUE
	#define LED_BLUE          D7
#endif

#ifndef LED_RED
	#define LED_RED           D8
#endif

// For PWM_Value from 0-255.You can change to 1024 or 2048
#define MAX_PWM_VALUE         256

#define MAPPING_TABLE_SIZE    (MAX_PWM_VALUE / 10) + 1

// You have to calibrate and update this mapping table
float mappingTable[MAPPING_TABLE_SIZE] =
{
	0.0,     3.281,   6.860,  10.886,  15.285,  20.355,  26.096,  32.732,  40.785,  50.180,
	62.557,  79.557, 104.461, 136.075, 163.066, 181.930, 195.724, 207.132, 216.228, 223.684,
	230.395, 236.136, 241.206, 245.680, 249.781, 253.509
};

#define HW_TIMER_INTERVAL_US      100L

volatile uint32_t startMillis = 0;

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1,NRF_TIMER_3,NRF_TIMER_4 (1,3 and 4)
// If you select the already-used NRF_TIMER_0 or NRF_TIMER_2, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER3
NRF52_MBED_Timer ITimer(NRF_TIMER_3);

#define LED_TOGGLE_INTERVAL_MS        500L

#define NUMBER_ISR_TIMERS         16

void TimerHandler()
{
	static bool toggle  = false;
	static int timeRun  = 0;

	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
		doingSomething(i);
	}

	// Toggle LED every LED_TOGGLE_INTERVAL_MS = 500ms = 0.5s
	if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * 1000) / HW_TIMER_INTERVAL_US) )
	{
		timeRun = 0;

		//timer interrupt toggles pin LED_BUILTIN
		digitalWrite(LED_BUILTIN, toggle);
		toggle = !toggle;
	}
}

/////////////////////////////////////////////////

typedef void (*irqCallback)  ();

/////////////////////////////////////////////////


typedef struct
{
	int16_t       PWM_Value;          // Writing negative value to stop and free this PWM
	uint16_t      PWM_PremapValue;    // To detect if use the same PWM_Value setting => don't do anything
	uint16_t      pin;
	int16_t       countPWM;
	bool          beingUsed;
} ISRTimerData;

// In NRF52, avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

///////////////////////////////////

volatile ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
{
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
	{ 0, 0, 0, 0, false },
};

void doingSomething(int index)
{
	// First check if enabled and pin != 0
	if ( (curISRTimerData[index].beingUsed) && (curISRTimerData[index].PWM_Value > 0) && (curISRTimerData[index].pin != 0) )
	{
		// Divide the time into MAX_PWM_VALUE slots.
		// PWM_Value = 0 => no digitalWrite(pin, 1)
		// PWM_Value > 0 => digitalWrite(pin, 1) from countPWM = 0 to countPWM = PWM_Value

		if (curISRTimerData[index].countPWM == 0)
		{
			if (curISRTimerData[index].PWM_Value > 0)
				digitalWrite(curISRTimerData[index].pin, 1);
			else
				digitalWrite(curISRTimerData[index].pin, 0);
		}
		else if (curISRTimerData[index].countPWM == curISRTimerData[index].PWM_Value)
		{
			digitalWrite(curISRTimerData[index].pin, 0);
		}
	}

	curISRTimerData[index].countPWM = (curISRTimerData[index].countPWM + 1) % MAX_PWM_VALUE;
}

///////////////////////////////////////////


void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting FakeAnalogWrite on "));
	Serial.println(BOARD_NAME);
	Serial.println(NRF52_MBED_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);

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
	// You can use up to 16 timer for each ISR_Timer
	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
		curISRTimerData[i].beingUsed      = false;
		curISRTimerData[i].pin            = 0;
		curISRTimerData[i].PWM_Value      = 0;

		//ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
	}
}

void fakeAnalogWrite(uint16_t pin, uint16_t value)
{
	uint16_t localValue;
	uint16_t localIndex = 0;

	// First check if already got that pin, then just update the PWM_Value
	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
		if ( (curISRTimerData[i].beingUsed) && (curISRTimerData[i].pin == pin) )
		{
			localValue = (value < MAX_PWM_VALUE) ? value : MAX_PWM_VALUE;

			if (curISRTimerData[i].PWM_PremapValue == localValue)
			{
#if (LOCAL_DEBUG > 0)
				Serial.print(F("Ignore : Same Value for index = "));
				Serial.println(i);
#endif

				return;
			}
			else if (curISRTimerData[i].PWM_Value >= 0)
			{
				curISRTimerData[i].PWM_PremapValue = localValue;

				// Mapping to corect value
				if ( ( localValue == 0) || ( localValue == MAX_PWM_VALUE - 1) )
				{
					// Keep MAX_PWM_VALUE
					curISRTimerData[i].PWM_Value = localValue;
				}
				else
				{
					// Get the mapping index
					for (uint16_t j = 0; j < MAPPING_TABLE_SIZE; j++)
					{
						if ( (float) localValue < mappingTable[j])
						{
							localIndex = j - 1;
							break;
						}
					}

#if (LOCAL_DEBUG > 1)
					Serial.print(F("localIndex = "));
					Serial.println(localIndex);
#endif

					// Can use map() function
					// Can use map() function
					curISRTimerData[i].PWM_Value = (uint16_t) ( (localIndex * 10 ) +
					                                            ( (value - mappingTable[localIndex]) * 10 ) /  (mappingTable[localIndex + 1] - mappingTable[localIndex]) );

#if (LOCAL_DEBUG > 0)
					Serial.print(F("Update index = "));
					Serial.print(i);
					Serial.print(F(", pin = "));
					Serial.print(pin);
					Serial.print(F(", input PWM_Value = "));
					Serial.print(value);
					Serial.print(F(", mapped PWM_Value = "));
					Serial.println(curISRTimerData[i].PWM_Value);
#endif
				}
			}
			else
			{
				curISRTimerData[i].beingUsed      = false;
				curISRTimerData[i].pin            = 0;
				curISRTimerData[i].PWM_Value      = 0;
			}

			// Reset countPWM
			curISRTimerData[i].countPWM = 0;

			return;
		}
	}

	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
		if (!curISRTimerData[i].beingUsed)
		{
			curISRTimerData[i].beingUsed    = true;
			curISRTimerData[i].pin          = pin;

			// Mapping to corect value
			localValue = (value < MAX_PWM_VALUE) ? value : MAX_PWM_VALUE;

			if ( ( localValue == 0) || ( localValue == MAX_PWM_VALUE - 1) )
			{
				// Keep MAX_PWM_VALUE
				curISRTimerData[i].PWM_Value = localValue;
			}
			else
			{
				// Get the mapping index
				for (uint16_t j = 0; j < MAPPING_TABLE_SIZE; j++)
				{
					if ( (float) localValue < mappingTable[j])
					{
						localIndex = j - 1;
						break;
					}
				}

#if (LOCAL_DEBUG > 1)
				Serial.print(F("localIndex = "));
				Serial.println(localIndex);
#endif

				// Can use map() function
				// Can use map() function
				curISRTimerData[i].PWM_Value = (uint16_t) ( (localIndex * 10 ) +
				                                            ( (value - mappingTable[localIndex]) * 10 ) /  (mappingTable[localIndex + 1] - mappingTable[localIndex]) );
			}

			curISRTimerData[i].countPWM     = 0;

			pinMode(pin, OUTPUT);

#if (LOCAL_DEBUG > 0)
			Serial.print(F("Add index = "));
			Serial.print(i);
			Serial.print(F(", pin = "));
			Serial.print(pin);
			Serial.print(F(", input PWM_Value = "));
			Serial.print(value);
			Serial.print(F(", mapped PWM_Value = "));
			Serial.println(curISRTimerData[i].PWM_Value);
#endif

			return;
		}
	}
}

#define DELAY_BETWEEN_CHANGE_MS     5000L
#define REPEAT_INTERVAL_MS          10000L

#define DIVIDER                     5

void loop()
{
	for (uint16_t i = 0; i <= MAX_PWM_VALUE / DIVIDER; i++)
	{
		fakeAnalogWrite(D2, i * DIVIDER);
		fakeAnalogWrite(D3, i * DIVIDER);
		fakeAnalogWrite(D4, i * DIVIDER);
		fakeAnalogWrite(D5, i * DIVIDER);
		fakeAnalogWrite(D6, i * DIVIDER);
		fakeAnalogWrite(D7, i * DIVIDER);
		fakeAnalogWrite(D8, i * DIVIDER);
		fakeAnalogWrite(D9, i * DIVIDER);

#if (LOCAL_DEBUG > 0)
		Serial.print(F("Test PWM_Value = "));
		Serial.print(i * DIVIDER);
		Serial.print(F(", max = "));
		Serial.println(MAX_PWM_VALUE - 1);
#endif

		delay(DELAY_BETWEEN_CHANGE_MS);
	}

	Serial.println(F("==================="));
	delay(REPEAT_INTERVAL_MS);
}
