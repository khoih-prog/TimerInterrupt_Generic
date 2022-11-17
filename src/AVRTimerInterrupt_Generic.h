/****************************************************************************************************************************
  TimerInterrupt.h

  For Arduino AVR boards (UNO, Nano, Mega, etc. )
  Written by Khoi Hoang

  TCNTx - Timer/Counter Register. The actual timer value is stored here.
  OCRx - Output Compare Register
  ICRx - Input Capture Register (only for 16bit timer)
  TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.
  TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt.

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

#ifndef TimerInterrupt_h
#define TimerInterrupt_h

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

#ifndef TIMER_INTERRUPT_VERSION
  #define TIMER_INTERRUPT_VERSION           "TimerInterrupt v1.8.0"

  #define TIMER_INTERRUPT_VERSION_MAJOR     1
  #define TIMER_INTERRUPT_VERSION_MINOR     8
  #define TIMER_INTERRUPT_VERSION_PATCH     0

  #define TIMER_INTERRUPT_VERSION_INT      1008000
#endif

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "pins_arduino.h"

#include "TimerInterrupt_Generic_Debug.h"

#define MAX_COUNT_8BIT            255
#define MAX_COUNT_10BIT           1023
#define MAX_COUNT_16BIT           65535

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
  #define TCCR2A TCCR2
  #define TCCR2B TCCR2
  #define COM2A1 COM21
  #define COM2A0 COM20
  #define OCR2A OCR2
  #define TIMSK2 TIMSK
  #define OCIE2A OCIE2
  #define TIMER2_COMPA_vect TIMER2_COMP_vect
  #define TIMSK1 TIMSK
#endif

typedef void (*timer_callback)();
typedef void (*timer_callback_p)(void *);

enum
{
  HW_TIMER_0 = 0,
  HW_TIMER_1,
  HW_TIMER_2,
  HW_TIMER_3,
  HW_TIMER_4,
  HW_TIMER_5,
  NUM_HW_TIMERS
};

enum
{
  NO_CLOCK_SOURCE = 0,
  NO_PRESCALER,
  PRESCALER_8,
  PRESCALER_64,
  PRESCALER_256,
  PRESCALER_1024,
  NUM_ITEMS
};

enum
{
  T2_NO_CLOCK_SOURCE = 0,
  T2_NO_PRESCALER,
  T2_PRESCALER_8,
  T2_PRESCALER_32,
  T2_PRESCALER_64,
  T2_PRESCALER_128,
  T2_PRESCALER_256,
  T2_PRESCALER_1024,
  T2_NUM_ITEMS
};

const unsigned int prescalerDiv   [NUM_ITEMS]     = { 1, 1, 8, 64, 256, 1024 };
const unsigned int prescalerDivT2 [T2_NUM_ITEMS]  = { 1, 1, 8, 32,  64,  128, 256, 1024 };

class TimerInterrupt
{
  private:

    bool            _timerDone;
    int8_t          _timer;
    unsigned int    _prescalerIndex;
    uint32_t        _OCRValue;
    uint32_t        _OCRValueRemaining;
    volatile long   _toggle_count;
    double           _frequency;

    void*           _callback;        // pointer to the callback function
    void*           _params;          // function parameter

    ///////////////////////////////////////////

    void set_OCR()
    {
      // Run with noInterrupt()
      // Set the OCR for the given timer,
      // set the toggle count,
      // then turn on the interrupts
      uint32_t _OCRValueToUse;

      switch (_timer)
      {
        case 1:
          _OCRValueToUse = min(MAX_COUNT_16BIT, _OCRValueRemaining);
          OCR1A = _OCRValueToUse;
          _OCRValueRemaining -= _OCRValueToUse;

#if defined(OCR1A) && defined(TIMSK1) && defined(OCIE1A)
          // Bit 1 – OCIEA: Output Compare A Match Interrupt Enable
          // When this bit is written to '1', and the I-flag in the Status Register is set (interrupts globally enabled), the
          // Timer/Counter Output Compare A Match interrupt is enabled. The corresponding Interrupt Vector is
          // executed when the OCFA Flag, located in TIFR1, is set.
          bitWrite(TIMSK1, OCIE1A, 1);
#elif defined(OCR1A) && defined(TIMSK) && defined(OCIE1A)
          // this combination is for at least the ATmega32
          bitWrite(TIMSK, OCIE1A, 1);
#endif
          break;

#if defined(OCR2A) && defined(TIMSK2) && defined(OCIE2A)

        case 2:
          _OCRValueToUse = min(MAX_COUNT_8BIT, _OCRValueRemaining);
          OCR2A = _OCRValueToUse;
          _OCRValueRemaining -= _OCRValueToUse;

          bitWrite(TIMSK2, OCIE2A, 1);
          break;
#endif

#if defined(OCR3A) && defined(TIMSK3) && defined(OCIE3A)

        case 3:
          _OCRValueToUse = min(MAX_COUNT_16BIT, _OCRValueRemaining);
          OCR3A = _OCRValueToUse;
          _OCRValueRemaining -= _OCRValueToUse;

          bitWrite(TIMSK3, OCIE3A, 1);
          break;
#endif

#if defined(OCR4A) && defined(TIMSK4) && defined(OCIE4A)

        case 4:

#if TIMER_INTERRUPT_USING_ATMEGA_32U4
          _OCRValueToUse = min(MAX_COUNT_8BIT, _OCRValueRemaining);
#else
          _OCRValueToUse = min(MAX_COUNT_16BIT, _OCRValueRemaining);
#endif

          OCR4A = _OCRValueToUse;
          _OCRValueRemaining -= _OCRValueToUse;

          bitWrite(TIMSK4, OCIE4A, 1);
          break;
#endif

#if defined(OCR5A) && defined(TIMSK5) && defined(OCIE5A)

        case 5:
          _OCRValueToUse = min(MAX_COUNT_16BIT, _OCRValueRemaining);
          OCR5A = _OCRValueToUse;
          _OCRValueRemaining -= _OCRValueToUse;

          bitWrite(TIMSK5, OCIE5A, 1);
          break;
#endif
      }

      // Flag _OCRValue == 0 => end of long timer
      if (_OCRValueRemaining == 0)
        _timerDone = true;

    }

    ///////////////////////////////////////////

  public:

    TimerInterrupt()
    {
      _timer              = -1;
      _frequency          = 0;
      _callback           = NULL;
      _params             = NULL;
      _timerDone          = false;
      _prescalerIndex     = NO_PRESCALER;
      _OCRValue           = 0;
      _OCRValueRemaining  = 0;
      _toggle_count       = -1;
    };

    explicit TimerInterrupt(uint8_t timerNo)
    {
      _timer              = timerNo;
      _frequency          = 0;
      _callback           = NULL;
      _params             = NULL;
      _timerDone          = false;
      _prescalerIndex     = NO_PRESCALER;
      _OCRValue           = 0;
      _OCRValueRemaining  = 0;
      _toggle_count       = -1;
    };

    void callback() __attribute__((always_inline))
    {
      if (_callback != NULL)
      {
        if (_params != NULL)
          (*(timer_callback_p)_callback)(_params);
        else
          (*(timer_callback)_callback)();
      }
    }

    void init(int8_t timer)
    {
      // Set timer specific stuff
      // All timers in CTC mode
      // 8 bit timers will require changing prescalar values,
      // whereas 16 bit timers are set to either ck/1 or ck/64 prescalar

      //cli();//stop interrupts
      noInterrupts();

      switch (timer)
      {
#if defined(TCCR1A) && defined(TCCR1B) && defined(WGM12)

        case 1:
          // 16 bit timer
          TCCR1A = 0;
          TCCR1B = 0;
          // Page 172-173. ATmega 328/328P or Page 145-146 of ATmega 640/1280/2560
          // Mode 4 => Clear Timer on Compare match (CTC) using OCR1A for counter value
          bitWrite(TCCR1B, WGM12, 1);
          // No scaling now
          bitWrite(TCCR1B, CS10, 1);

          TISR_LOGWARN(F("T1"));

          break;
#endif

#if defined(TCCR2A) && defined(TCCR2B)

        case 2:
          // 8 bit timer
          TCCR2A = 0;
          TCCR2B = 0;
          // Page 205-206. ATmegal328, Page 184-185 ATmega 640/1280/2560
          // Mode 2 => Clear Timer on Compare match (CTC) using OCR2A for counter value
          bitWrite(TCCR2A, WGM21, 1);
          // No scaling now
          bitWrite(TCCR2B, CS20, 1);

          TISR_LOGWARN(F("T2"));

          break;
#endif

#if defined(TCCR3A) && defined(TCCR3B) &&  defined(TIMSK3)

        case 3:
          // 16 bit timer
          TCCR3A = 0;
          TCCR3B = 0;
          bitWrite(TCCR3B, WGM32, 1);
          bitWrite(TCCR3B, CS30, 1);

          TISR_LOGWARN(F("T3"));

          break;
#endif

#if defined(TCCR4A) && defined(TCCR4B) &&  defined(TIMSK4)

        case 4:
          // 16 bit timer
          TCCR4A = 0;
          TCCR4B = 0;
#if defined(WGM42)
          bitWrite(TCCR4B, WGM42, 1);
#elif defined(CS43)
          // TODO this may not be correct
          // atmega32u4
          bitWrite(TCCR4B, CS43, 1);
#endif
          bitWrite(TCCR4B, CS40, 1);

          TISR_LOGWARN(F("T4"));

          break;
#endif

#if defined(TCCR5A) && defined(TCCR5B) &&  defined(TIMSK5)

        case 5:
          // 16 bit timer
          TCCR5A = 0;
          TCCR5B = 0;
          bitWrite(TCCR5B, WGM52, 1);
          bitWrite(TCCR5B, CS50, 1);

          TISR_LOGWARN(F("T5"));

          break;
#endif
      }

      _timer = timer;

      //sei();//enable interrupts
      interrupts();

    }

    ///////////////////////////////////////////

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // bool setFrequency(float frequency, timer_callback_p callback, /* void* */ uint32_t params, unsigned long duration = 0);

    // frequency (in hertz) and duration (in milliseconds).
    // Return true if frequency is OK with selected timer (OCRValue is in range)
    bool setFrequency(float frequency, timer_callback_p callback, uint32_t params, unsigned long duration = 0)
    {
      uint8_t       andMask = 0b11111000;
      unsigned long OCRValue;
      bool isSuccess = false;

      //frequencyLimit must > 1
      float frequencyLimit = frequency * 17179.840;

      // Limit frequency to larger than (0.00372529 / 64) Hz or interval 17179.840s / 17179840 ms to avoid uint32_t overflow
      if ((_timer <= 0) || (callback == NULL) || ((frequencyLimit) < 1) )
      {
        return false;
      }
      else
      {
        // Calculate the toggle count. Duration must be at least longer then one cycle
        if (duration > 0)
        {
          _toggle_count = frequency * duration / 1000;

          TISR_LOGWARN1(F("setFrequency => _toggle_count ="), _toggle_count);
          TISR_LOGWARN3(F("Frequency ="), frequency, F(", duration ="), duration);

          if (_toggle_count < 1)
          {
            return false;
          }
        }
        else
        {
          _toggle_count = -1;
        }

        //Timer0 and timer2 are 8 bit timers, meaning they can store a maximum counter value of 255.
        //Timer2 does not have the option of 1024 prescaler, only 1, 8, 32, 64
        //Timer1 is a 16 bit timer, meaning it can store a maximum counter value of 65535.
        int prescalerIndexStart;

        //Use smallest prescaler first, then increase until fits (<255)
        if (_timer != 2)
        {
          if (frequencyLimit > 64)
            prescalerIndexStart = NO_PRESCALER;
          else if (frequencyLimit > 8)
            prescalerIndexStart = PRESCALER_8;
          else
            prescalerIndexStart = PRESCALER_64;


          for (int prescalerIndex = prescalerIndexStart; prescalerIndex <= PRESCALER_1024; prescalerIndex++)
          {
            OCRValue = F_CPU / (frequency * prescalerDiv[prescalerIndex]) - 1;

            TISR_LOGWARN1(F("Freq * 1000 ="), frequency * 1000);
            TISR_LOGWARN3(F("F_CPU ="), F_CPU, F(", preScalerDiv ="), prescalerDiv[prescalerIndex]);
            TISR_LOGWARN3(F("OCR ="), OCRValue, F(", preScalerIndex ="), prescalerIndex);

            // We use very large _OCRValue now, and every time timer ISR activates, we deduct min(MAX_COUNT_16BIT, _OCRValueRemaining) from _OCRValueRemaining
            // So that we can create very long timer, even if the counter is only 16-bit.
            // Use very high frequency (OCRValue / MAX_COUNT_16BIT) around 16 * 1024 to achieve higher accuracy
#if TIMER_INTERRUPT_USING_ATMEGA_32U4
            uint16_t MAX_COUNT_32U4 = (_timer == 4) ? MAX_COUNT_8BIT : MAX_COUNT_16BIT;

            if ( (OCRValue / MAX_COUNT_32U4) < 16384 )
#else
            if ( (OCRValue / MAX_COUNT_16BIT) < 16384 )
#endif
            {
              _OCRValue           = OCRValue;
              _OCRValueRemaining  = OCRValue;
              _prescalerIndex = prescalerIndex;

              TISR_LOGWARN1(F("OK in loop => _OCR ="), _OCRValue);
              TISR_LOGWARN3(F("_preScalerIndex ="), _prescalerIndex, F(", preScalerDiv ="), prescalerDiv[_prescalerIndex]);

              isSuccess = true;

              break;
            }
          }

          if (!isSuccess)
          {
            // Always do this
            _OCRValue           = OCRValue;
            _OCRValueRemaining  = OCRValue;
            _prescalerIndex = PRESCALER_1024;

            TISR_LOGWARN1(F("OK out loop => _OCR ="), _OCRValue);
            TISR_LOGWARN3(F("_preScalerIndex ="), _prescalerIndex, F(", preScalerDiv ="), prescalerDiv[_prescalerIndex]);
          }
        }
        else
        {
          if (frequencyLimit > 64)
            prescalerIndexStart = T2_NO_PRESCALER;
          else if (frequencyLimit > 8)
            prescalerIndexStart = T2_PRESCALER_8;
          else if (frequencyLimit > 2)
            prescalerIndexStart = T2_PRESCALER_32;
          else
            prescalerIndexStart = T2_PRESCALER_64;

          // Page 206-207. ATmegal328
          //8-bit Timer2 has more options up to 1024 prescaler, from 1, 8, 32, 64, 128, 256 and 1024
          for (int prescalerIndex = prescalerIndexStart; prescalerIndex <= T2_PRESCALER_1024; prescalerIndex++)
          {
            OCRValue = F_CPU / (frequency * prescalerDivT2[prescalerIndex]) - 1;

            TISR_LOGWARN3(F("F_CPU ="), F_CPU, F(", preScalerDiv ="), prescalerDivT2[prescalerIndex]);
            TISR_LOGWARN3(F("OCR2 ="), OCRValue, F(", preScalerIndex ="), prescalerIndex);

            // We use very large _OCRValue now, and every time timer ISR activates, we deduct min(MAX_COUNT_8BIT, _OCRValue) from _OCRValue
            // to create very long timer, even if the counter is only 16-bit.
            // Use very high frequency (OCRValue / MAX_COUNT_8BIT) around 16 * 1024 to achieve higher accuracy
            if ( (OCRValue / MAX_COUNT_8BIT) < 16384 )
            {
              _OCRValue           = OCRValue;
              _OCRValueRemaining  = OCRValue;
              // same as prescalarbits
              _prescalerIndex = prescalerIndex;

              TISR_LOGWARN1(F("OK in loop => _OCR ="), _OCRValue);
              TISR_LOGWARN3(F("_preScalerIndex ="), _prescalerIndex, F(", preScalerDiv ="), prescalerDivT2[_prescalerIndex]);

              isSuccess = true;

              break;
            }
          }

          if (!isSuccess)
          {
            // Always do this
            _OCRValue           = OCRValue;
            _OCRValueRemaining  = OCRValue;
            // same as prescalarbits
            _prescalerIndex = T2_PRESCALER_1024;

            TISR_LOGWARN1(F("OK out loop => _OCR ="), _OCRValue);
            TISR_LOGWARN3(F("_preScalerIndex ="), _prescalerIndex, F(", preScalerDiv ="), prescalerDivT2[_prescalerIndex]);
          }
        }

        //cli();//stop interrupts
        noInterrupts();

        _frequency = frequency;
        _callback  = (void*) callback;
        _params    = reinterpret_cast<void*>(params);

        _timerDone = false;

        // 8 bit timers from here
#if defined(TCCR2B)

        if (_timer == 2)
        {
          TCCR2B = (TCCR2B & andMask) | _prescalerIndex;   //prescalarbits;

          TISR_LOGWARN1(F("TCCR2B ="), TCCR2B);
        }

#endif

        // 16 bit timers from here
#if defined(TCCR1B)
#if ( TIMER_INTERRUPT_USING_ATMEGA_32U4 )

        if (_timer == 1)
#else
        else if (_timer == 1)
#endif
        {
          TCCR1B = (TCCR1B & andMask) | _prescalerIndex;   //prescalarbits;

          TISR_LOGWARN1(F("TCCR1B ="), TCCR1B);
        }

#endif

#if defined(TCCR3B)
        else if (_timer == 3)
          TCCR3B = (TCCR3B & andMask) | _prescalerIndex;   //prescalarbits;

#endif

#if defined(TCCR4B)
        else if (_timer == 4)
          TCCR4B = (TCCR4B & andMask) | _prescalerIndex;   //prescalarbits;

#endif

#if defined(TCCR5B)
        else if (_timer == 5)
          TCCR5B = (TCCR5B & andMask) | _prescalerIndex;   //prescalarbits;

#endif

        // Set the OCR for the given timer,
        // set the toggle count,
        // then turn on the interrupts
        set_OCR();

        //sei();//allow interrupts
        interrupts();

        return true;
      }
    }

    ///////////////////////////////////////////


    void init()
    {
      init(_timer);
    };


    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setFrequency(float frequency, timer_callback callback, unsigned long duration = 0)
    {
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    template<typename TArg>
    bool setInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "setInterval() callback argument size must be <= 4 bytes");
      return setFrequency((float) (1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), (uint32_t) params,
                          duration);
    }

    // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
    {
      return setFrequency((float) (1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    template<typename TArg>
    bool attachInterrupt(float frequency, void (*callback)(TArg), TArg params, unsigned long duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "attachInterrupt() callback argument size must be <= 4 bytes");
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    bool attachInterrupt(float frequency, timer_callback callback, unsigned long duration = 0)
    {
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    template<typename TArg>
    bool attachInterruptInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "attachInterruptInterval() callback argument size must be <= 4 bytes");
      return setFrequency( (float) ( 1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), (uint32_t) params,
                           duration);
    }

    // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool attachInterruptInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
    {
      return setFrequency( (float) ( 1000.0f / interval), reinterpret_cast<timer_callback_p> (callback), /*NULL*/ 0,
                           duration);
    }


    ///////////////////////////////////////////

    // void detachInterrupt();

    void detachInterrupt()
    {
      //cli();//stop interrupts
      noInterrupts();

      switch (_timer)
      {
#if defined(TIMSK1) && defined(OCIE1A)

        case 1:
          bitWrite(TIMSK1, OCIE1A, 0);

          TISR_LOGWARN(F("Disable T1"));

          break;
#endif

        case 2:
#if defined(TIMSK2) && defined(OCIE2A)
          bitWrite(TIMSK2, OCIE2A, 0); // disable interrupt
#endif

          TISR_LOGWARN(F("Disable T2"));

          break;

#if defined(TIMSK3) && defined(OCIE3A)

        case 3:
          bitWrite(TIMSK3, OCIE3A, 0);

          TISR_LOGWARN(F("Disable T3"));

          break;
#endif

#if defined(TIMSK4) && defined(OCIE4A)

        case 4:
          bitWrite(TIMSK4, OCIE4A, 0);

          TISR_LOGWARN(F("Disable T4"));

          break;
#endif

#if defined(TIMSK5) && defined(OCIE5A)

        case 5:
          bitWrite(TIMSK5, OCIE5A, 0);

          TISR_LOGWARN(F("Disable T5"));

          break;
#endif
      }

      //sei();//allow interrupts
      interrupts();
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt(unsigned long duration = 0)
    {
      //cli();//stop interrupts
      noInterrupts();

      // Calculate the toggle count
      if (duration > 0)
      {
        _toggle_count = _frequency * duration / 1000;
      }
      else
      {
        _toggle_count = -1;
      }

      switch (_timer)
      {
#if defined(TIMSK1) && defined(OCIE1A)

        case 1:
          bitWrite(TIMSK1, OCIE1A, 1);

          TISR_LOGWARN(F("Enable T1"));

          break;
#endif

        case 2:
#if defined(TIMSK2) && defined(OCIE2A)
          bitWrite(TIMSK2, OCIE2A, 1); // enable interrupt
#endif

          TISR_LOGWARN(F("Enable T2"));

          break;

#if defined(TIMSK3) && defined(OCIE3A)

        case 3:
          bitWrite(TIMSK3, OCIE3A, 1);

          TISR_LOGWARN(F("Enable T3"));

          break;
#endif

#if defined(TIMSK4) && defined(OCIE4A)

        case 4:
          bitWrite(TIMSK4, OCIE4A, 1);

          TISR_LOGWARN(F("Enable T4"));

          break;
#endif

#if defined(TIMSK5) && defined(OCIE5A)

        case 5:
          bitWrite(TIMSK5, OCIE5A, 1);

          TISR_LOGWARN(F("Enable T5"));

          break;
#endif
      }

      //sei();//allow interrupts
      interrupts();
    }

    ///////////////////////////////////////////

    void disableTimer()
    {
      detachInterrupt();
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer(unsigned long duration = 0) __attribute__((always_inline))
    {
      reattachInterrupt(duration);
    }

    ///////////////////////////////////////////

    // Just stop clock source, still keep the count
    // void pauseTimer();

    // Just stop clock source, still keep the count
    void pauseTimer()
    {
      uint8_t andMask = 0b11111000;

      //Just clear the CSx2-CSx0. Still keep the count in TCNT and Timer Interrupt mask TIMKSx.

      // 8 bit timers from here
#if defined(TCCR2B)

      if (_timer == 2)
      {
        TCCR2B = (TCCR2B & andMask);

        TISR_LOGWARN1(F("TCCR2B ="), TCCR2B);
      }

#endif

      // 16 bit timers from here
#if defined(TCCR1B)
#if ( TIMER_INTERRUPT_USING_ATMEGA_32U4 )

      if (_timer == 1)
#else
      else if (_timer == 1)
#endif
      {
        TCCR1B = (TCCR1B & andMask);

        TISR_LOGWARN1(F("TCCR1B ="), TCCR1B);
      }

#endif

#if defined(TCCR3B)
      else if (_timer == 3)
        TCCR3B = (TCCR3B & andMask);

#endif

#if defined(TCCR4B)
      else if (_timer == 4)
        TCCR4B = (TCCR4B & andMask);

#endif

#if defined(TCCR5B)
      else if (_timer == 5)
        TCCR5B = (TCCR5B & andMask);

#endif
    }

    // Just reconnect clock source, continue from the current count
    // void resumeTimer();

    // Just reconnect clock source, continue from the current count
    void resumeTimer()
    {
      uint8_t andMask = 0b11111000;

      //Just restore the CSx2-CSx0 stored in _prescalerIndex. Still keep the count in TCNT and Timer Interrupt mask TIMKSx.
      // 8 bit timers from here
#if defined(TCCR2B)

      if (_timer == 2)
      {
        TCCR2B = (TCCR2B & andMask) | _prescalerIndex;   //prescalarbits;

        TISR_LOGWARN1(F("TCCR2B ="), TCCR2B);
      }

#endif

      // 16 bit timers from here
#if defined(TCCR1B)
#if ( TIMER_INTERRUPT_USING_ATMEGA_32U4 )

      if (_timer == 1)
#else
      else if (_timer == 1)
#endif
      {
        TCCR1B = (TCCR1B & andMask) | _prescalerIndex;   //prescalarbits;

        TISR_LOGWARN1(F("TCCR1B ="), TCCR1B);
      }

#endif

#if defined(TCCR3B)
      else if (_timer == 3)
        TCCR3B = (TCCR3B & andMask) | _prescalerIndex;   //prescalarbits;

#endif

#if defined(TCCR4B)
      else if (_timer == 4)
        TCCR4B = (TCCR4B & andMask) | _prescalerIndex;   //prescalarbits;

#endif

#if defined(TCCR5B)
      else if (_timer == 5)
        TCCR5B = (TCCR5B & andMask) | _prescalerIndex;   //prescalarbits;

#endif
    }

    ///////////////////////////////////////////


    // Just stop clock source, clear the count
    void stopTimer()
    {
      detachInterrupt();
    }

    // Just reconnect clock source, start current count from 0
    void restartTimer(unsigned long duration = 0)
    {
      reattachInterrupt(duration);
    }

    int8_t getTimer() __attribute__((always_inline))
    {
      return _timer;
    };

    long getCount() __attribute__((always_inline))
    {
      return _toggle_count;
    };

    void setCount(long countInput) __attribute__((always_inline))
    {
      //cli();//stop interrupts
      //noInterrupts();

      _toggle_count = countInput;

      //sei();//enable interrupts
      //interrupts();
    };

    long get_OCRValue() __attribute__((always_inline))
    {
      return _OCRValue;
    };

    long get_OCRValueRemaining() __attribute__((always_inline))
    {
      return _OCRValueRemaining;
    };

    void adjust_OCRValue() //__attribute__((always_inline))
    {
      //cli();//stop interrupts
      noInterrupts();

      if (_timer != 2)
      {
#if TIMER_INTERRUPT_USING_ATMEGA_32U4

        if (_timer == 4)
        {
          if (_OCRValueRemaining < MAX_COUNT_8BIT)
          {
            set_OCR();
          }

          _OCRValueRemaining -= min(MAX_COUNT_8BIT, _OCRValueRemaining);
        }
        else
        {
          if (_OCRValueRemaining < MAX_COUNT_16BIT)
          {
            set_OCR();
          }

          _OCRValueRemaining -= min(MAX_COUNT_16BIT, _OCRValueRemaining);
        }

#else

        if (_OCRValueRemaining < MAX_COUNT_16BIT)
        {
          set_OCR();
        }

        _OCRValueRemaining -= min(MAX_COUNT_16BIT, _OCRValueRemaining);
#endif
      }
      else
      {
        if (_OCRValueRemaining < MAX_COUNT_8BIT)
        {
          set_OCR();
        }

        _OCRValueRemaining -= min(MAX_COUNT_8BIT, _OCRValueRemaining);
      }

      if (_OCRValueRemaining <= 0)
      {
        // Reset value for next cycle
        _OCRValueRemaining = _OCRValue;
        _timerDone = true;
      }
      else
        _timerDone = false;

      //sei();//enable interrupts
      interrupts();
    };

    void reload_OCRValue() //__attribute__((always_inline))
    {
      //cli();//stop interrupts
      noInterrupts();

      // Reset value for next cycle, have to deduct the value already loaded to OCR register

      _OCRValueRemaining = _OCRValue;
      set_OCR();

      _timerDone = false;

      //sei();//enable interrupts
      interrupts();
    };

    bool checkTimerDone() //__attribute__((always_inline))
    {
      return _timerDone;
    };

}; // class TimerInterrupt

//////////////////////////////////////////////

// To be sure not used Timers are disabled
#if !defined(USE_TIMER_1)
  #define USE_TIMER_1     false
#endif

#if !defined(USE_TIMER_2)
  #define USE_TIMER_2     false
#elif ( USE_TIMER_2 && TIMER_INTERRUPT_USING_ATMEGA_32U4 )
  #error Timer2 is disabled for ATMEGA_32U4, only available for ATMEGA_328(P) and Mega
#endif

#if !defined(USE_TIMER_3)
  #define USE_TIMER_3     false
#elif ( USE_TIMER_3 && ( TIMER_INTERRUPT_USING_ATMEGA_32U4 || TIMER_INTERRUPT_USING_ATMEGA2560 ) )
  #if(_TIMERINTERRUPT_LOGLEVEL_>3)
    #warning Timer3 (16-bit) is OK to use for ATMEGA_32U4 and Mega
  #endif
#elif USE_TIMER_3
  #error Timer3 is only available for ATMEGA_32U4 and Mega
#endif

#if !defined(USE_TIMER_4)
  #define USE_TIMER_4     false
#elif ( USE_TIMER_4 && ( TIMER_INTERRUPT_USING_ATMEGA_32U4 || TIMER_INTERRUPT_USING_ATMEGA2560 ) )
  #if(_TIMERINTERRUPT_LOGLEVEL_>3)
    #warning Timer4 is OK to use for ATMEGA_32U4 (10-bit but using as 8-bit) and Mega (16-bit)
  #endif
#elif USE_TIMER_4
  #error Timer4 is only available for ATMEGA_32U4 and Mega
#endif

#if !defined(USE_TIMER_5)
  #define USE_TIMER_5     false
#elif ( USE_TIMER_5 && TIMER_INTERRUPT_USING_ATMEGA2560 )
  #if(_TIMERINTERRUPT_LOGLEVEL_>3)
    #warning Timer5 is OK to use for Mega
  #endif
#elif USE_TIMER_5
  #error Timer5 is only available for Mega
#endif

//////////////////////////////////////////////

#if USE_TIMER_1
#ifndef TIMER1_INSTANTIATED
// To force pre-instatiate only once
#define TIMER1_INSTANTIATED
TimerInterrupt ITimer1(HW_TIMER_1);

// Timer0 is used for micros(), millis(), delay(), etc and can't be used
// Pre-instatiate

ISR(TIMER1_COMPA_vect)
{
  long countLocal = ITimer1.getCount();

  if (ITimer1.getTimer() == 1)
  {
    if (countLocal != 0)
    {
      if (ITimer1.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T1 callback, _OCRValueRemaining ="), ITimer1.get_OCRValueRemaining(), (", millis ="), millis());

        ITimer1.callback();

        // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_16BIT if _OCRValueRemaining > MAX_COUNT_16BIT
        if (ITimer1.get_OCRValue() > MAX_COUNT_16BIT)
        {
          ITimer1.reload_OCRValue();
        }

        if (countLocal > 0)
          ITimer1.setCount(countLocal - 1);
      }
      else
      {
        //Deduct _OCRValue by min(MAX_COUNT_16BIT, _OCRValue)
        // If _OCRValue == 0, flag _timerDone for next cycle
        // If last one (_OCRValueRemaining < MAX_COUNT_16BIT) => load _OCR register _OCRValueRemaining
        ITimer1.adjust_OCRValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T1 done"));

      ITimer1.detachInterrupt();
    }
  }
}

#endif  //#ifndef TIMER1_INSTANTIATED
#endif    //#if USE_TIMER_1

#if USE_TIMER_2
#ifndef TIMER2_INSTANTIATED
#define TIMER2_INSTANTIATED
TimerInterrupt ITimer2(HW_TIMER_2);

ISR(TIMER2_COMPA_vect)
{
  long countLocal = ITimer2.getCount();

  if (ITimer2.getTimer() == 2)
  {
    if (countLocal != 0)
    {
      if (ITimer2.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T2 callback, _OCRValueRemaining ="), ITimer2.get_OCRValueRemaining(), (", millis ="), millis());

        ITimer2.callback();

        // To reload _OCRValue
        if (ITimer2.get_OCRValue() > MAX_COUNT_8BIT)
        {
          ITimer2.reload_OCRValue();
        }

        if (countLocal > 0)
          ITimer2.setCount(countLocal - 1);

      }
      else
      {
        //Deduct _OCRValue by min(MAX_COUNT_8BIT, _OCRValue)
        // If _OCRValue == 0, flag _timerDone for next cycle
        ITimer2.adjust_OCRValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T2 done"));

      ITimer2.detachInterrupt();
    }
  }
}
#endif  //#ifndef TIMER2_INSTANTIATED
#endif    //#if USE_TIMER_2

#if (TIMER_INTERRUPT_USING_ATMEGA2560 || TIMER_INTERRUPT_USING_ATMEGA_32U4)

// Pre-instatiate
#if USE_TIMER_3
#ifndef TIMER3_INSTANTIATED
// To force pre-instatiate only once
#define TIMER3_INSTANTIATED
TimerInterrupt ITimer3(HW_TIMER_3);

ISR(TIMER3_COMPA_vect)
{
  long countLocal = ITimer3.getCount();

  if (ITimer3.getTimer() == 3)
  {
    if (countLocal != 0)
    {
      if (ITimer3.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T3 callback, _OCRValueRemaining ="), ITimer3.get_OCRValueRemaining(), (", millis ="), millis());

        ITimer3.callback();

        // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_16BIT
        if (ITimer3.get_OCRValue() > MAX_COUNT_16BIT)
        {
          ITimer3.reload_OCRValue();
        }

        if (countLocal > 0)
          ITimer3.setCount(countLocal - 1);
      }
      else
      {
        //Deduct _OCRValue by min(MAX_COUNT_16BIT, _OCRValue)
        // If _OCRValue == 0, flag _timerDone for next cycle
        // If last one (_OCRValueRemaining < MAX_COUNT_16BIT) => load _OCR register _OCRValueRemaining
        ITimer3.adjust_OCRValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T3 done"));

      ITimer3.detachInterrupt();
    }
  }
}

#endif  //#ifndef TIMER3_INSTANTIATED
#endif    //#if USE_TIMER_3

#endif      //#if (TIMER_INTERRUPT_USING_ATMEGA2560 || TIMER_INTERRUPT_USING_ATMEGA_32U4)

#if (TIMER_INTERRUPT_USING_ATMEGA2560 || TIMER_INTERRUPT_USING_ATMEGA_32U4)

// Even 32u4 Timer4 has 10-bit counter, we use only 8-bit to simplify by not using 2-bit High Byte Register (TC4H)
// Check 15.2.2 Accuracy, page 141 of ATmega16U4/32U4 [DATASHEET]

#if USE_TIMER_4
#ifndef TIMER4_INSTANTIATED
// To force pre-instatiate only once
#define TIMER4_INSTANTIATED
TimerInterrupt ITimer4(HW_TIMER_4);

ISR(TIMER4_COMPA_vect)
{
  long countLocal = ITimer4.getCount();

  if (ITimer4.getTimer() == 4)
  {
    if (countLocal != 0)
    {
      if (ITimer4.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T4 callback, _OCRValueRemaining ="), ITimer4.get_OCRValueRemaining(), (", millis ="), millis());

        ITimer4.callback();

        // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_16BIT (Mega2560) or MAX_COUNT_8BIT (32u4)
        if (ITimer4.get_OCRValue() > MAX_COUNT_16BIT)
        {
          ITimer4.reload_OCRValue();
        }

        if (countLocal > 0)
          ITimer4.setCount(countLocal - 1);
      }
      else
      {
        //Deduct _OCRValue by min(MAX_COUNT_16BIT, _OCRValue) or min(MAX_COUNT_8BIT, _OCRValue)
        // If _OCRValue == 0, flag _timerDone for next cycle
        // If last one (_OCRValueRemaining < MAX_COUNT_16BIT / MAX_COUNT_8BIT) => load _OCR register _OCRValueRemaining
        ITimer4.adjust_OCRValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T4 done"));

      ITimer4.detachInterrupt();
    }
  }
}


#endif  //#ifndef TIMER4_INSTANTIATED
#endif    //#if USE_TIMER_4

#endif      //#if (TIMER_INTERRUPT_USING_ATMEGA2560 || TIMER_INTERRUPT_USING_ATMEGA_32U4)

#if TIMER_INTERRUPT_USING_ATMEGA2560

#if USE_TIMER_5
#ifndef TIMER5_INSTANTIATED
// To force pre-instatiate only once
#define TIMER5_INSTANTIATED
TimerInterrupt ITimer5(HW_TIMER_5);

ISR(TIMER5_COMPA_vect)
{
  long countLocal = ITimer5.getCount();

  if (ITimer5.getTimer() == 5)
  {
    if (countLocal != 0)
    {
      if (ITimer5.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T5 callback, _OCRValueRemaining ="), ITimer5.get_OCRValueRemaining(), (", millis ="), millis());

        ITimer5.callback();

        // To reload _OCRValueRemaining as well as _OCR register to MAX_COUNT_16BIT
        if (ITimer5.get_OCRValue() > MAX_COUNT_16BIT)
        {
          ITimer5.reload_OCRValue();
        }

        if (countLocal > 0)
          ITimer5.setCount(countLocal - 1);
      }
      else
      {
        //Deduct _OCRValue by min(MAX_COUNT_16BIT, _OCRValue)
        // If _OCRValue == 0, flag _timerDone for next cycle
        // If last one (_OCRValueRemaining < MAX_COUNT_16BIT) => load _OCR register _OCRValueRemaining
        ITimer5.adjust_OCRValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T5 done"));

      ITimer5.detachInterrupt();
    }
  }
}

#endif  //#ifndef TIMER5_INSTANTIATED
#endif    //#if USE_TIMER_5

#endif      //#if TIMER_INTERRUPT_USING_ATMEGA2560

#endif      //#ifndef TimerInterrupt_h

