/****************************************************************************************************************************
  TimerInterruptLEDDemo.ino
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
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD51__) || defined(__SAMD51J20A__) \
      || defined(__SAMD51J19A__) || defined(__SAMD51G19A__) || defined(__SAMD51P19A__)  \
      || defined(__SAMD21E15A__) || defined(__SAMD21E16A__) || defined(__SAMD21E17A__) || defined(__SAMD21E18A__) \
      || defined(__SAMD21G15A__) || defined(__SAMD21G16A__) || defined(__SAMD21G17A__) || defined(__SAMD21G18A__) \
      || defined(__SAMD21J15A__) || defined(__SAMD21J16A__) || defined(__SAMD21J17A__) || defined(__SAMD21J18A__) )
#error This code is designed to run on SAMD21/SAMD51 platform! Please check your Tools->Board setting.
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
#define USING_TIMER_TC4         false     // Not to use with Servo library
#define USING_TIMER_TC5         false
#define USING_TIMER_TCC         false
#define USING_TIMER_TCC1        false
#define USING_TIMER_TCC2        false     // Don't use this, can crash on some boards

// Uncomment To test if conflict with Servo library
//#include "Servo.h"

/////////////////////////////////////////////////////////////////

//#ifndef LED_BUILTIN
//  #define LED_BUILTIN       13
//#endif

#ifndef LED_BLUE
	#define LED_BLUE          2
#endif

#ifndef LED_RED
	#define LED_RED           8
#endif

#include "TimerInterrupt_Generic.h"
#include "SAMD_ISR_Timer.h"

// TC3, TC4, TC5 max permissible HW_TIMER_INTERVAL_MS is 1398.101 ms, larger will overflow, therefore not permitted
// Use TCC, TCC1, TCC2 for longer HW_TIMER_INTERVAL_MS
#define HW_TIMER_INTERVAL_MS      10

///////////////////////////////////////////////

#if (TIMER_INTERRUPT_USING_SAMD21)

	#if USING_TIMER_TC3
		#define SELECTED_TIMER      TIMER_TC3
	#elif USING_TIMER_TC4
		#define SELECTED_TIMER      TIMER_TC4
	#elif USING_TIMER_TC5
		#define SELECTED_TIMER      TIMER_TC5
	#elif USING_TIMER_TCC
		#define SELECTED_TIMER      TIMER_TCC
	#elif USING_TIMER_TCC1
		#define SELECTED_TIMER      TIMER_TCC1
	#elif USING_TIMER_TCC2
		#define SELECTED_TIMER      TIMER_TCC
	#else
		#error You have to select 1 Timer
	#endif

#else

	#if !(USING_TIMER_TC3)
		#error You must select TC3 for SAMD51
	#endif

	#define SELECTED_TIMER      TIMER_TC3

#endif

// Init selected SAMD timer
SAMDTimer ITimer(SELECTED_TIMER);

////////////////////////////////////////////////

// Init SAMD_ISR_Timer
// Each SAMD_ISR_Timer can service 16 different ISR-based timers
SAMD_ISR_Timer ISR_Timer;

#define TIMER_INTERVAL_1S             1000L
#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L

void TimerHandler(void)
{
	ISR_Timer.run();
}

// In SAMD, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething1()
{
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	Serial.println(F("G"));
}

void doingSomething2()
{
	digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
	Serial.println(F("B"));
}
void doingSomething3()
{
	digitalWrite(LED_RED, !digitalRead(LED_RED));
	Serial.println(F("R"));
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	delay(100);

	Serial.print(F("\nStarting TimerInterruptLEDDemo on "));
	Serial.println(BOARD_NAME);
	Serial.println(SAMD_TIMER_INTERRUPT_VERSION);
	Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
	//HardwareTimer *MyTim = new HardwareTimer(Instance);

	// configure pin in output mode
	pinMode(LED_BUILTIN,  OUTPUT);
	pinMode(LED_BLUE,     OUTPUT);
	pinMode(LED_RED,      OUTPUT);

	// Interval in millisecs
	if (ITimer.attachInterruptInterval_MS(HW_TIMER_INTERVAL_MS, TimerHandler))
	{
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));

	// Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
	// You can use up to 16 timer for each ISR_Timer
	ISR_Timer.setInterval(TIMER_INTERVAL_1S,  doingSomething1);
	ISR_Timer.setInterval(TIMER_INTERVAL_2S,  doingSomething2);
	ISR_Timer.setInterval(TIMER_INTERVAL_5S,  doingSomething3);
}


void loop()
{
	/* Nothing to do all is done by hardware. Even no interrupt required. */
}
