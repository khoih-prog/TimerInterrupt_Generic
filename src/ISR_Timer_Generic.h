/********************************************************************************************************************************
  ISR_Timer_Generic.h
  For Generic boards
  Written by Khoi Hoang

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one Hardware timer and avoid conflicting with other cores' tasks.
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

  Version: 1.13.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.1.0   K Hoang      10/11/2020 Initial Super-Library coding to merge all TimerInterrupt Libraries
  1.2.0   K Hoang      12/11/2020 Add STM32_TimerInterrupt Library
  1.3.0   K Hoang      01/12/2020 Add Mbed Mano-33-BLE Library. Add support to AVR UNO, Nano, Arduino Mini, Ethernet, BT. etc.
  1.3.1   K.Hoang      09/12/2020 Add complex examples and board Version String. Fix SAMD bug.
  1.3.2   K.Hoang      06/01/2021 Fix warnings. Optimize examples to reduce memory usage
  1.4.0   K.Hoang      02/04/2021 Add support to Arduino, Adafruit, Sparkfun AVR 32u4, 328P, 128128RFA1 and Sparkfun SAMD
  1.5.0   K.Hoang      17/04/2021 Add support to Arduino megaAVR ATmega4809-based boards (Nano Every, UNO WiFi Rev2, etc.)
  1.6.0   K.Hoang      15/06/2021 Add T3/T4 support to 32u4. Add support to RP2040, ESP32-S2
  1.7.0   K.Hoang      13/08/2021 Add support to Adafruit nRF52 core v0.22.0+
  1.8.0   K.Hoang      24/11/2021 Update to use latest TimerInterrupt Libraries' versions
  1.9.0   K.Hoang      09/05/2022 Update to use latest TimerInterrupt Libraries' versions
  1.10.0  K.Hoang      10/08/2022 Update to use latest ESP32_New_TimerInterrupt Library version
  1.11.0  K.Hoang      12/08/2022 Add support to new ESP32_C3, ESP32_S2 and ESP32_S3 boards
  1.12.0  K.Hoang      29/09/2022 Update for SAMD, RP2040, MBED_RP2040
  1.13.0  K.Hoang      16/11/2022 Fix doubled time for ESP32_C3,S2 and S3. Fix poor timer accuracy bug for MBED RP2040
                                  Fix bug disabling TCB0 for megaAVR
*****************************************************************************************************************************/

#pragma once

#ifndef ISR_TIMER_GENERIC_H
#define ISR_TIMER_GENERIC_H

///////////////////////////////////////////

#ifndef TIMER_INTERRUPT_GENERIC_VERSION
  #define TIMER_INTERRUPT_GENERIC_VERSION          "TimerInterrupt_Generic v1.12.0"

  #define TIMER_INTERRUPT_GENERIC_VERSION_MAJOR     1
  #define TIMER_INTERRUPT_GENERIC_VERSION_MINOR     12
  #define TIMER_INTERRUPT_GENERIC_VERSION_PATCH     0

  #define TIMER_INTERRUPT_GENERIC_VERSION_INT      1012000
#endif

///////////////////////////////////////////

#include <stddef.h>

///////////////////////////////////////////

#if ( defined(ESP8266) || ESP8266 ) || ( defined(ESP32) || ESP32 )
  #define IRAM_ATTR_PREFIX      IRAM_ATTR
#else
  #define IRAM_ATTR_PREFIX
#endif

#if !( ARDUINO_ESP32S2_DEV || ARDUINO_FEATHERS2 || ARDUINO_ESP32S2_THING_PLUS || ARDUINO_MICROS2 || \
      ARDUINO_METRO_ESP32S2 || ARDUINO_MAGTAG29_ESP32S2 || ARDUINO_FUNHOUSE_ESP32S2 || \
      ARDUINO_ADAFRUIT_FEATHER_ESP32S2_NOPSRAM || ARDUINO_ADAFRUIT_QTPY_ESP32S2)
#define CONFIG_ESP32_APPTRACE_ENABLE
#endif

#if ( defined(ESP8266) || ESP8266 )
extern "C"
{
#include "ets_sys.h"
#include "os_type.h"
#include "mem.h"
}
#else
#include <inttypes.h>
#endif

