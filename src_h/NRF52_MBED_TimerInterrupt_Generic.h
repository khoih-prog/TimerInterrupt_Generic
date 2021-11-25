/****************************************************************************************************************************
  NRF52_MBED_TimerInterrupt.h
  For NRF52 boards using mbed-RTOS such as Nano-33-BLE
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/NRF52_MBED_TimerInterrupt
  Licensed under MIT license

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

  Version: 1.8.0

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
*****************************************************************************************************************************/
/*
  nRF52 has 5 Hardware TIMERs: NRF_TIMER0-NRF_TIMER4
  NRF_TIMER0 is used by the soft device, NRF_TIMER1-NRF_TIMER4 are available
  
  Defined in file ./adafruit/hardware/nrf52/0.21.0/cores/nRF5/nordic/nrfx/mdk/nrf52.h
  
  #define NRF_TIMER0_BASE             0x40008000UL
  #define NRF_TIMER1_BASE             0x40009000UL
  #define NRF_TIMER2_BASE             0x4000A000UL
  #define NRF_TIMER3_BASE             0x4001A000UL
  #define NRF_TIMER4_BASE             0x4001B000UL
  ...
  #define NRF_TIMER0                  ((NRF_TIMER_Type*)         NRF_TIMER0_BASE)
  #define NRF_TIMER1                  ((NRF_TIMER_Type*)         NRF_TIMER1_BASE)
  #define NRF_TIMER2                  ((NRF_TIMER_Type*)         NRF_TIMER2_BASE)
  #define NRF_TIMER3                  ((NRF_TIMER_Type*)         NRF_TIMER3_BASE)
  #define NRF_TIMER4                  ((NRF_TIMER_Type*)         NRF_TIMER4_BASE)

  ===============================================================================
  
  Defined in ./adafruit/hardware/nrf52/0.21.0/cores/nRF5/nordic/nrfx/hal/nrf_timer.h
  
  Timer prescalers
  typedef enum
  {
      NRF_TIMER_FREQ_16MHz = 0, ///< Timer frequency 16 MHz.
      NRF_TIMER_FREQ_8MHz,      ///< Timer frequency 8 MHz.
      NRF_TIMER_FREQ_4MHz,      ///< Timer frequency 4 MHz.
      NRF_TIMER_FREQ_2MHz,      ///< Timer frequency 2 MHz.
      NRF_TIMER_FREQ_1MHz,      ///< Timer frequency 1 MHz.
      NRF_TIMER_FREQ_500kHz,    ///< Timer frequency 500 kHz.
      NRF_TIMER_FREQ_250kHz,    ///< Timer frequency 250 kHz.
      NRF_TIMER_FREQ_125kHz,    ///< Timer frequency 125 kHz.
      NRF_TIMER_FREQ_62500Hz,   ///< Timer frequency 62500 Hz.
      NRF_TIMER_FREQ_31250Hz    ///< Timer frequency 31250 Hz.
  } nrf_timer_frequency_t;
  
*/
#pragma once

#ifndef NRF52_MBED_TIMERINTERRUPT_H
#define NRF52_MBED_TIMERINTERRUPT_H

#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #error This code is designed to run on nRF52-based Nano-33-BLE boards using mbed-RTOS platform! Please check your Tools->Board setting.
#endif

#include <Arduino.h>
// File ~/.arduino15/packages/arduino/hardware/mbed/1.3.0/cores/arduino/mbed/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_15_0/modules/nrfx/hal/nrf_timer.h
// This mbed-RTOS file is very old, compared to the Adafruit's nrf_timer.h
// It's better to replace with the new one later. But be careful not to chain break anything
#include "hal/nrf_timer.h"

#ifndef NRF52_MBED_TIMER_INTERRUPT_VERSION
  #define NRF52_MBED_TIMER_INTERRUPT_VERSION       "NRF52_MBED_TimerInterrupt v1.3.0"
#endif

#include "TimerInterrupt_Generic_Debug.h"

class NRF52_MBED_TimerInterrupt;

typedef NRF52_MBED_TimerInterrupt NRF52_MBED_Timer;

typedef void (*timerCallback)  (void);

typedef enum
{
  NRF_TIMER_0 = 0,
  NRF_TIMER_1 = 1,
  NRF_TIMER_2 = 2,
  NRF_TIMER_3 = 3,
  NRF_TIMER_4 = 4,
  NRF_MAX_TIMER
} NRF52_MBED_TimerNumber;

const char* NRF52_MBED_TimerName[NRF_MAX_TIMER] =
{
  "NRF_TIMER0",
  "NRF_TIMER1",
  "NRF_TIMER2-DON'T_USE_THIS",
  "NRF_TIMER3",
  "NRF_TIMER4",
};

