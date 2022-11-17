/****************************************************************************************************************************
  SAMDTimerInterrupt.h
  For SAMD boards
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
/*
  SAMD21

  The Timer/Counter for Control Applications (TCC) module provides a set of timing and counting related functionality, such as the
  generation of periodic waveforms, the capturing of a periodic waveform's frequency/duty cycle, software timekeeping for periodic
  operations, waveform extension control, fault detection etc.

  The counter size of the TCC modules can be 16- or 24-bit depending on the TCC instance

  1) Nano-33-IoT SAMD21G18A
  .arduino15/packages/arduino/tools/CMSIS-Atmel/1.2.0/CMSIS/Device/ATMEL/samd21/include/samd21g18a.h
   #define TC3  ((Tc *)0x42002C00UL)


*/
#pragma once

#ifndef SAMDTIMERINTERRUPT_H
#define SAMDTIMERINTERRUPT_H

///////////////////////////////////////////

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

///////////////////////////////////////////

#if ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) \
   || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) || defined(ARDUINO_SAMD_MKRGSM1400) \
   || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) \
   || defined(__SAMD21E15A__) || defined(__SAMD21E16A__) || defined(__SAMD21E17A__) || defined(__SAMD21E18A__) \
   || defined(__SAMD21G15A__) || defined(__SAMD21G16A__) || defined(__SAMD21G17A__) || defined(__SAMD21G18A__) \
   || defined(__SAMD21J15A__) || defined(__SAMD21J16A__) || defined(__SAMD21J17A__) || defined(__SAMD21J18A__) || defined(__SAMD21__) )

#define TIMER_INTERRUPT_USING_SAMD21      true

#if !defined(BOARD_NAME)

  #if defined(ARDUINO_QWIIC_MICRO)
    #define BOARD_NAME    "Sparkfun SAMD21_QWIIC_MICRO"

    #if(_TIMERINTERRUPT_LOGLEVEL_>3)
      #warning BOARD_NAME == Sparkfun SAMD21_QWIIC_MICRO
    #endif

  #elif defined(__SAMD21E15A__)
    #define BOARD_NAME    "__SAMD21E15A__"
  #elif defined(__SAMD21E16A__)
    #define BOARD_NAME    "__SAMD21E16A__"
  #elif defined(__SAMD21E17A__)
    #define BOARD_NAME    "__SAMD21E17A__"
  #elif defined(__SAMD21E18A__)
    #define BOARD_NAME    "__SAMD21E18A__"
  #elif defined(__SAMD21G15A__)
    #define BOARD_NAME    "__SAMD21G15A__"
  #elif defined(__SAMD21G16A__)
    #define BOARD_NAME    "__SAMD21G16A__"
  #elif defined(__SAMD21G17A__)
    #define BOARD_NAME    "__SAMD21G17A__"
  #elif defined(__SAMD21G18A__)
    #define BOARD_NAME    "__SAMD21G18A__"
  #elif defined(__SAMD21J15A__)
    #define BOARD_NAME    "__SAMD21J15A__"
  #elif defined(__SAMD21J16A__)
    #define BOARD_NAME    "__SAMD21J16A__"
  #elif defined(__SAMD21J17A__)
    #define BOARD_NAME    "__SAMD21J17A__"
  #elif defined(__SAMD21J18A__)
    #define BOARD_NAME    "__SAMD21J18A__"
  #else
    #define BOARD_NAME    "Unknown SAMD21"
  #endif

#endif

#if(_TIMERINTERRUPT_LOGLEVEL_>3)
  #warning Using SAMD21 Hardware Timer
#endif

///////////////////////////////////////////

#elif ( defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) )

#define TIMER_INTERRUPT_USING_SAMD51      true

#if !defined(BOARD_NAME)

  #if defined(ARDUINO_SAMD51_THING_PLUS)
    #define BOARD_NAME    "Sparkfun SAMD51_THING_PLUS"

    #if(_TIMERINTERRUPT_LOGLEVEL_>3)
      #warning BOARD_NAME == Sparkfun SAMD51_THING_PLUS
    #endif

  #elif defined(ARDUINO_SAMD51_MICROMOD)
    #define BOARD_NAME    "Sparkfun SAMD51_MICROMOD"

    #if(_TIMERINTERRUPT_LOGLEVEL_>3)
      #warning BOARD_NAME == Sparkfun SAMD51_MICROMOD
    #endif
  #elif defined(__SAMD51J20A__)
    #define BOARD_NAME    "__SAMD51J20A__"
  #elif defined(__SAMD51J19A__)
    #define BOARD_NAME    "__SAMD51J19A__"
  #elif defined(__SAMD51G19A__)
    #define BOARD_NAME    "__SAMD51G19A__"
  #elif defined(__SAMD51P19A__)
    #define BOARD_NAME    "__SAMD51P19A__"
  #else
    #define BOARD_NAME    "Unknown SAMD51"
  #endif

