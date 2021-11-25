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
********************************************************************************************************************************/

#pragma once

#ifndef TIMERINTERRUPT_GENERIC_H
#define TIMERINTERRUPT_GENERIC_H

#if !defined(TIMER_INTERRUPT_GENERIC_VERSION)
  #define TIMER_INTERRUPT_GENERIC_VERSION       "TimerInterrupt_Generic v1.8.0"
#endif

#include "TimerInterrupt_Generic_Debug.h"

/////////////////////////////// AVR ///////////////////////////////

#if ( defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
      defined(__AVR_ATmega640__) || defined(__AVR_ATmega641__))
  #if defined(TIMER_INTERRUPT_USING_ATMEGA2560)
    #undef TIMER_INTERRUPT_USING_ATMEGA2560
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA2560      true
  
  #define TIMER_INTERRUPT_USING_AVR             true
  
  #define BOARD_TYPE    "Arduino AVR Mega2560/ADK"
  
  #warning Using Arduino AVR Mega, Mega640(P), Mega2560/ADK. Timer1-5 available
  
#elif ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) ) 
  #define TIMER_INTERRUPT_USING_AVR     true
  
  #define BOARD_TYPE    "Arduino AVR UNO, Nano, etc."
  
  #warning Using Aduino AVR ATMega644(P), ATMega328(P) such as UNO, Nano. Only Timer1,2 available

#elif ( defined(ARDUINO_AVR_FEATHER328P) || defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || \
      defined(ARDUINO_AVR_PROTRINKET5FTDI) || defined(ARDUINO_AVR_PROTRINKET3FTDI) )
  #define TIMER_INTERRUPT_USING_AVR     true
  
  #define BOARD_TYPE      "Adafruit AVR ATMega328(P)"
  
  #warning Using Adafruit ATMega328(P), such as AVR_FEATHER328P or AVR_METRO. Only Timer1,2 available
        
#elif ( defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_LEONARDO_ETH) || defined(ARDUINO_AVR_YUN) || defined(ARDUINO_AVR_MICRO) || \
        defined(ARDUINO_AVR_ESPLORA)  || defined(ARDUINO_AVR_LILYPAD_USB)  || defined(ARDUINO_AVR_ROBOT_CONTROL) || defined(ARDUINO_AVR_ROBOT_MOTOR) || \
        defined(ARDUINO_AVR_CIRCUITPLAY)  || defined(ARDUINO_AVR_YUNMINI) || defined(ARDUINO_AVR_INDUSTRIAL101) || defined(ARDUINO_AVR_LININO_ONE) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4       true
  
  #define TIMER_INTERRUPT_USING_AVR               true
  
  #define BOARD_TYPE    "Arduino AVR ATMega32U4"
  
  #warning Using Arduino ATMega32U4, such as Leonardo or Leonardo ETH. Only Timer1,3,4 available
    
#elif ( defined(ARDUINO_AVR_FLORA8 ) || defined(ARDUINO_AVR_FEATHER32U4) || defined(ARDUINO_AVR_CIRCUITPLAY) || defined(ARDUINO_AVR_ITSYBITSY32U4_5V) || \
        defined(ARDUINO_AVR_ITSYBITSY32U4_3V)  || defined(ARDUINO_AVR_BLUEFRUITMICRO) || defined(ARDUINO_AVR_ADAFRUIT32U4) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4       true
  
  #define TIMER_INTERRUPT_USING_AVR               true
  
  #define BOARD_TYPE    "Adafruit AVR ATMega32U4"
  
  #warning Using Adafruit ATMega32U4, such as Feather_32u4, AVR_CIRCUITPLAY, etc.. Only Timer1,3,4 available
  
#elif ( defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_MAKEYMAKEY ) || defined(ARDUINO_AVR_PROMICRO) || defined(ARDUINO_AVR_FIOV3) || \
        defined(ARDUINO_AVR_QDUINOMINI) || defined(ARDUINO_AVR_LILYPAD_ARDUINO_USB_PLUS_BOARD ) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4       true
  
  #define TIMER_INTERRUPT_USING_AVR               true 
  
  #define BOARD_TYPE    "Generic or Sparkfun AVR ATMega32U4"
  #warning Using Generic ATMega32U4, such as Sparkfun AVR_MAKEYMAKEY, AVR_PROMICRO, etc. Only Timer1,3,4 available

#elif ( defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_DIGITAL_SANDBOX ) || defined(ARDUINO_REDBOT) || defined(ARDUINO_AVR_SERIAL_7_SEGMENT) )
  #define TIMER_INTERRUPT_USING_AVR               true
  
  #define BOARD_TYPE    "Generic or Sparkfun AVR ATMega328P"
  
  #warning Using Generic ATMega328P, such as Sparkfun AVR_DIGITAL_SANDBOX, REDBOT, etc.