/*
typedef enum
{
    NRF_TIMER_CC_CHANNEL0 = 0, ///< Timer capture/compare channel 0.
    NRF_TIMER_CC_CHANNEL1,     ///< Timer capture/compare channel 1.
    NRF_TIMER_CC_CHANNEL2,     ///< Timer capture/compare channel 2.
    NRF_TIMER_CC_CHANNEL3,     ///< Timer capture/compare channel 3.
#ifdef NRF52
    NRF_TIMER_CC_CHANNEL4,     ///< Timer capture/compare channel 4.
    NRF_TIMER_CC_CHANNEL5,     ///< Timer capture/compare channel 5.
#endif
} nrf_timer_cc_channel_t;
*/

///////////////////////////////////////////////////////
// Missing function in old mbed. To be removed when mbed-RTOS hal/nrf_timer.h file is updated

#ifndef NRF_STATIC_INLINE
  #ifdef NRF_DECLARE_ONLY
    #define NRF_STATIC_INLINE
  #else
    #define NRF_STATIC_INLINE __STATIC_INLINE
  #endif
#endif // NRF_STATIC_INLINE

NRF_STATIC_INLINE void nrf_timer_cc_set(NRF_TIMER_Type *       p_reg,
                                        nrf_timer_cc_channel_t cc_channel,
                                        uint32_t               cc_value)
{
    p_reg->CC[cc_channel] = cc_value;
}

NRF_STATIC_INLINE uint32_t nrf_timer_cc_get(NRF_TIMER_Type const * p_reg,
                                            nrf_timer_cc_channel_t cc_channel)
{
    return (uint32_t)p_reg->CC[cc_channel];
}
///////////////////////////////////////////////////////

class NRF52_MBED_TimerInterrupt;

NRF_TIMER_Type* nrf_timers    [NRF_MAX_TIMER] = { NRF_TIMER0, NRF_TIMER1, NRF_TIMER2, NRF_TIMER3, NRF_TIMER4 };

IRQn_Type       nrf_timers_irq[NRF_MAX_TIMER] = { TIMER0_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn, TIMER4_IRQn };

NRF52_MBED_TimerInterrupt*  nRF52Timers [NRF_MAX_TIMER] = { NULL, NULL, NULL, NULL, NULL };


class NRF52_MBED_TimerInterrupt
{
  private:
    uint8_t               _timer       = NRF_TIMER_3;
    
    NRF_TIMER_Type*        nrf_timer  = NRF_TIMER3;
    nrf_timer_cc_channel_t cc_channel = NRF_TIMER_CC_CHANNEL0;
    
    IRQn_Type              _timer_IRQ;

    timerCallback        _callback;        // pointer to the callback function
    
    // NRF_TIMER_FREQ_16MHz,NRF_TIMER_FREQ_8MHz,...,NRF_TIMER_FREQ_31250Hz
    nrf_timer_frequency_t _frequency_t = NRF_TIMER_FREQ_1MHz;
    float                 _frequency;       // Timer frequency
    uint32_t              _timerCount;      // count to activate timer

  public:

    NRF52_MBED_TimerInterrupt(uint8_t timer = NRF_TIMER_3)
    {
      // KH, force to use NRF_TIMER3 if accidentally select already used
      // NRF_TIMER0
      // NRF_TIMER2 (reserved by mbed RTOS)
      // To store to know which to delete in destructor
      if ( (timer == NRF_TIMER_0) || (timer == NRF_TIMER_1) || (timer == NRF_TIMER_2) )
        _timer = NRF_TIMER_3;
      else
        _timer = timer;
      
      nrf_timer   = nrf_timers[_timer];
      
      _timer_IRQ  = nrf_timers_irq[_timer];

      // Update to use in TIMERx_IRQHandler
      nRF52Timers[_timer] = this;
        
      _callback = NULL;
      
      // Timer mode with 32bit width
      nrf_timer_bit_width_set(nrf_timer, NRF_TIMER_BIT_WIDTH_32);
      nrf_timer_mode_set(nrf_timer, NRF_TIMER_MODE_TIMER);
      
      // KH, just 1MHz is enough. 16MHz can have better accuracy, but shorter time range.
      nrf_timer_frequency_set(nrf_timer, _frequency_t);
      
      switch (_frequency_t)
      {
      case NRF_TIMER_FREQ_16MHz:
        _frequency = 16000000.0f;
        
        break;
      case NRF_TIMER_FREQ_8MHz:
        _frequency = 8000000.0f;
        
        break;
        case NRF_TIMER_FREQ_4MHz:
        _frequency = 4000000.0f;
        
        break;
      case NRF_TIMER_FREQ_2MHz:
        _frequency = 2000000.0f;
        
        break;
      case NRF_TIMER_FREQ_1MHz:
        _frequency = 1000000.0f;
        
        break;
      default:
        _frequency = 1000000.0f;
        break;            
      } 
    };
    