#endif

#if(_TIMERINTERRUPT_LOGLEVEL_>3)
  #warning Using SAMD51 Hardware Timer
#endif
#else
#error Unknown board
#endif

///////////////////////////////////////////

// Too many boards sharing the same ARDUINO_SAMD_ZERO but very different, such as SAMD21 SparkFun RedBoard Turbo
// Have to exclude some from the list
#if ( defined(ARDUINO_SAMD_ZERO) && ! ( defined(ADAFRUIT_FEATHER_M0) || defined(ARDUINO_SAMD_FEATHER_M0) || defined(ADAFRUIT_METRO_M0_EXPRESS) || \
      defined(ARDUINO_SAMD_HALLOWING_M0) || defined(ADAFRUIT_BLM_BADGE) ) )

// Specific for SAMD21 SparkFun RedBoard Turbo
#if !defined(Serial)
  #define Serial    SerialUSB
#endif

#endif

///////////////////////////////////////////

#include "Arduino.h"

///////////////////////////////////////////

#ifndef SAMD_TIMER_INTERRUPT_VERSION
  #define SAMD_TIMER_INTERRUPT_VERSION            "SAMDTimerInterrupt v1.10.1"

  #define SAMD_TIMER_INTERRUPT_VERSION_MAJOR      1
  #define SAMD_TIMER_INTERRUPT_VERSION_MINOR      10
  #define SAMD_TIMER_INTERRUPT_VERSION_PATCH      1

  #define SAMD_TIMER_INTERRUPT_VERSION_INT        1010001
#endif

///////////////////////////////////////////

#include "TimerInterrupt_Generic_Debug.h"

#define TIMER_HZ      48000000L

////////////////////////////////////////////////////

#if (TIMER_INTERRUPT_USING_SAMD51)

///////////////////////////////////////////

typedef enum
{
  TIMER_TC3 = 0,
  MAX_TIMER
} SAMDTimerNumber;

///////////////////////////////////////////

class SAMDTimerInterrupt;

typedef SAMDTimerInterrupt SAMDTimer;

typedef void (*timerCallback)  ();

timerCallback TC3_callback;

#define SAMD_TC3        ((TcCount16*) _SAMDTimer)

////////////////////////////////////////////////////

static inline void TC3_wait_for_sync()
{
  while (TC3->COUNT16.SYNCBUSY.reg != 0);
}

////////////////////////////////////////////////////