#elif ( defined(__AVR_ATmega128RFA1__) || defined(ARDUINO_ATMEGA128RFA1_DEV_BOARD) )
  #define TIMER_INTERRUPT_USING_AVR               true
  
  #define BOARD_TYPE    "Generic or Sparkfun AVR ATMega128RFA1"
  #warning Using Generic ATMega128RFA1, such as Sparkfun ATMEGA128RFA1_DEV_BOARD, etc.  
   
#elif ( defined(ARDUINO_AVR_GEMMA) || defined(ARDUINO_AVR_TRINKET3) || defined(ARDUINO_AVR_TRINKET5) )
  #error These AVR boards are not supported! Please check your Tools->Board setting.

/////////////////////////////// ESP8266 ///////////////////////////////

#elif ( defined(ESP8266) || ESP8266 )
  
  #define TIMER_INTERRUPT_USING_ESP8266           true

/////////////////////////////// ESP32 ///////////////////////////////
       
#elif ( defined(ESP32) || ESP32 )

  #define TIMER_INTERRUPT_USING_ESP32             true

/////////////////////////////// Nano-33-BLE ///////////////////////////////

#elif ( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #warning Using ARDUINO_NANO33BLE
  #define TIMER_INTERRUPT_USING_NANO33BLE         true

/////////////////////////////// SAMD ///////////////////////////////
  
#elif ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
     || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
     || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
     || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
     || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )

  #define TIMER_INTERRUPT_USING_SAMD              true

/////////////////////////////// NRF52 ///////////////////////////////

#elif (defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
       defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || \
       defined(NRF52840_CLUE) || defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || \
       defined(MDBT50Q_RX) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
        
  #define TIMER_INTERRUPT_USING_NRF52             true

/////////////////////////////// SAM DUE ///////////////////////////////

#elif ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) ) 
  
  #define TIMER_INTERRUPT_USING_SAMDUE            true
       
/////////////////////////////// TEENSY ///////////////////////////////

#elif ( defined(CORE_TEENSY) ||  defined(__IMXRT1062__) || defined(__MK66FX1M0__) || defined(__MK64FX512__) ||  \
        defined(__MKL26Z64__) || defined(__MK20DX256__) || defined(__MK20DX128__) )

  #define TIMER_INTERRUPT_USING_TEENSY            true

/////////////////////////////// STM32 ///////////////////////////////

#elif ( defined(STM32F0) || defined(STM32F1) ||  defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
        defined(STM32L0) || defined(STM32L1) ||  defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
        defined(STM32WB) || defined(STM32MP1) || defined(STM32L5) )
        
  #define TIMER_INTERRUPT_USING_STM32             true

#elif ( defined(__AVR_ATmega4809__) || defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_AVR_NANO_EVERY) )
       
  #define TIMER_INTERRUPT_USING_MEGA_AVR          true

#elif ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || defined(ARDUINO_GENERIC_RP2040) ) && !defined(ARDUINO_ARCH_MBED)

  #define TIMER_INTERRUPT_USING_RPI_PICO          true  
     
#elif ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
      defined(ARDUINO_GENERIC_RP2040) ) && defined(ARDUINO_ARCH_MBED)
      
  #define TIMER_INTERRUPT_USING_MBED_RPI_PICO     true 
     
#else

  #error Unsupported Board! Please check your Tools->Board setting.
  
#endif

////////////////////////////////////////////////////

#if TIMER_INTERRUPT_USING_AVR
  #include "AVRTimerInterrupt_Generic.h"
#elif TIMER_INTERRUPT_USING_ESP8266
  #include "ESP8266TimerInterrupt_Generic.h"  
#elif TIMER_INTERRUPT_USING_ESP32
  #include "ESP32TimerInterrupt_Generic.h"  
#elif TIMER_INTERRUPT_USING_NANO33BLE
  #include "NRF52_MBED_TimerInterrupt_Generic.h" 
#elif TIMER_INTERRUPT_USING_SAMD
  #include "SAMDTimerInterrupt_Generic.h"
#elif TIMER_INTERRUPT_USING_NRF52
  #include "NRF52TimerInterrupt_Generic.h" 
#elif TIMER_INTERRUPT_USING_SAMDUE
  #include "SAMDUETimerInterrupt_Generic.h" 
#elif TIMER_INTERRUPT_USING_TEENSY
  #include "TeensyTimerInterrupt_Generic.h" 
#elif TIMER_INTERRUPT_USING_STM32
  #include "STM32TimerInterrupt_Generic.h"
#elif TIMER_INTERRUPT_USING_MEGA_AVR
  #include "megaAVR_TimerInterrupt_Generic.h" 
#elif TIMER_INTERRUPT_USING_RPI_PICO
  #include "RP2040_TimerInterrupt_Generic.h" 
#elif TIMER_INTERRUPT_USING_MBED_RPI_PICO
  #include "MBED_RP2040_TimerInterrupt_Generic.h" 
#endif

////////////////////////////////////////////////////


#endif    // TIMERINTERRUPT_GENERIC_H