    ~NRF52_MBED_TimerInterrupt()
    {
      nRF52Timers[_timer] = NULL;
    }

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to NRF52-hal-timer.c
    bool setFrequency(float frequency, timerCallback callback)
    {
      // This function will be called when time out interrupt will occur
      if (callback) 
      {
          _callback = callback;
      } 
      else 
      {
          TISR_LOGERROR(F("NRF52_MBED_TimerInterrupt: ERROR: NULL callback function pointer."));

          return false;
      }
      
      if ( (frequency <= 0) || (frequency > _frequency / 10.0f) )
      {
        TISR_LOGERROR1(F("NRF52_MBED_TimerInterrupt: ERROR: Negative or Too high frequency. Must be <="), _frequency/10.0f);
        
        return false;
      }
      
      // select timer frequency is 1MHz for better accuracy. We don't use 16-bit prescaler for now.
      // Will use later if very low frequency is needed.
      _timerCount = (uint32_t) _frequency / frequency;
      
      TISR_LOGWARN1(F("NRF52_MBED_TimerInterrupt: Timer ="), NRF52_MBED_TimerName[_timer]);
      TISR_LOGWARN3(F("Frequency ="), _frequency, F(", _count ="), (uint32_t) (_timerCount));

      // Start if not already running (and reset?)
      nrf_timer_task_trigger(nrf_timer, NRF_TIMER_TASK_START);
      nrf_timer_task_trigger(nrf_timer, NRF_TIMER_TASK_CLEAR);

      // Clear and enable compare interrupt
      nrf_timer_int_mask_t channel_mask = nrf_timer_compare_int_get(cc_channel);
      nrf_timer_int_enable(nrf_timer, channel_mask);
      
      NVIC_EnableIRQ(_timer_IRQ);
           
      //nrf_timer_cc_write(nrf_timer, cc_channel, _timerCount);
      // New for Adafruit nRF52 core 0.21.0
      nrf_timer_cc_set(nrf_timer, cc_channel, _timerCount);

      return true;
    }

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to NRF52-hal-timer.c
    bool setInterval(unsigned long interval, timerCallback callback)
    {
      return setFrequency((float) (1000000.0f / interval), callback);
    }

    bool attachInterrupt(float frequency, timerCallback callback)
    {
      return setFrequency(frequency, callback);
    }

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to NRF52-hal-timer.c
    bool attachInterruptInterval(unsigned long interval, timerCallback callback)
    {
      return setFrequency( (float) ( 1000000.0f / interval), callback);
    }

    void detachInterrupt(void)
    {
      NVIC_DisableIRQ(_timer_IRQ);
      
      // Stop timer
      nrf_timer_task_trigger(nrf_timer, NRF_TIMER_TASK_STOP);

      // Disable timer compare interrupt
      nrf_timer_int_mask_t channel_mask = nrf_timer_compare_int_get(cc_channel);
      nrf_timer_int_disable(nrf_timer, channel_mask);

      // Clear event
      nrf_timer_event_t event = nrf_timer_compare_event_get(channel_mask);
      nrf_timer_event_clear(nrf_timer, event);
    }

    void disableTimer(void)
    {
      detachInterrupt();
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt()
    {
      setFrequency(_frequency, _callback);
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer(void)
    {
      // Start if not already running (and reset?)
      nrf_timer_task_trigger(nrf_timer, NRF_TIMER_TASK_START);
      nrf_timer_task_trigger(nrf_timer, NRF_TIMER_TASK_CLEAR);

      // Clear and enable compare interrupt
      nrf_timer_int_mask_t channel_mask = nrf_timer_compare_int_get(cc_channel);
      nrf_timer_int_enable(nrf_timer, channel_mask);
      
      NVIC_EnableIRQ(_timer_IRQ);
      
      //nrf_timer_cc_write(nrf_timer, cc_channel, _timerCount);
      // New for Adafruit nRF52 core 0.21.0
      nrf_timer_cc_set(nrf_timer, cc_channel, _timerCount);
    }

    // Just stop clock source, clear the count
    void stopTimer(void)
    {
      disableTimer();
    }

    // Just reconnect clock source, start current count from 0
    void restartTimer(void)
    {
      enableTimer();
    }
    
    timerCallback getCallback(void)
    {
      return _callback;
    }
    
    IRQn_Type getTimerIRQn(void)
    {
      return _timer_IRQ;
    }
}; // class NRF52_MBED_TimerInterrupt

// Timer 0 is used by the soft device
// Timer 2 is used by the mbed-RTOS
// only Timer 1, 3 and 4 are available
extern "C" void TIMER1_IRQHandler_v(void)
{
  if (nRF52Timers[1]) 
  {
    nRF52Timers[1]->detachInterrupt();
    
    (*(nRF52Timers[1]->getCallback()))();
    
    nRF52Timers[1]->enableTimer();
  }
}

extern "C" void TIMER3_IRQHandler_v(void) 
{
  if (nRF52Timers[3]) 
  {
    nRF52Timers[3]->detachInterrupt();
    
    (*(nRF52Timers[3]->getCallback()))();
    
    nRF52Timers[3]->enableTimer();
  }
}

extern "C" void TIMER4_IRQHandler_v(void) 
{
  if (nRF52Timers[4]) 
  {
    nRF52Timers[4]->detachInterrupt();
    
    (*(nRF52Timers[4]->getCallback()))();
    
    nRF52Timers[4]->enableTimer();
  }
}

#endif    // NRF52_MBED_TIMERINTERRUPT_H