void TC3_Handler()
{
  // If this interrupt is due to the compare register matching the timer count
  if (TC3->COUNT16.INTFLAG.bit.MC0 == 1)
  {
    TC3->COUNT16.INTFLAG.bit.MC0 = 1;
    (*TC3_callback)();
  }
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////

class SAMDTimerInterrupt
{
  private:

    ////////////////////////////////////////////////////

    SAMDTimerNumber _timerNumber;

    // point to timer struct, (TcCount16*) TC3 for SAMD51
    void*           _SAMDTimer = NULL;

    timerCallback   _callback;        // pointer to the callback function
    float           _frequency;       // Timer frequency

    float           _period;
    int             _prescaler;
    int             _compareValue;

    ////////////////////////////////////////////////////

  public:

    ////////////////////////////////////////////////////

    SAMDTimerInterrupt(const SAMDTimerNumber& timerNumber)
    {
      _timerNumber = timerNumber;

      if (_timerNumber == TIMER_TC3)
      {
        _SAMDTimer = (TcCount16*) TC3;
      }

      _callback = NULL;
    };

    ////////////////////////////////////////////////////

    ~SAMDTimerInterrupt()
    {
    }

    ////////////////////////////////////////////////////

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to SAMD-hal-timer.c
    bool setFrequency(const float& frequency, timerCallback callback)
    {
      _period =  (1000000.0f / frequency);

      if (_timerNumber == TIMER_TC3)
      {
        TISR_LOGWARN3(F("SAMDTimerInterrupt: F_CPU (MHz) ="), F_CPU / 1000000, F(", TIMER_HZ ="), TIMER_HZ / 1000000);
        TISR_LOGWARN3(F("TC_Timer::startTimer _Timer = 0x"), String((uint32_t) _SAMDTimer, HEX), F(", TC3 = 0x"),
                      String((uint32_t) TC3, HEX));

        // maxPermittedPeriod = 1,398,101.33us for 48MHz timer clock
        float maxPermittedPeriod = (65536.0f * 1024) / (TIMER_HZ / 1000000.0f );

        if (_period > maxPermittedPeriod )
        {
          TISR_LOGERROR3(F("Max permissible _period (us) ="),  maxPermittedPeriod, F(", current _period (us) ="), _period);

          return false;
        }

        // Enable the TC bus clock, use clock generator 0
        GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);

        while (GCLK->SYNCBUSY.reg > 0);

        TC3->COUNT16.CTRLA.bit.ENABLE = 0;

        // Use match mode so that the timer counter resets when the count matches the
        // compare register
        TC3->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
        TC3_wait_for_sync();

        // Enable the compare interrupt
        TC3->COUNT16.INTENSET.reg = 0;
        TC3->COUNT16.INTENSET.bit.MC0 = 1;

        // Enable IRQ
        NVIC_EnableIRQ(TC3_IRQn);

        _callback     = callback;
        TC3_callback  = callback;

        setPeriod_TIMER_TC3(_period);

        return true;
      }
      else
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to SAMD-hal-timer.c
    bool setInterval(const float& interval, timerCallback callback)
    {
      return setFrequency((float) (1000000.0f / interval), callback);
    }

    ////////////////////////////////////////////////////

    // interval (in milliseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here or to SAMD-hal-timer.c
    bool setInterval_MS(const float& interval, timerCallback callback)
    {
      return setFrequency((float) (1000.0f / interval), callback);
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    bool attachInterrupt(const float& frequency, timerCallback callback)
    {
      return setFrequency(frequency, callback);
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to SAMD-hal-timer.c
    bool attachInterruptInterval(const float& interval, timerCallback callback)
    {
      return setFrequency( (float) ( 1000000.0f / interval), callback);
    }

    ////////////////////////////////////////////////////

    // interval (in milliseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here or to SAMD-hal-timer.c
    bool attachInterruptInterval_MS(const float& interval, timerCallback callback)
    {
      return setFrequency( (float) ( 1000.0f / interval), callback);
    }

    ////////////////////////////////////////////////////

    void detachInterrupt()
    {
      // Disable Interrupt
      if (_timerNumber == TIMER_TC3)
      {
        NVIC_DisableIRQ(TC3_IRQn);
      }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void disableTimer()
    {
      // Disable Timer
      if (_timerNumber == TIMER_TC3)
      {
        // Disable TC3
        TC3->COUNT16.CTRLA.bit.ENABLE = 0;
      }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt()
    {
      // Disable Interrupt
      if (_timerNumber == TIMER_TC3)
      {
        NVIC_EnableIRQ(TC3_IRQn);
      }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer()
    {
      // Enable Timer
      if (_timerNumber == TIMER_TC3)
      {
        // Enable TC3
        SAMD_TC3->CTRLA.reg |= TC_CTRLA_ENABLE;
      }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // Just stop clock source, clear the count
    void stopTimer()
    {
      // TODO, clear the count
      disableTimer();
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // Just reconnect clock source, start current count from 0
    void restartTimer()
    {
      // TODO, clear the count
      enableTimer();
    }

    ///////////////////////////////////////////////////////////////////////////////////////

  private:

    ///////////////////////////////////////////////////////////////////////////////////////

    void setPeriod_TIMER_TC3(const float& period)
    {
      uint32_t TC_CTRLA_PRESCALER_DIVN = 1;

      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
      TC3_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1024;
      TC3_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV256;
      TC3_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV64;
      TC3_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV16;
      TC3_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV4;
      TC3_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV2;
      TC3_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1;
      TC3_wait_for_sync();

      if (period > 300000)
      {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV1024;
        _prescaler = 1024;
      }
      else if (80000 < period && period <= 300000)
      {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV256;
        _prescaler = 256;
      }
      else if (20000 < period && period <= 80000)
      {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV64;
        _prescaler = 64;
      }
      else if (10000 < period && period <= 20000)
      {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV16;
        _prescaler = 16;
      }
      else if (5000 < period && period <= 10000)
      {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV8;
        _prescaler = 8;
      }
      else if (2500 < period && period <= 5000)
      {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV4;
        _prescaler = 4;
      }
      else if (1000 < period && period <= 2500)
      {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV2;
        _prescaler = 2;
      }
      else if (period <= 1000)
      {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV1;
        _prescaler = 1;
      }

      TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIVN;
      TC3_wait_for_sync();

      _compareValue = (int)(TIMER_HZ / (_prescaler / (period / 1000000.0))) - 1;

      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      TC3->COUNT16.COUNT.reg = map(TC3->COUNT16.COUNT.reg, 0,
                                   TC3->COUNT16.CC[0].reg, 0, _compareValue);
      TC3->COUNT16.CC[0].reg = _compareValue;
      TC3_wait_for_sync();

      TC3->COUNT16.CTRLA.bit.ENABLE = 1;
      TC3_wait_for_sync();

      TISR_LOGDEBUG3(F("SAMD51 TC3 period ="), period, F(", _prescaler ="), _prescaler);
      TISR_LOGDEBUG1(F("_compareValue ="), _compareValue);
    }
}; // class SAMDTimerInterrupt


////////////////////////////////////////////////////////////////////////////////////////////////

#elif (TIMER_INTERRUPT_USING_SAMD21)

////////////////////////////////////////////////////

typedef enum
{
  TIMER_TC3  = 0,     // TC3
  TIMER_TC4  = 1,     // TC4
  TIMER_TC5  = 2,     // TC5
  TIMER_TCC  = 3,     // TCC0
  TIMER_TCC1 = 4,     // TCC1
  TIMER_TCC2 = 5,     // TCC2
  MAX_TIMER
} SAMDTimerNumber;

//static void* TIMER[MAX_TIMER] = { (Tc*) TC3, (Tc*) TC4, (Tc*) TC5, (Tcc*) TCC0, (Tcc*) TCC1, (Tcc*) TCC2 };

static IRQn_Type TIMER_IRQ[MAX_TIMER] = { TC3_IRQn, TC4_IRQn, TC5_IRQn, TCC0_IRQn, TCC1_IRQn, TCC2_IRQn };

static uint16_t TC_GCLK_ID[MAX_TIMER] = { GCM_TCC2_TC3, GCM_TC4_TC5, GCM_TC4_TC5, GCM_TCC0_TCC1, GCM_TCC0_TCC1, GCM_TCC2_TC3 };

////////////////////////////////////////////////////

class SAMDTimerInterrupt;

typedef SAMDTimerInterrupt SAMDTimer;

typedef void (*timerCallback)  ();

timerCallback TC3_callback;
timerCallback TCC_callback;
timerCallback TCC1_callback;
timerCallback TCC2_callback;
timerCallback TC4_callback;
timerCallback TC5_callback;


#define SAMD_TC3        ((TcCount16*) _SAMDTimer)
#define SAMD_TCC        ((Tcc*) _SAMDTimer)


////////////////////////////////////////////////////////


////////////////////////////////////////////////////////

#if USING_TIMER_TC3
#if(_TIMERINTERRUPT_LOGLEVEL_>3)
  #warning USING_TIMER_TC3
#endif

void TC3_Handler()
{
  // get timer struct
  TcCount16* TC = (TcCount16*) TC3;

  // If the compare register matching the timer count, trigger this interrupt
  if (TC->INTFLAG.bit.MC0 == 1)
  {
    TC->INTFLAG.bit.MC0 = 1;
    (*TC3_callback)();
  }
}

#endif

////////////////////////////////////////////////////////

#if USING_TIMER_TC4
#if(_TIMERINTERRUPT_LOGLEVEL_>3)
  #warning USING_TIMER_TC4
#endif

void TC4_Handler()
{
  // get timer struct
  TcCount16* TC = (TcCount16*) TC4;

  // If the compare register matching the timer count, trigger this interrupt
  if (TC->INTFLAG.bit.MC0 == 1)
  {
    TC->INTFLAG.bit.MC0 = 1;
    (*TC4_callback)();
  }
}

#endif

////////////////////////////////////////////////////////

#if USING_TIMER_TC5
#if(_TIMERINTERRUPT_LOGLEVEL_>3)
  #warning USING_TIMER_TC5
#endif

void TC5_Handler()
{
  // get timer struct
  TcCount16* TC = (TcCount16*) TC5;

  // If the compare register matching the timer count, trigger this interrupt
  if (TC->INTFLAG.bit.MC0 == 1)
  {
    TC->INTFLAG.bit.MC0 = 1;
    (*TC5_callback)();
  }
}

#endif

////////////////////////////////////////////////////////

#if USING_TIMER_TCC
#if(_TIMERINTERRUPT_LOGLEVEL_>3)
  #warning USING_TIMER_TCC
#endif

void TCC0_Handler()
{
  // get timer struct
  Tcc* TC = (Tcc*) TCC0;

  // If the compare register matching the timer count, trigger this interrupt
  if (TC->INTFLAG.bit.MC0 == 1)
  {
    // A compare to cc0 caused the interrupt
    TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag ovf flag
  }

  if (TC->INTFLAG.bit.OVF == 1)
  {
    (*TCC_callback)();

    TC->INTFLAG.bit.OVF = 1;
  }
}

#endif

////////////////////////////////////////////////////////

#if USING_TIMER_TCC1
#if(_TIMERINTERRUPT_LOGLEVEL_>3)
  #warning USING_TIMER_TCC1
#endif

void TCC1_Handler()
{
  // get timer struct
  Tcc* TC = (Tcc*) TCC1;

  // If the compare register matching the timer count, trigger this interrupt
  if (TC->INTFLAG.bit.MC0 == 1)
  {
    // A compare to cc0 caused the interrupt
    TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag ovf flag
  }

  if (TC->INTFLAG.bit.OVF == 1)
  {
    (*TCC1_callback)();

    TC->INTFLAG.bit.OVF = 1;
  }
}

#endif

////////////////////////////////////////////////////////

#if USING_TIMER_TCC2
#if(_TIMERINTERRUPT_LOGLEVEL_>3)
  #warning USING_TIMER_TCC2
#endif

void TCC2_Handler()
{
  // get timer struct
  Tcc* TC = (Tcc*) TCC2;

  // If the compare register matching the timer count, trigger this interrupt
  if (TC->INTFLAG.bit.MC0 == 1)
  {
    // A compare to cc0 caused the interrupt
    TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag ovf flag
  }

  if (TC->INTFLAG.bit.OVF == 1)
  {
    (*TCC2_callback)();

    TC->INTFLAG.bit.OVF = 1;
  }
}

#endif

///////////////////////////////////////////////////////////////////////////////

class SAMDTimerInterrupt
{
  private:

    ////////////////////////////////////////////////////

    SAMDTimerNumber _timerNumber;

    // point to timer struct, (TcCount16*) TC3 or (Tcc*) TCC0 for SAMD21
    void*           _SAMDTimer = NULL;

    timerCallback   _callback;        // pointer to the callback function
    float           _frequency;       // Timer frequency

    float           _period;
    int             _prescaler;
    int             _compareValue;

    ////////////////////////////////////////////////////

  public:

    ////////////////////////////////////////////////////

    SAMDTimerInterrupt(const SAMDTimerNumber& timerNumber)
    {
      _timerNumber = timerNumber;

      if (_timerNumber == TIMER_TC3)
      {
        _SAMDTimer = (TcCount16*) TC3;
      }
      else if (_timerNumber == TIMER_TCC)
      {
        _SAMDTimer = (Tcc*) TCC0;
      }
      else if (_timerNumber == TIMER_TCC1)
      {
        _SAMDTimer = (Tcc*) TCC1;
      }
      else if (_timerNumber == TIMER_TCC2)
      {
        _SAMDTimer = (Tcc*) TCC2;
      }
      else if (_timerNumber == TIMER_TC4)
      {
        _SAMDTimer = (TcCount16*) TC4;
      }
      else if (_timerNumber == TIMER_TC5)
      {
        _SAMDTimer = (TcCount16*) TC5;
      }

      _callback = NULL;
    };

    ////////////////////////////////////////////////////

    ~SAMDTimerInterrupt()
    {
    }

    ////////////////////////////////////////////////////

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to SAMD-hal-timer.c
    bool setFrequency(const float& frequency, timerCallback callback)
    {
      _period =  (1000000.0f / frequency);

      TISR_LOGDEBUG3(F("_period ="), _period, F(", frequency ="), frequency);

      if ( (_timerNumber == TIMER_TC3) || (_timerNumber == TIMER_TC4) || (_timerNumber == TIMER_TC5) )
      {
        TISR_LOGDEBUG1(F("_timerNumber ="), _timerNumber);

        // maxPermittedPeriod = 1,398,101.33us for 48MHz timer clock
        float maxPermittedPeriod = (65536.0f * 1024) / (TIMER_HZ / 1000000.0f );

        if (_period > maxPermittedPeriod )
        {
          TISR_LOGERROR3(F("Max permissible _period (us) ="),  maxPermittedPeriod, F(", current _period (us) ="), _period);

          return false;
        }

        REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID (
                                         TC_GCLK_ID[_timerNumber]));

        while ( GCLK->STATUS.bit.SYNCBUSY == 1 );

        TISR_LOGWARN3(F("SAMDTimerInterrupt: F_CPU (MHz) ="), F_CPU / 1000000, F(", TIMER_HZ ="), TIMER_HZ / 1000000);

        if (_timerNumber == TIMER_TC3)
        {
          TISR_LOGWARN3(F("TC3_Timer::startTimer _Timer = 0x"), String((uint32_t) _SAMDTimer, HEX), F(", TC3 = 0x"),
                        String((uint32_t) TC3, HEX));
        }
        else if (_timerNumber == TIMER_TC4)
        {
          TISR_LOGWARN3(F("TC4_Timer::startTimer _Timer = 0x"), String((uint32_t) _SAMDTimer, HEX), F(", TC4 = 0x"),
                        String((uint32_t) TC4, HEX));
        }
        else if (_timerNumber == TIMER_TC5)
        {
          TISR_LOGWARN3(F("TC5_Timer::startTimer _Timer = 0x"), String((uint32_t) _SAMDTimer, HEX), F(", TC5 = 0x"),
                        String((uint32_t) TC5, HEX));
        }

        SAMD_TC3->CTRLA.reg &= ~TC_CTRLA_ENABLE;

        // Use the 16-bit timer
        SAMD_TC3->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;

        while (SAMD_TC3->STATUS.bit.SYNCBUSY == 1);

        // Use match mode so that the timer counter resets when the count matches the compare register
        SAMD_TC3->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;

        while (SAMD_TC3->STATUS.bit.SYNCBUSY == 1);

        setPeriod_TIMER_TC3(_period);

        // Enable the compare interrupt
        SAMD_TC3->INTENSET.reg = 0;
        SAMD_TC3->INTENSET.bit.MC0 = 1;

        NVIC_EnableIRQ(TIMER_IRQ[_timerNumber]);

        SAMD_TC3->CTRLA.reg |= TC_CTRLA_ENABLE;

        while (SAMD_TC3->STATUS.bit.SYNCBUSY == 1);

        _callback     = callback;

        if (_timerNumber == TIMER_TC3)
          TC3_callback  = callback;
        else if (_timerNumber == TIMER_TC4)
          TC4_callback  = callback;
        else if (_timerNumber == TIMER_TC5)
          TC5_callback  = callback;
      }
      else if ( (_timerNumber == TIMER_TCC) || (_timerNumber == TIMER_TCC1) || (_timerNumber == TIMER_TCC2) )
      {
        REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(TC_GCLK_ID[_timerNumber]));

        while ( GCLK->STATUS.bit.SYNCBUSY == 1 );

        TISR_LOGWARN3(F("SAMDTimerInterrupt: F_CPU (MHz) ="), F_CPU / 1000000, F(", TIMER_HZ ="), TIMER_HZ / 1000000);

        if (_timerNumber == TIMER_TCC)
        {
          TISR_LOGWARN3(F("TCC_Timer::startTimer _Timer = 0x"), String((uint32_t) _SAMDTimer, HEX), F(", TCC0 = 0x"),
                        String((uint32_t) TCC0, HEX));
        }
        else if (_timerNumber == TIMER_TCC1)
        {
          TISR_LOGWARN3(F("TCC_Timer::startTimer _Timer = 0x"), String((uint32_t) _SAMDTimer, HEX), F(", TCC1 = 0x"),
                        String((uint32_t) TCC1, HEX));
        }
        else if (_timerNumber == TIMER_TCC2)
        {
          TISR_LOGWARN3(F("TCC_Timer::startTimer _Timer = 0x"), String((uint32_t) _SAMDTimer, HEX), F(", TCC2 = 0x"),
                        String((uint32_t) TCC2, HEX));
        }

        SAMD_TCC->CTRLA.reg &= ~TCC_CTRLA_ENABLE;   // Disable TC

        while (SAMD_TCC->SYNCBUSY.bit.ENABLE == 1); // wait for sync

        setPeriod_TIMER_TCC(_period);

        // Use match mode so that the timer counter resets when the count matches the compare register
        SAMD_TCC->WAVE.reg |= TCC_WAVE_WAVEGEN_NFRQ;   // Set wave form configuration

        while (SAMD_TCC->SYNCBUSY.bit.WAVE == 1); // wait for sync

        // Enable the compare interrupt
        SAMD_TCC->INTENSET.reg = 0;
        SAMD_TCC->INTENSET.bit.OVF = 1;
        SAMD_TCC->INTENSET.bit.MC0 = 1;

        NVIC_EnableIRQ(TIMER_IRQ[_timerNumber]);

        SAMD_TCC->CTRLA.reg |= TCC_CTRLA_ENABLE;

        while (SAMD_TCC->SYNCBUSY.bit.ENABLE == 1); // wait for sync

        _callback     = callback;

        if (_timerNumber == TIMER_TCC)
          TCC_callback  = callback;
        else if (_timerNumber == TIMER_TCC1)
          TCC1_callback  = callback;
        else if (_timerNumber == TIMER_TCC2)
          TCC2_callback  = callback;
      }

      return true;
    }

    ////////////////////////////////////////////////////

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here or to SAMD-hal-timer.c
    bool setInterval(const float& interval, timerCallback callback)
    {
      return setFrequency((float) (1000000.0f / interval), callback);
    }

    ////////////////////////////////////////////////////

    // interval (in milliseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here or to SAMD-hal-timer.c
    bool setInterval_MS(const float& interval, timerCallback callback)
    {
      return setFrequency((float) (1000.0f / interval), callback);
    }

    ////////////////////////////////////////////////////

    bool attachInterrupt(const float& frequency, timerCallback callback)
    {
      return setFrequency(frequency, callback);
    }

    ////////////////////////////////////////////////////

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here or to SAMD-hal-timer.c
    bool attachInterruptInterval(const float& interval, timerCallback callback)
    {
      return setFrequency( (float) ( 1000000.0f / interval), callback);
    }

    ////////////////////////////////////////////////////

    // interval (in milliseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here or to SAMD-hal-timer.c
    bool attachInterruptInterval_MS(const float& interval, timerCallback callback)
    {
      return setFrequency( (float) ( 1000.0f / interval), callback);
    }

    ////////////////////////////////////////////////////

    void detachInterrupt()
    {
      // Disable Interrupt
      NVIC_DisableIRQ(TIMER_IRQ[_timerNumber]);
    }

    ////////////////////////////////////////////////////

    void disableTimer()
    {
      // Disable Timer
      if ( (_timerNumber == TIMER_TC3) || (_timerNumber == TIMER_TC4) || (_timerNumber == TIMER_TC5) )
      {
        // Disable TCx
        SAMD_TC3->CTRLA.reg &= ~TC_CTRLA_ENABLE;
      }
      else if ( (_timerNumber == TIMER_TCC) || (_timerNumber == TIMER_TCC1) || (_timerNumber == TIMER_TCC2) )
      {
        // Disable TCCx
        SAMD_TCC->CTRLA.reg &= ~TCC_CTRLA_ENABLE;
      }
    }

    ////////////////////////////////////////////////////

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt()
    {
      // Disable Interrupt
      NVIC_EnableIRQ(TIMER_IRQ[_timerNumber]);
    }

    ////////////////////////////////////////////////////

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer()
    {
      // Enable Timer
      if ( (_timerNumber == TIMER_TC3) || (_timerNumber == TIMER_TC4) || (_timerNumber == TIMER_TC5) )
      {
        // Enable TCx
        SAMD_TC3->CTRLA.reg |= TC_CTRLA_ENABLE;
      }
      else if ( (_timerNumber == TIMER_TCC) || (_timerNumber == TIMER_TCC1) || (_timerNumber == TIMER_TCC2) )
      {
        // Enable TCCx
        SAMD_TCC->CTRLA.reg |= TCC_CTRLA_ENABLE;
      }
    }

    ////////////////////////////////////////////////////

    // Just stop clock source, clear the count
    void stopTimer()
    {
      // TODO, clear the count
      disableTimer();
    }

    ////////////////////////////////////////////////////

    // Just reconnect clock source, start current count from 0
    void restartTimer()
    {
      // TODO, clear the count
      enableTimer();
    }

    ////////////////////////////////////////////////////

  private:

    ////////////////////////////////////////////////////

    void setPeriod_TIMER_TC3(const float& period)
    {
      TcCount16* _Timer = (TcCount16*) _SAMDTimer;

      _Timer->CTRLA.reg &= ~( TC_CTRLA_ENABLE | TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_PRESCALER_DIV256 |
                              TC_CTRLA_PRESCALER_DIV64 |
                              TC_CTRLA_PRESCALER_DIV16 | TC_CTRLA_PRESCALER_DIV4 | TC_CTRLA_PRESCALER_DIV2 | TC_CTRLA_PRESCALER_DIV1 );

      while (_Timer->STATUS.bit.SYNCBUSY == 1);

      if (period > 300000)
      {
        // Set prescaler to 1024
        _Timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
        _prescaler = 1024;
      }
      else if (80000 < period && period <= 300000)
      {
        // Set prescaler to 256
        _Timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV256;
        _prescaler = 256;
      }
      else if (20000 < period && period <= 80000)
      {
        // Set prescaler to 64
        _Timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64;
        _prescaler = 64;
      }
      else if (10000 < period && period <= 20000)
      {
        // Set prescaler to 16
        _Timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV16;
        _prescaler = 16;
      }
      else if (5000 < period && period <= 10000)
      {
        // Set prescaler to 8
        _Timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV8;
        _prescaler = 8;
      }
      else if (2500 < period && period <= 5000)
      {
        // Set prescaler to 4
        _Timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV4;
        _prescaler = 4;
      }
      else if (1000 < period && period <= 2500)
      {
        // Set prescaler to 2
        _Timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV2;
        _prescaler = 2;
      }
      else if (period <= 1000)
      {
        // Set prescaler to 1
        _Timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1;
        _prescaler = 1;
      }

      while (_Timer->STATUS.bit.SYNCBUSY == 1);

      _compareValue = (int)(TIMER_HZ / (_prescaler / (period / 1000000.0))) - 1;


      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      _Timer->COUNT.reg = map(_Timer->COUNT.reg, 0, _Timer->CC[0].reg, 0, _compareValue);
      _Timer->CC[0].reg = _compareValue;

      while (_Timer->STATUS.bit.SYNCBUSY == 1);

      if (_timerNumber == TIMER_TC3)
      {
        TISR_LOGDEBUG3(F("SAMD21 TC3 period ="), period, F(", _prescaler ="), _prescaler);
      }
      else if (_timerNumber == TIMER_TC4)
      {
        TISR_LOGDEBUG3(F("SAMD21 TC4 period ="), period, F(", _prescaler ="), _prescaler);
      }
      else if (_timerNumber == TIMER_TC5)
      {
        TISR_LOGDEBUG3(F("SAMD21 TC5 period ="), period, F(", _prescaler ="), _prescaler);
      }

      TISR_LOGDEBUG1(F("_compareValue ="), _compareValue);
    }

    ////////////////////////////////////////////////////

    void setPeriod_TIMER_TCC(const float& period)
    {
      Tcc* _Timer = (Tcc*) _SAMDTimer;

      _Timer->CTRLA.reg &= ~( TCC_CTRLA_ENABLE | TCC_CTRLA_PRESCALER_DIV1024 | TCC_CTRLA_PRESCALER_DIV256 |
                              TCC_CTRLA_PRESCALER_DIV64 |
                              TCC_CTRLA_PRESCALER_DIV16 | TCC_CTRLA_PRESCALER_DIV4 | TCC_CTRLA_PRESCALER_DIV2 | TCC_CTRLA_PRESCALER_DIV1 );

      while (_Timer->SYNCBUSY.bit.ENABLE == 1);

      if (period > 300000)
      {
        // Set prescaler to 1024
        _Timer->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV1024;
        _prescaler = 1024;
      }
      else if (80000 < period && period <= 300000)
      {
        // Set prescaler to 256
        _Timer->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV256;
        _prescaler = 256;
      }
      else if (20000 < period && period <= 80000)
      {
        // Set prescaler to 64
        _Timer->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV64;
        _prescaler = 64;
      }
      else if (10000 < period && period <= 20000)
      {
        // Set prescaler to 16
        _Timer->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV16;
        _prescaler = 16;
      }
      else if (5000 < period && period <= 10000)
      {
        // Set prescaler to 8
        _Timer->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV8;
        _prescaler = 8;
      }
      else if (2500 < period && period <= 5000)
      {
        // Set prescaler to 4
        _Timer->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV4;
        _prescaler = 4;
      }
      else if (1000 < period && period <= 2500)
      {
        // Set prescaler to 2
        _Timer->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV2;
        _prescaler = 2;
      }
      else if (period <= 1000)
      {
        // Set prescaler to 1
        _Timer->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV1;
        _prescaler = 1;
      }

      _compareValue = (int)(TIMER_HZ / (_prescaler / (period / 1000000))) - 1;

      _Timer->PER.reg = _compareValue;

      while (_Timer->SYNCBUSY.bit.PER == 1);

      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      //_Timer->COUNT.reg = map(_Timer->COUNT.reg, 0, _Timer->CC[0].reg, 0, _compareValue);

      _Timer->CC[0].reg = 0xFFF;

      while (_Timer->SYNCBUSY.bit.CC0 == 1);

      if (_timerNumber == TIMER_TCC)
      {
        TISR_LOGDEBUG3(F("SAMD21 TCC period ="), period, F(", _prescaler ="), _prescaler);
      }
      else if (_timerNumber == TIMER_TCC1)
      {
        TISR_LOGDEBUG3(F("SAMD21 TCC1 period ="), period, F(", _prescaler ="), _prescaler);
      }
      else if (_timerNumber == TIMER_TCC2)
      {
        TISR_LOGDEBUG3(F("SAMD21 TCC2 period ="), period, F(", _prescaler ="), _prescaler);
      }

      TISR_LOGDEBUG1(F("_compareValue ="), _compareValue);
    }

    ////////////////////////////////////////////////////
}; // class SAMDTimerInterrupt

#endif

#endif    // SAMDTIMERINTERRUPT_H
