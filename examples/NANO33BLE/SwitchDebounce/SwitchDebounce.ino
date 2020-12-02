/****************************************************************************************************************************
   SwitchDebounce.ino
   For NRF52 boards using mbed-RTOS such as Nano-33-BLE
   Written by Khoi Hoang

   Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
   unsigned long miliseconds), you just consume only one NRF52 timer and avoid conflicting with other cores' tasks.
   The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
   Therefore, their executions are not blocked by bad-behaving functions / tasks.
   This important feature is absolutely necessary for mission-critical tasks.

   Based on SimpleTimer - A timer library for Arduino.
   Author: mromani@ottotecnica.com
   Copyright (c) 2010 OTTOTECNICA Italy

   Based on BlynkTimer.h
   Author: Volodymyr Shymanskyy

   Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt_Generic
   Licensed under MIT license

   Version: 1.3.0

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
   1.1.0   K Hoang      10/11/2020 Initial Super-Library coding to merge all TimerInterrupt Libraries
   1.2.0   K Hoang      12/11/2020 Add STM32_TimerInterrupt Library
   1.3.0   K Hoang      01/12/2020 Add Mbed Nano-33-BLE Library. Add support to AVR UNO, Nano, Arduino Mini, Ethernet, BT. etc.
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

   Switch Debouncing uses high frequency hardware timer 50Hz == 20ms) to measure the time from the SW is pressed,
   debouncing time is 100ms => SW is considered pressed if timer count is > 5, then call / flag SW is pressed
   When the SW is released, timer will count (debounce) until more than 50ms until consider SW is released.
   We can set to flag or call a function whenever SW is pressed more than certain predetermined time, even before
   SW is released.
*/

#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #error This code is designed to run on nRF52-based Nano-33-BLE boards using mbed-RTOS platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "NRF52_MBED_TimerInterrupt.h"
// For Nano33-BLE, don't use Serial.print() in ISR as system will definitely hang.
#define TIMER_INTERRUPT_DEBUG      1

#include "TimerInterrupt_Generic.h"

//#ifndef LED_BUILTIN
//  #define LED_BUILTIN         D13
//#endif

#ifndef LED_BLUE_PIN
  #define LED_BLUE_PIN          D7
#endif

unsigned int SWPin = D2;

#define TIMER_INTERVAL_MS         10
#define DEBOUNCING_INTERVAL_MS    100
#define LONG_PRESS_INTERVAL_MS    5000

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1,NRF_TIMER_3,NRF_TIMER_4 (1,3 and 4)
// If you select the already-used NRF_TIMER_0 or NRF_TIMER_2, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER3
NRF52_MBED_Timer ITimer(NRF_TIMER_3);

volatile bool SWPressed     = false;
volatile bool SWLongPressed = false;

unsigned int debounceCountSWPressed  = 0;
unsigned int debounceCountSWReleased = 0;
bool toggle0 = false;
bool toggle1 = false;
  
void TimerHandler(void)
{
  if ( (!digitalRead(SWPin)) )
  {
    // Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
    debounceCountSWReleased = 0;

    if (++debounceCountSWPressed >= DEBOUNCING_INTERVAL_MS / TIMER_INTERVAL_MS)
    {
      // Call and flag SWPressed
      if (!SWPressed)
      {
        SWPressed = true;
        // Do something for SWPressed here in ISR
        // But it's better to use outside software timer to do your job instead of inside ISR
        //Your_Response_To_Press();
        digitalWrite(LED_BUILTIN, toggle0);
        toggle0 = !toggle0;
      }

      if (debounceCountSWPressed >= LONG_PRESS_INTERVAL_MS / TIMER_INTERVAL_MS)
      {
        // Call and flag SWLongPressed
        if (!SWLongPressed)
        {
          SWLongPressed = true;
          // Do something for SWLongPressed here in ISR
          // But it's better to use outside software timer to do your job instead of inside ISR
          //Your_Response_To_Long_Press();
          digitalWrite(LED_BLUE_PIN, toggle1);
          toggle1 = !toggle1;
        }
      }
    }
  }
  else
  {
    // Start debouncing counting debounceCountSWReleased and clear debounceCountSWPressed
    if ( SWPressed && (++debounceCountSWReleased >= DEBOUNCING_INTERVAL_MS / TIMER_INTERVAL_MS))
    {
      // Call and flag SWPressed

      SWPressed     = false;
      SWLongPressed = false;

      // Do something for !SWPressed here in ISR
      // But it's better to use outside software timer to do your job instead of inside ISR
      //Your_Response_To_Release();

      // Call and flag SWPressed
      debounceCountSWPressed = 0;
    }
  }
}

void setup()
{
  pinMode(SWPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  delay(100);

  Serial.printf("\nStarting SwitchDebounce on %s\n", BOARD_NAME);
  Serial.printf("Version : v%s\n", TIMER_INTERRUPT_GENERIC_VERSION);
 
  // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler))
    Serial.printf("Starting  ITimer OK, millis() = %ld\n", millis());
  else
    Serial.println("Can't set ITimer. Select another freq., duration or timer");
}

void printResult(uint32_t currTime)
{
  Serial.printf("Time = %ld, Switch = %s\n", currTime, SWLongPressed? "LongPressed" : (SWPressed? "Pressed" : "Released") );
}

#define CHECK_INTERVAL_MS     1000L

void loop()
{
  static uint32_t lastTime = 0;
  static uint32_t currTime;

  currTime = millis();

  if (currTime - lastTime > CHECK_INTERVAL_MS)
  {
    printResult(currTime);
    lastTime = currTime;
  }
}