///////////////////////////////////////////

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#include "TimerInterrupt_Generic_Debug.h"

//#define ISR_Timer ISRTimer

typedef void (*timerCallback)();
typedef void (*timerCallback_p)(void *);

///////////////////////////////////////////

class ISR_Timer
{
  public:
    // maximum number of timers
#define MAX_NUMBER_TIMERS         16
#define TIMER_RUN_FOREVER         0
#define TIMER_RUN_ONCE            1

    // constructor
    ISR_Timer();

    void IRAM_ATTR_PREFIX init();

    // this function must be called inside loop()
    void IRAM_ATTR_PREFIX run();

    // Timer will call function 'f' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int IRAM_ATTR_PREFIX setInterval(const float& d, timerCallback f);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int IRAM_ATTR_PREFIX setInterval(const float& d, timerCallback_p f, void* p);

    // Timer will call function 'f' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int IRAM_ATTR_PREFIX setTimeout(const float& d, timerCallback f);

    // Timer will call function 'f' with parameter 'p' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int IRAM_ATTR_PREFIX setTimeout(const float& d, timerCallback_p f, void* p);

    // Timer will call function 'f' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int IRAM_ATTR_PREFIX setTimer(const float& d, timerCallback f, const uint32_t& n);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int IRAM_ATTR_PREFIX setTimer(const float& d, timerCallback_p f, void* p, const uint32_t& n);

    // updates interval of the specified timer
    bool IRAM_ATTR_PREFIX changeInterval(const uint8_t& numTimer, const float& d);

    // destroy the specified timer
    void IRAM_ATTR_PREFIX deleteTimer(const uint8_t& numTimer);

    // restart the specified timer
    void IRAM_ATTR_PREFIX restartTimer(const uint8_t& numTimer);

    // returns true if the specified timer is enabled
    bool IRAM_ATTR_PREFIX isEnabled(const uint8_t& numTimer);

    // enables the specified timer
    void IRAM_ATTR_PREFIX enable(const uint8_t& numTimer);

    // disables the specified timer
    void IRAM_ATTR_PREFIX disable(const uint8_t& numTimer);

    // enables all timers
    void IRAM_ATTR_PREFIX enableAll();

    // disables all timers
    void IRAM_ATTR_PREFIX disableAll();

    // enables the specified timer if it's currently disabled, and vice-versa
    void IRAM_ATTR_PREFIX toggle(const uint8_t& numTimer);

    // returns the number of used timers
    uint8_t IRAM_ATTR_PREFIX getNumTimers();

    ///////////////////////////////////////////

    // returns the number of available timers
    uint8_t IRAM_ATTR_PREFIX getNumAvailableTimers()
    {
      return MAX_NUMBER_TIMERS - numTimers;
    };

    ///////////////////////////////////////////
    ///////////////////////////////////////////

  private:
    // deferred call constants
#define TIMER_DEFCALL_DONTRUN   0       // don't call the callback function
#define TIMER_DEFCALL_RUNONLY   1       // call the callback function but don't delete the timer
#define TIMER_DEFCALL_RUNANDDEL 2       // call the callback function and delete the timer

    // low level function to initialize and enable a new timer
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int IRAM_ATTR_PREFIX setupTimer(const float& d, void* f, void* p, bool h, const uint32_t& n);

    // find the first available slot
    int IRAM_ATTR_PREFIX findFirstFreeSlot();

    ///////////////////////////////////////////

    typedef struct
    {
      unsigned long prev_millis;        // value returned by the millis() function in the previous run() call
      void*         callback;           // pointer to the callback function
      void*         param;              // function parameter
      bool          hasParam;           // true if callback takes a parameter
      float         delay;              // delay value
      uint32_t      maxNumRuns;         // number of runs to be executed
      uint32_t      numRuns;            // number of executed runs
      bool          enabled;            // true if enabled
      unsigned      toBeCalled;         // deferred function call (sort of) - N.B.: only used in run()
    } timer_t;

    ///////////////////////////////////////////

    volatile timer_t timer[MAX_NUMBER_TIMERS];

    // actual number of timers in use (-1 means uninitialized)
    volatile int numTimers;
};

#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
#endif

#include "ISR_Timer-Impl_Generic.h"

#endif    // ISR_TIMER_GENERIC_H

