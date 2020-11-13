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

   Version: 1.2.0

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
   1.1.0   K Hoang      10/11/2020 Initial Super-Library coding to merge all TimerInterrupt Libraries
   1.2.0   K Hoang      12/11/2020 Add STM32_TimerInterrupt Library
*****************************************************************************************************************************/

#pragma once

#define TIMER_INTERRUPT_GENERIC_VERSION       "1.2.0"

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)

  #define TIMER_INTERRUPT_USING_AVR     true
  #include "TimerInterrupt.h"

#elif ( defined(ESP8266) || ESP8266 )
  
  #define TIMER_INTERRUPT_USING_ESP8266     true
  #include "ESP8266TimerInterrupt.h"

#elif ( defined(ESP32) || ESP32 )

  #define TIMER_INTERRUPT_USING_ESP32     true
  #include "ESP32TimerInterrupt.h"

#elif ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
     || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
     || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
     || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
     || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )

  #define TIMER_INTERRUPT_USING_SAMD     true
  #include "SAMDTimerInterrupt.h"


#elif ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
        defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
        defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
        
  #define TIMER_INTERRUPT_USING_NRF52     true
  #include "NRF52TimerInterrupt.h" 
  
#elif ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) ) 
  
  #define TIMER_INTERRUPT_USING_SAMDUE     true
  #include "SAMDUETimerInterrupt.h" 
       
       
#elif ( defined(CORE_TEENSY) ||  defined(__IMXRT1062__) || defined(__MK66FX1M0__) || defined(__MK64FX512__) ||  \
        defined(__MKL26Z64__) || defined(__MK20DX256__) || defined(__MK20DX128__) )

  #define TIMER_INTERRUPT_USING_TEENSY     true
  #include "TeensyTimerInterrupt.h" 

#elif ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )
       
  #define TIMER_INTERRUPT_USING_STM32      true
  #include "STM32TimerInterrupt.h"        
#else

  #error Unsupported Board! Please check your Tools->Board setting.
  
#endif
