/****************************************************************************************************************************
  FakeAnalogWrite.ino
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
#define _TIMERINTERRUPT_LOGLEVEL_     0

#define LOCAL_DEBUG         1

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

#ifndef LED_BUILTIN
	#define LED_BUILTIN       13
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

#define TIMER2_FREQUENCY_HZ     10000UL
#define TIMER2_INTERVAL_US      (1000000UL / TIMER2_FREQUENCY_HZ)

volatile uint32_t startMillis = 0;

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1,NRF_TIMER_3,NRF_TIMER_4 (1,3 and 4)
// If you select the already-used NRF_TIMER_0 or NRF_TIMER_2, it'll be auto modified to use NRF_TIMER_1

//ISR_Timer ISR_Timer2;

#define LED_TOGGLE_INTERVAL_MS        500L

#define NUMBER_ISR_TIMERS         16

void TimerHandler(void)
{
	static bool toggle  = false;
	static uint32_t timeRun  = 0;

	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
		doingSomething(i);
	}

	// Toggle LED every LED_TOGGLE_INTERVAL_MS = 500ms = 0.5s
	if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * TIMER2_FREQUENCY_HZ) / 1000) )
	{
		timeRun = 0;

		//timer interrupt toggles pin LED_BUILTIN
		digitalWrite(LED_BUILTIN, toggle);
		toggle = !toggle;
	}
}

/////////////////////////////////////////////////



typedef void (*irqCallback)  (void);

/////////////////////////////////////////////////


typedef struct
{
	int16_t       PWM_Value;          // Writing negative value to stop and free this PWM
	uint16_t      PWM_PremapValue;    // To detect if use the same PWM_Value setting => don't do anything
	uint16_t      pin;
	int16_t       countPWM;
	bool          beingUsed;
} ISRTimerData;

// Avoid doing something fancy in ISR, for example Serial.print()
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

	//if (ITimer1.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler))
	if (ITimer1.attachInterrupt(TIMER2_FREQUENCY_HZ, TimerHandler))
	{
		Serial.print(F("Starting  ITimer1 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

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

#define USING_MAPPING_TABLE   false

void fakeAnalogWrite(uint16_t pin, uint16_t value)
{
	uint16_t localValue;

#if USING_MAPPING_TABLE
	uint16_t localIndex = 0;
#endif

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

#if USING_MAPPING_TABLE

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
					                                            ( (localValue - mappingTable[localIndex]) * 10 ) /  (mappingTable[localIndex + 1] - mappingTable[localIndex]) );

#else
					curISRTimerData[i].PWM_Value = localValue;
#endif

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
				curISRTimerData[i].PWM_PremapValue = localValue;

#if USING_MAPPING_TABLE

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
				                                            ( (localValue - mappingTable[localIndex]) * 10 ) /  (mappingTable[localIndex + 1] - mappingTable[localIndex]) );
#else
				curISRTimerData[i].PWM_Value = localValue;
#endif
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
		// Change the pin according to board
		// UNI, Nano, etc can use pins from 2-12. Pin 13 is used for LED_BUILTIN
		// Mega can use many more pins, such as 22-53

#if (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__))
		fakeAnalogWrite(22, i * DIVIDER);
		fakeAnalogWrite(23, i * DIVIDER);
		fakeAnalogWrite(24, i * DIVIDER);
		fakeAnalogWrite(25, i * DIVIDER);
		fakeAnalogWrite(26, i * DIVIDER);
		fakeAnalogWrite(27, i * DIVIDER);
		fakeAnalogWrite(28, i * DIVIDER);
		fakeAnalogWrite(29, i * DIVIDER);
		fakeAnalogWrite(30, i * DIVIDER);
		fakeAnalogWrite(31, i * DIVIDER);
		fakeAnalogWrite(32, i * DIVIDER);
		fakeAnalogWrite(33, i * DIVIDER);
		fakeAnalogWrite(34, i * DIVIDER);
		fakeAnalogWrite(35, i * DIVIDER);
		fakeAnalogWrite(36, i * DIVIDER);
		fakeAnalogWrite(37, i * DIVIDER);
#else
		fakeAnalogWrite( 2, i * DIVIDER);
		fakeAnalogWrite( 3, i * DIVIDER);
		fakeAnalogWrite( 4, i * DIVIDER);
		fakeAnalogWrite( 5, i * DIVIDER);
		fakeAnalogWrite( 6, i * DIVIDER);
		fakeAnalogWrite( 7, i * DIVIDER);
		fakeAnalogWrite( 8, i * DIVIDER);
		fakeAnalogWrite( 9, i * DIVIDER);
		fakeAnalogWrite(10, i * DIVIDER);
		fakeAnalogWrite(11, i * DIVIDER);
		fakeAnalogWrite(12, i * DIVIDER);
		fakeAnalogWrite(A0, i * DIVIDER);
		fakeAnalogWrite(A1, i * DIVIDER);
		fakeAnalogWrite(A2, i * DIVIDER);
		fakeAnalogWrite(A3, i * DIVIDER);
		fakeAnalogWrite(A4, i * DIVIDER);
#endif

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
