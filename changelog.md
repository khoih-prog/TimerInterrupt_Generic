# TimerInterrupt_Generic Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt_Generic.svg?)](https://www.ardu-badge.com/TimerInterrupt_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/TimerInterrupt_Generic.svg)](https://github.com/khoih-prog/TimerInterrupt_Generic/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/TimerInterrupt_Generic/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/TimerInterrupt_Generic.svg)](http://github.com/khoih-prog/TimerInterrupt_Generic/issues)



<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Major Releases v1.13.0](#major-releases-v1130)
  * [Major Releases v1.12.0](#major-releases-v1120)
  * [Major Releases v1.11.0](#major-releases-v1110)
  * [Major Releases v1.10.0](#major-releases-v1100)
  * [Major Releases v1.9.0](#major-releases-v190)
  * [Major Releases v1.8.0](#major-releases-v180)
  * [Releases v1.7.0](#releases-v170)
  * [Major Releases v1.6.0](#major-releases-v160)
  * [Releases v1.5.0](#releases-v150)
  * [Major Releases v1.4.0](#major-releases-v140)
  * [Releases v1.3.2](#releases-v132)
  * [Releases v1.3.1](#releases-v131)
  * [Releases v1.3.0](#releases-v130)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.0](#releases-v110)


---
---

## Changelog

### Major Releases v1.13.0

1. Update to use latest versions of the following libraries
- [ESP32_New_TimerInterrupt](https://github.com/khoih-prog/ESP32_New_TimerInterrupt)
- [MBED_RPI_PICO_TimerInterrupt](https://github.com/khoih-prog/MBED_RPI_PICO_TimerInterrupt)
- [megaAVR_TimerInterrupt](https://github.com/khoih-prog/megaAVR_TimerInterrupt)
- [NRF52_TimerInterrupt](https://github.com/khoih-prog/NRF52_TimerInterrupt)
- [NRF52_MBED_TimerInterrupt](https://github.com/khoih-prog/NRF52_MBED_TimerInterrupt)
2. Modify ESP32 examples to avoid using `LED_BUILTIN` / `GPIO2` as it can cause crash in some boards, such as `ESP32_C3`
3. Use `allman astyle` and add `utils`
4. Update `Packages_Patches`

### Major Releases v1.12.0

1. Update to use latest versions of the following libraries
- [SAMDTimerInterrupt](https://github.com/khoih-prog/SAMDTimerInterrupt)
- [RPI_PICO_TimerInterrupt](https://github.com/khoih-prog/RPI_PICO_TimerInterrupt)
- [MBED_RPI_PICO_TimerInterrupt](https://github.com/khoih-prog/MBED_RPI_PICO_TimerInterrupt)
2. Optimize library code by using `reference-passing` instead of `value-passing`
3. Using `float` instead of `ulong` for better interval accuracy
4. Drop `src_cpp` and `src_h` directories
5. Update `Packages_Patches`


### Major Releases v1.11.0

1. Update to use latest versions of ESP32_New_TimerInterrupt Library

  - [ESP32_New_TimerInterrupt v1.4.0](https://github.com/khoih-prog/ESP32_New_TimerInterrupt)
  
  to add support to 
  - ESP32_S2 : ESP32S2 Native USB, UM FeatherS2 Neo, UM TinyS2, UM RMP, microS2, LOLIN_S2_MINI, LOLIN_S2_PICO, ADAFRUIT_FEATHER_ESP32S2, ADAFRUIT_FEATHER_ESP32S2_TFT, ATMegaZero ESP32-S2, Deneyap Mini, FRANZININHO_WIFI, FRANZININHO_WIFI_MSC
  - ESP32_S3 : UM TinyS3, UM PROS3, UM FeatherS3, ESP32_S3_USB_OTG, ESP32S3_CAM_LCD, DFROBOT_FIREBEETLE_2_ESP32S3, ADAFRUIT_FEATHER_ESP32S3_TFT
  - ESP32_C3 : LOLIN_C3_MINI, DFROBOT_BEETLE_ESP32_C3, ADAFRUIT_QTPY_ESP32C3, AirM2M_CORE_ESP32C3, XIAO_ESP32C3

### Major Releases v1.10.0

1. Update to use latest versions of ESP32_New_TimerInterrupt Library

  - [ESP32_New_TimerInterrupt v1.3.0](https://github.com/khoih-prog/ESP32_New_TimerInterrupt)

2. Update `Packages_Patches`


### Major Releases v1.9.0

1. Update to use latest versions of these TimerInterrupt Libraries

  - [ESP32_New_TimerInterrupt v1.2.0](https://github.com/khoih-prog/ESP32_New_TimerInterrupt)
  - [ESP8266TimerInterrupt v1.6.0](https://github.com/khoih-prog/ESP8266TimerInterrupt)
  - [TimerInterrupt v1.8.0](https://github.com/khoih-prog/TimerInterrupt)
  - [megaAVR_TimerInterrupt v1.6.1](https://github.com/khoih-prog/megaAVR_TimerInterrupt)
  - [STM32_TimerInterrupt v1.3.0](https://github.com/khoih-prog/STM32_TimerInterrupt)
  - [SAMD_TimerInterrupt v1.9.0](https://github.com/khoih-prog/SAMD_TimerInterrupt)
  - [NRF52_MBED_TimerInterrupt v1.4.0](https://github.com/khoih-prog/NRF52_MBED_TimerInterrupt)
  - [RPI_PICO_TimerInterrupt v1.2.0](https://github.com/khoih-prog/RPI_PICO_TimerInterrupt)
  - [MBED_RPI_PICO_TimerInterrupt v1.1.0](https://github.com/khoih-prog/MBED_RPI_PICO_TimerInterrupt)
  - [SAMDUE_TimerInterrupt v1.3.0](https://github.com/khoih-prog/SAMDUE_TimerInterrupt)
  - [NRF52_TimerInterrupt v1.4.1](https://github.com/khoih-prog/NRF52_TimerInterrupt)
  - [Teensy_TimerInterrupt v1.3.0](https://github.com/khoih-prog/Teensy_TimerInterrupt)
2. Update `Packages_Patches`

### Major Releases v1.8.0

1. Update to use latest versions of these TimerInterrupt Libraries

  - [ESP32_New_TimerInterrupt v1.0.1](https://github.com/khoih-prog/ESP32_New_TimerInterrupt)
  - [ESP8266TimerInterrupt v1.4.1](https://github.com/khoih-prog/ESP8266TimerInterrupt)
  - [TimerInterrupt v1.7.0](https://github.com/khoih-prog/TimerInterrupt)
  - [megaAVR_TimerInterrupt v1.4.0](https://github.com/khoih-prog/megaAVR_TimerInterrupt)
  - [STM32_TimerInterrupt v1.2.1](https://github.com/khoih-prog/STM32_TimerInterrupt)
  - [SAMD_TimerInterrupt v1.5.0](https://github.com/khoih-prog/SAMD_TimerInterrupt)
  - [NRF52_MBED_TimerInterrupt v1.3.0](https://github.com/khoih-prog/NRF52_MBED_TimerInterrupt)
  - [RPI_PICO_TimerInterrupt v1.1.1](https://github.com/khoih-prog/RPI_PICO_TimerInterrupt)
  - [MBED_RPI_PICO_TimerInterrupt v1.0.1](https://github.com/khoih-prog/MBED_RPI_PICO_TimerInterrupt)
  
### Releases v1.7.0

1. Add support to Adafruit nRF52 core v0.22.0+
2. Add support to nRF52 Raytac MDBT50Q_RX Dongle

**Warnings for Releases v1.6.0-** : Use [`Adafruit nRF52 v0.22.0+`](https://github.com/adafruit/Adafruit_nRF52_Arduino/releases/tag/0.22.0) will create **compiler errors**. If so, please use the [`Adafruit nRF52 v0.21.0`](https://github.com/adafruit/Adafruit_nRF52_Arduino/releases/tag/0.21.0). It's advisable to update to Releases v1.7.0+

### Major Releases v1.6.0

1. Add support to **MBED RP2040 boards** such as **Nano_RP2040_Connect, RASPBERRY_PI_PICO**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed)
2. Add support to **RP2040-based boards such as **RASPBERRY_PI_PICO, ADAFRUIT_ITSYBITSY_RP2040, ADAFRUIT_QTPY_RP2040, ADAFRUIT_STEMMAFRIEND_RP2040, ADAFRUIT_TRINKEYQT_RP2040, ADAFRUIT_MACROPAD_RP2040, SPARKFUN_PROMICRO_RP2040, etc.**, using [**Earle Philhower's arduino-pico** core](https://github.com/earlephilhower/arduino-pico)
3. Add support to **ESP32-S2** such as **Saola, AI-Thinker ESP-12K, etc.**
4. Update for many other boards
5. Add and Update examples

### Releases v1.5.0

1. Add support to **ATmega4809-based boards** such as **Arduino UNO WiFi Rev2, AVR_NANO_EVERY, etc.**

### Major Releases v1.4.0

1. Add support to **Generic or Sparkfun AVR ATmega_32U4** such as **AVR_MAKEYMAKEY, AVR_PROMICRO, etc.**
2. Add support to **Generic or Sparkfun AVR ATmega_328(P)** such as **ARDUINO_REDBOT, ARDUINO_AVR_DIGITAL_SANDBOX, etc.**
3. Add support to **Generic or Sparkfun AVR ATmega128RFA1** such as **ATMEGA128RFA1_DEV_BOARD, etc.**
4. Add support to **Adafruit AVR ATMEGA_32U4** such as **AVR_FLORA8, AVR_FEATHER32U4, etc.**
5. Add support to **Adafruit AVR ATMEGA_328(P)** such as **AVR_FEATHER328P, AVR_METRO, etc.**
6. Add support to **AVR ATMEGA_16U4, ATMEGA_32U4** such as **Leonardo, YUN, ESPLORA, etc.**
7. Add support to **Sparkfun SAMD21 boards** such as **SparkFun_RedBoard_Turbo, SparkFun_Qwiic_Micro, etc.**
8. Add support to **Sparkfun SAMD51 boards** such as **SparkFun_SAMD51_Thing_Plus, SparkFun_SAMD51_MicroMod, etc.**
9. Update examples

### Releases v1.3.2

1. Clean-up all compiler warnings possible.
2. Optimize library and examples to reduce memory usage by using Flash String whenever possible.
3. Add Table of Contents
4. Add better debug feature

### Releases v1.3.1

1. Add many complex examples.
2. Add board Version String.
3. Fix SAMD bug.


### Releases v1.3.0

1. Add [**NRF52_MBED_TimerInterrupt Library**](https://github.com/khoih-prog/NRF52_MBED_TimerInterrupt) to support **NRF52840-based board using mbed-RTOS such as Nano-33-BLE.**
2. Add support for UNO, Nano, Mini, Arduino Ethernet, Fio, BT, LilyPad, Pro, Pro Mini, NG, UNO WiFi.


### Releases v1.2.0

1. Add [**STM32_TimerInterrupt Library**](https://github.com/khoih-prog/STM32_TimerInterrupt)

### Releases v1.1.0

1. Initial Super-Library coding to merge all TimerInterrupt Libraries
   


