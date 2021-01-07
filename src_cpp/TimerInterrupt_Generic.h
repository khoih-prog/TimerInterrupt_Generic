/****************************************************************************************************************************
   TimerInterrupt_Generic.h
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

   Version: 1.3.2

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
   1.1.0   K Hoang      10/11/2020 Initial Super-Library coding to merge all TimerInterrupt Libraries
   1.2.0   K Hoang      12/11/2020 Add STM32_TimerInterrupt Library
   1.3.0   K Hoang      01/12/2020 Add Mbed Mano-33-BLE Library. Add support to AVR UNO, Nano, Arduino Mini, Ethernet, BT. etc.
   1.3.1   K.Hoang      09/12/2020 Add complex examples and board Version String. Fix SAMD bug.
   1.3.2   K.Hoang      06/01/2021 Fix warnings. Optimize examples to reduce memory usage
********************************************************************************************************************************/

#pragma once

#ifndef TIMERINTERRUPT_GENERIC_H
#define TIMERINTERRUPT_GENERIC_H

#if !defined(TIMER_INTERRUPT_GENERIC_VERSION)
  #define TIMER_INTERRUPT_GENERIC_VERSION       "TimerInterrupt_Generic v1.3.2"
#endif

#include "TimerInterrupt_Generic_Debug.h"

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) || defined(ARDUINO_AVR_ETHERNET) || defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT) || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED)

  #define TIMER_INTERRUPT_USING_AVR     true
  #include "AVRTimerInterrupt_Generic.h"

#elif ( defined(ESP8266) || ESP8266 )
  
  #define TIMER_INTERRUPT_USING_ESP8266     true
  #include "ESP8266TimerInterrupt_Generic.h"

#elif ( defined(ESP32) || ESP32 )

  #define TIMER_INTERRUPT_USING_ESP32     true
  #include "ESP32TimerInterrupt_Generic.h"

#elif ( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #warning Using ARDUINO_NANO33BLE
  #define TIMER_INTERRUPT_USING_NANO33BLE  true
  #include "NRF52_MBED_TimerInterrupt_Generic.h" 
  
#elif ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
     || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
     || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
     || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
     || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )

  #define TIMER_INTERRUPT_USING_SAMD     true
  #include "SAMDTimerInterrupt_Generic.h"


#elif ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
        defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
        defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
        
  #define TIMER_INTERRUPT_USING_NRF52     true
  #include "NRF52TimerInterrupt_Generic.h" 
  
#elif ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) ) 
  
  #define TIMER_INTERRUPT_USING_SAMDUE     true
  #include "SAMDUETimerInterrupt_Generic.h" 
       
       
#elif ( defined(CORE_TEENSY) ||  defined(__IMXRT1062__) || defined(__MK66FX1M0__) || defined(__MK64FX512__) ||  \
        defined(__MKL26Z64__) || defined(__MK20DX256__) || defined(__MK20DX128__) )

  #define TIMER_INTERRUPT_USING_TEENSY     true
  #include "TeensyTimerInterrupt_Generic.h" 

#elif ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )
       
  #define TIMER_INTERRUPT_USING_STM32      true
  #include "STM32TimerInterrupt_Generic.h"  
   
#else

  #error Unsupported Board! Please check your Tools->Board setting.
  
#endif

#endif    // TIMERINTERRUPT_GENERIC_H
