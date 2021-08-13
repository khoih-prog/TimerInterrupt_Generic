/****************************************************************************************************************************
  ESP32TimerInterrupt.h
  For ESP32 boards
  Written by Khoi Hoang

  The ESP32 has two timer groups, each one with two general purpose hardware timers. All the timers are based on 64 bits
  counters and 16 bit prescalers. The timer counters can be configured to count up or down and support automatic reload
  and software reload. They can also generate alarms when they reach a specific value, defined by the software. The value
  of the counter can be read by the software program.

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

  Version: 1.7.0

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
*****************************************************************************************************************************/

#pragma once

#ifndef ESP32TIMERINTERRUPT_H
#define ESP32TIMERINTERRUPT_H

#ifndef ESP32
  #error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#elif ( ARDUINO_ESP32S2_DEV || ARDUINO_FEATHERS2 || ARDUINO_ESP32S2_THING_PLUS || ARDUINO_MICROS2 || \
        ARDUINO_METRO_ESP32S2 || ARDUINO_MAGTAG29_ESP32S2 || ARDUINO_FUNHOUSE_ESP32S2 || \
        ARDUINO_ADAFRUIT_FEATHER_ESP32S2_NOPSRAM )
  #error This code is not designed to run on ESP32-S2 platform! Please check your Tools->Board setting.   
#endif

#ifndef ESP32_TIMER_INTERRUPT_VERSION
  #define ESP32_TIMER_INTERRUPT_VERSION       "ESP32TimerInterrupt v1.4.0"
#endif

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

#include <esp32-hal-timer.h>

/* hw_timer_t defined in harware/espressif/esp32/cores/esp32/esp32-hal-timer.c:
  typedef struct hw_timer_s 
  {
    hw_timer_reg_t * dev;
    uint8_t num;
    uint8_t group;
    uint8_t timer;
    portMUX_TYPE lock;
  } hw_timer_t;

  static hw_timer_t hw_timer[4] = 
  {
    {(hw_timer_reg_t *)(DR_REG_TIMERGROUP0_BASE),0,0,0,portMUX_INITIALIZER_UNLOCKED},
    {(hw_timer_reg_t *)(DR_REG_TIMERGROUP0_BASE + 0x0024),1,0,1,portMUX_INITIALIZER_UNLOCKED},
    {(hw_timer_reg_t *)(DR_REG_TIMERGROUP0_BASE + 0x1000),2,1,0,portMUX_INITIALIZER_UNLOCKED},
    {(hw_timer_reg_t *)(DR_REG_TIMERGROUP0_BASE + 0x1024),3,1,1,portMUX_INITIALIZER_UNLOCKED}
  };

  typedef void (*voidFuncPtr)(void);
  static voidFuncPtr __timerInterruptHandlers[4] = {0,0,0,0};

  void IRAM_ATTR __timerISR(void * arg)
  {
    uint32_t s0 = TIMERG0.int_st_timers.val;
    uint32_t s1 = TIMERG1.int_st_timers.val;
    TIMERG0.int_clr_timers.val = s0;
    TIMERG1.int_clr_timers.val = s1;
    uint8_t status = (s1 & 3) << 2 | (s0 & 3);
    uint8_t i = 4;
    //restart the timers that should autoreload
    while(i--)
    {
        hw_timer_reg_t * dev = hw_timer[i].dev;
        if((status & (1 << i)) && dev->config.autoreload){
            dev->config.alarm_en = 1;
        }
    }
    i = 4;
    //call callbacks
    while(i--)
    {
      if(__timerInterruptHandlers[i] && (status & (1 << i)))
      {
          __timerInterruptHandlers[i]();
      }
    }
  }

*/

class ESP32TimerInterrupt;

typedef ESP32TimerInterrupt ESP32Timer;

#define MAX_ESP32_NUM_TIMERS      4

typedef void (*timer_callback)  (void);


class ESP32TimerInterrupt
{
  private:
    hw_timer_t*     _timer;
    uint8_t         _timerNo;

    timer_callback  _callback;        // pointer to the callback function
    float           _frequency;       // Timer frequency
    uint64_t        _timerCount;      // count to activate timer

  public:

    ESP32TimerInterrupt()
    {
      _timer    = NULL;
      // A wrong number
      _timerNo  = MAX_ESP32_NUM_TIMERS;
      _callback = NULL;
    };

    ESP32TimerInterrupt(uint8_t timerNo)
    {
      _timer    = NULL;
      _timerNo  = timerNo;
      _callback = NULL;
    };

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to esp32-hal-timer.c
    bool setFrequency(float frequency, timer_callback callback)
    {
      // select timer frequency is 1MHz for better accuracy. We don't use 16-bit prescaler for now.
      // Will use later if very low frequency is needed.
      _frequency  = 1000000;
      _timerCount = (uint64_t) _frequency / frequency;
      // count up

      TISR_LOGWARN3(F("ESP32TimerInterrupt: _timerNo ="), _timerNo, F(", _fre ="), _frequency);
      TISR_LOGWARN3(F("_count ="), (uint32_t) (_timerCount >> 32) , F("-"), (uint32_t) (_timerCount));

      // Clock to timer (prescaler) is F_CPU / 3 = 240MHz / 3 = 80MHz
      _timer = timerBegin(_timerNo, F_CPU / (_frequency * 3), true);
      // Interrupt on EGDE
      timerAttachInterrupt(_timer, callback, true);

      // autoreload = true to run forever
      timerAlarmWrite(_timer, _timerCount, true);

      timerAlarmEnable(_timer);

      return true;
    }

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to esp32-hal-timer.c
    bool setInterval(unsigned long interval, timer_callback callback)
    {
      return setFrequency((float) (1000000.0f / interval), callback);
    }

    bool attachInterrupt(float frequency, timer_callback callback)
    {
      return setFrequency(frequency, callback);
    }

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to esp32-hal-timer.c
    bool attachInterruptInterval(unsigned long interval, timer_callback callback)
    {
      return setFrequency( (float) ( 1000000.0f / interval), callback);
    }

    void detachInterrupt()
    {
      timerDetachInterrupt(_timer);
    }

    void disableTimer(void)
    {
      timerDetachInterrupt(_timer);
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt()
    {
      setFrequency(_frequency, _callback);
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer(void)
    {
      setFrequency(_frequency, _callback);
    }

    // Just stop clock source, clear the count
    void stopTimer(void)
    {
      timerStop(_timer);
    }

    // Just reconnect clock source, start current count from 0
    void restartTimer(void)
    {
      timerRestart(_timer);
    }

    int8_t getTimer() __attribute__((always_inline))
    {
      return _timerNo;
    };

}; // class ESP32TimerInterrupt

#endif    // ESP32TIMERINTERRUPT_H
