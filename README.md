# SAMD_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/SAMD_TimerInterrupt.svg?)](https://www.ardu-badge.com/SAMD_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/SAMD_TimerInterrupt.svg)](https://github.com/khoih-prog/SAMD_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/SAMD_TimerInterrupt/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/SAMD_TimerInterrupt.svg)](http://github.com/khoih-prog/SAMD_TimerInterrupt/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Important Change from v1.6.0](#Important-Change-from-v160)
* [Why do we need this SAMD_TimerInterrupt library](#why-do-we-need-this-samd_timerinterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Arduino SAMD boards](#1-for-arduino-samd-boards)
      * [For core version v1.8.10+](#for-core-version-v1810)
      * [For core version v1.8.9-](#for-core-version-v189-)
  * [2. For Adafruit SAMD boards](#2-for-adafruit-samd-boards)
  * [3. For Seeeduino SAMD boards](#3-for-seeeduino-samd-boards)
* [Libraries' Patches](#libraries-patches)
  * [1. For application requiring 2K+ HTML page](#1-for-application-requiring-2k-html-page)
  * [2. For Ethernet library](#2-for-ethernet-library)
  * [3. For EthernetLarge library](#3-for-ethernetlarge-library)
  * [4. For Etherne2 library](#4-for-ethernet2-library)
  * [5. For Ethernet3 library](#5-for-ethernet3-library)
  * [6. For UIPEthernet library](#6-for-uipethernet-library)
  * [7. For fixing ESP32 compile error](#7-for-fixing-esp32-compile-error)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [New from v1.0.0](#new-from-v100)
* [Usage](#usage)
  * [1. Using only Hardware Timer directly](#1-using-only-hardware-timer-directly)
    * [1.1 Init Hardware Timer](#11-init-hardware-timer)
    * [1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function](#12-set-hardware-timer-interval-and-attach-timer-interrupt-handler-function)
  * [2. Using 16 ISR_based Timers from 1 Hardware Timer](#2-using-16-isr_based-timers-from-1-hardware-timer)
    * [2.1 Init Hardware Timer and ISR-based Timer](#21-init-hardware-timer-and-isr-based-timer)
    * [2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions](#22-set-hardware-timer-interval-and-attach-timer-interrupt-handler-functions)
* [Examples](#examples)
  * [  1. Argument_None](examples/Argument_None)
  * [  2. Argument_None_uS](examples/Argument_None_uS)
  * [  3. ISR_16_Timers_Array](examples/ISR_16_Timers_Array)
  * [  4. ISR_RPM_Measure](examples/ISR_RPM_Measure)
  * [  5. ISR_Timer_Complex_Ethernet](examples/ISR_Timer_Complex_Ethernet)
  * [  6. ISR_Timer_Complex_WiFiNINA](examples/ISR_Timer_Complex_WiFiNINA)
  * [  7. RPM_Measure](examples/RPM_Measure)
  * [  8. SwitchDebounce](examples/SwitchDebounce)
  * [  9. TimerInterruptTest](examples/TimerInterruptTest)
  * [ 10. TimerInterruptLEDDemo](examples/TimerInterruptLEDDemo)
  * [ 11. **Change_Interval**](examples/Change_Interval)
  * [ 12. **ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex)
  * [ 13. **RepeatedAttachInterrupt_uS**](examples/RepeatedAttachInterrupt_uS)
  * [ 14. **multiFileProject**](examples/multiFileProject)
  * [ 15. **SAMD21_MultiTimers**](examples/SAMD21_MultiTimers) **New**
* [Example ISR_16_Timers_Array_Complex](#example-ISR_16_Timers_Array_Complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_Timer_Complex_WiFiNINA on Arduino SAMD21 SAMD_NANO_33_IOT using WiFiNINA](#1-ISR_Timer_Complex_WiFiNINA-on-Arduino-SAMD21-SAMD_NANO_33_IOT-using-WiFiNINA)
  * [2. TimerInterruptTest on Adafruit SAMD51 ITSYBITSY_M4](#2-TimerInterruptTest-on-Adafruit-SAMD51-ITSYBITSY_M4)
  * [3. Argument_None on Arduino SAMD21 SAMD_NANO_33_IOT](#3-Argument_None-on-Arduino-SAMD21-SAMD_NANO_33_IOT)
  * [4. ISR_16_Timers_Array on Arduino SAMD21 SAMD_NANO_33_IOT](#4-ISR_16_Timers_Array-on-Arduino-SAMD21-SAMD_NANO_33_IOT)
  * [5. Change_Interval on Arduino SAMD21 SAMD_NANO_33_IOT](#5-Change_Interval-on-Arduino-SAMD21-SAMD_NANO_33_IOT)
  * [6. RepeatedAttachInterrupt_uS on SAMD21 SEEED_XIAO_M0](#6-RepeatedAttachInterrupt_uS-on-SAMD21-SEEED_XIAO_M0)
  * [7. SAMD21_MultiTimers on SAMD21 SAMD_NANO_33_IOT](#7-SAMD21_MultiTimers-on-SAMD21-SAMD_NANO_33_IOT)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)


---
---

### Important Change from v1.6.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

### Why do we need this [SAMD_TimerInterrupt library](https://github.com/khoih-prog/SAMD_TimerInterrupt)

### Features

This library enables you to use Interrupt from Hardware Timers on an SAMD-based board, such as SAMD21 Nano-33-IoT, Adafruit SAMD51 Itsy-Bitsy M4, SparkFun SAMD51 MicroMod, etc.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

  - **Arduino SAMD21 (ZERO, MKR, NANO_33_IOT, etc.)**.
  - **Adafruit SAM21 (Itsy-Bitsy M0, Metro M0, Feather M0, Gemma M0, etc.)**.
  - **Adafruit SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)**.
  - **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)** 
  - **Sparkfun SAMD21 boards** such as **SparkFun_RedBoard_Turbo, SparkFun_Qwiic_Micro, etc.**
  - **Sparkfun SAMD51 boards** such as **SparkFun_SAMD51_Thing_Plus, SparkFun_SAMD51_MicroMod, etc.**
---

### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

3. The maximum time you can use directly on SAMD TC3/TCC Timer is **~1398.11ms = 1.39811s**. If you use time larger than 1398.11ms, the wrap-around will happen and you'll get much smaller time.

---
---


## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Arduino SAMD core 1.8.13+`](https://github.com/arduino/ArduinoCore-samd) for SAMD ARM Cortex-M0+ boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-samd.svg)](https://github.com/arduino/ArduinoCore-samd/releases/latest)
 3. [`Adafruit SAMD core 1.7.10+`](https://github.com/adafruit/ArduinoCore-samd) for SAMD ARM Cortex-M0+ and M4 boards (Nano 33 IoT, etc.). [![GitHub release](https://img.shields.io/github/release/adafruit/ArduinoCore-samd.svg)](https://github.com/adafruit/ArduinoCore-samd/releases/latest)
 4. [`Seeeduino SAMD core 1.8.3+`](https://github.com/Seeed-Studio/ArduinoCore-samd) for SAMD21/SAMD51 boards (XIAO M0, Wio Terminal, etc.). [![Latest release](https://img.shields.io/github/release/Seeed-Studio/ArduinoCore-samd.svg)](https://github.com/Seeed-Studio/ArduinoCore-samd/releases/latest/)
 5. [`Sparkfun SAMD core 1.8.7+`](https://github.com/sparkfun/Arduino_Boards) for SAMD21/SAMD51 boards (SparkFun_RedBoard_Turbo, SparkFun_SAMD51_Thing_Plus, etc.).
 6. [`Blynk library 1.1.0`](https://github.com/blynkkk/blynk-library/releases). [![Latest release](https://img.shields.io/github/release/blynkkk/blynk-library.svg)](https://github.com/blynkkk/blynk-library/releases/latest/) to use with some examples. Don't use Blynk beta versions.
 7. To use with some examples, depending on which Ethernet card you're using:
   - [`Ethernet_Generic library v2.6.1+`](https://github.com/khoih-prog/Ethernet_Generic) for W5100, W5200 and W5500/WIZ550io/WIZ850io/USR-ES1 with Wiznet W5500 chip.  [![GitHub release](https://img.shields.io/github/release/khoih-prog/Ethernet_Generic.svg)](https://github.com/khoih-prog/Ethernet_Generic/releases/latest)
   - [`EthernetENC library v2.0.3+`](https://github.com/jandrassy/EthernetENC) for ENC28J60. [![GitHub release](https://img.shields.io/github/release/jandrassy/EthernetENC.svg)](https://github.com/jandrassy/EthernetENC/releases/latest). **New and Better**
   - [`UIPEthernet library v2.0.12+`](https://github.com/UIPEthernet/UIPEthernet) for ENC28J60. [![GitHub release](https://img.shields.io/github/release/UIPEthernet/UIPEthernet.svg)](https://github.com/UIPEthernet/UIPEthernet/releases/latest)
 7. To use with some examples
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_16_Timers_Array](examples/ISR_16_Timers_Array) and [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) examples. New to avoid error with `old` Sparkfun core.
   
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**SAMD_TimerInterrupt**](https://github.com/khoih-prog/SAMD_TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/SAMD_TimerInterrupt.svg?)](https://www.ardu-badge.com/SAMD_TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**SAMD_TimerInterrupt**](https://github.com/khoih-prog/SAMD_TimerInterrupt) page.
2. Download the latest release `SAMD_TimerInterrupt-master.zip`.
3. Extract the zip file to `SAMD_TimerInterrupt-master` directory 
4. Copy whole `SAMD_TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**SAMD_TimerInterrupt** library](https://registry.platformio.org/libraries/khoih-prog/SAMD_TimerInterrupt) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/SAMD_TimerInterrupt/installation). Search for **SAMD_TimerInterrupt** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Packages' Patches

#### 1. For Arduino SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Arduino SAMD (Nano-33-IoT, etc) boards***, you have to copy the whole [Arduino SAMD Packages_Patches](Packages_Patches/arduino/hardware/samd/1.8.13) directory into Arduino SAMD directory (~/.arduino15/packages/arduino/hardware/samd/1.8.13).
 
#### For core version v1.8.10+

Supposing the Arduino SAMD version is 1.8.13. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.13/platform.txt`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.zz/platform.txt`
 
#### For core version v1.8.9-

Supposing the Arduino SAMD version is 1.8.9. These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.9/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/1.8.9/cores/arduino/Arduino.h`***

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z

These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.z/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/x.yy.z/cores/arduino/Arduino.h`***
 
 This is mandatory to fix the ***notorious Arduino SAMD compiler error***. See [Improve Arduino compatibility with the STL (min and max macro)](https://github.com/arduino/ArduinoCore-samd/pull/399)
 
```
 ...\arm-none-eabi\include\c++\7.2.1\bits\stl_algobase.h:243:56: error: macro "min" passed 3 arguments, but takes just 2
     min(const _Tp& __a, const _Tp& __b, _Compare __comp)
```

Whenever the above-mentioned compiler error issue is fixed with the new Arduino SAMD release, you don't need to copy the `Arduino.h` file anymore.

#### 2. For Adafruit SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Adafruit SAMD (Itsy-Bitsy M4, etc) boards***, you have to copy the whole [Adafruit SAMD Packages_Patches](Packages_Patches/adafruit/hardware/samd/1.7.10) directory into Adafruit samd directory (~/.arduino15/packages/adafruit/hardware/samd/1.7.10). 

Supposing the Adafruit SAMD core version is 1.7.10. This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/1.7.10/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/samd/1.7.10/cores/arduino/Print.h`
- `~/.arduino15/packages/adafruit/hardware/samd/1.7.10/cores/arduino/Print.cpp`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/cores/arduino/Print.h`
- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/cores/arduino/Print.cpp`

#### 3. For Seeeduino SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Seeeduino SAMD (XIAO M0, Wio Terminal, etc) boards***, you have to copy the whole [Seeeduino SAMD Packages_Patches](Packages_Patches/Seeeduino/hardware/samd/1.8.3) directory into Seeeduino samd directory (~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3). 

Supposing the Seeeduino SAMD core version is 1.8.3. This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Arduino.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Print.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Print.cpp`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Arduino.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Print.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Print.cpp`

---
---

### Libraries' Patches

#### Notes: These patches are totally optional and necessary only when you use the related Ethernet library and get certain error or issues.

#### 1. For application requiring 2K+ HTML page

If your application requires 2K+ HTML page, the current [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) must be modified if you are using W5200/W5500 Ethernet shields. W5100 is not supported for 2K+ buffer. If you use boards requiring different CS/SS pin for W5x00 Ethernet shield, for example ESP32, ESP8266, nRF52, etc., you also have to modify the following libraries to be able to specify the CS/SS pin correctly.

#### 2. For Ethernet library

To fix [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet), just copy these following files into the [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) directory to overwrite the old files:
- [Ethernet.h](LibraryPatches/Ethernet/src/Ethernet.h)
- [Ethernet.cpp](LibraryPatches/Ethernet/src/Ethernet.cpp)
- [EthernetServer.cpp](LibraryPatches/Ethernet/src/EthernetServer.cpp)
- [w5100.h](LibraryPatches/Ethernet/src/utility/w5100.h)
- [w5100.cpp](LibraryPatches/Ethernet/src/utility/w5100.cpp)

#### 3. For EthernetLarge library

To fix [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge), just copy these following files into the [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge) directory to overwrite the old files:
- [EthernetLarge.h](LibraryPatches/EthernetLarge/src/EthernetLarge.h)
- [EthernetLarge.cpp](LibraryPatches/EthernetLarge/src/EthernetLarge.cpp)
- [EthernetServer.cpp](LibraryPatches/EthernetLarge/src/EthernetServer.cpp)
- [w5100.h](LibraryPatches/EthernetLarge/src/utility/w5100.h)
- [w5100.cpp](LibraryPatches/EthernetLarge/src/utility/w5100.cpp)


#### 4. For Ethernet2 library

To fix [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2), just copy these following files into the [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2) directory to overwrite the old files:

- [Ethernet2.h](LibraryPatches/Ethernet2/src/Ethernet2.h)
- [Ethernet2.cpp](LibraryPatches/Ethernet2/src/Ethernet2.cpp)

To add UDP Multicast support, necessary for the [**UPnP_Generic library**](https://github.com/khoih-prog/UPnP_Generic):

- [EthernetUdp2.h](LibraryPatches/Ethernet2/src/EthernetUdp2.h)
- [EthernetUdp2.cpp](LibraryPatches/Ethernet2/src/EthernetUdp2.cpp)

#### 5. For Ethernet3 library

5. To fix [`Ethernet3 library`](https://github.com/sstaub/Ethernet3), just copy these following files into the [`Ethernet3 library`](https://github.com/sstaub/Ethernet3) directory to overwrite the old files:
- [Ethernet3.h](LibraryPatches/Ethernet3/src/Ethernet3.h)
- [Ethernet3.cpp](LibraryPatches/Ethernet3/src/Ethernet3.cpp)

#### 6. For UIPEthernet library

***To be able to compile and run on nRF52 boards with ENC28J60 using UIPEthernet library***, you have to copy these following files into the UIPEthernet `utility` directory to overwrite the old files:

- [UIPEthernet.h](LibraryPatches/UIPEthernet/UIPEthernet.h)
- [UIPEthernet.cpp](LibraryPatches/UIPEthernet/UIPEthernet.cpp)
- [Enc28J60Network.h](LibraryPatches/UIPEthernet/utility/Enc28J60Network.h)
- [Enc28J60Network.cpp](LibraryPatches/UIPEthernet/utility/Enc28J60Network.cpp)

#### 7. For fixing ESP32 compile error

To fix [`ESP32 compile error`](https://github.com/espressif/arduino-esp32), just copy the following file into the [`ESP32`](https://github.com/espressif/arduino-esp32) cores/esp32 directory (e.g. ./arduino-1.8.12/hardware/espressif/cores/esp32) to overwrite the old file:
- [Server.h](LibraryPatches/esp32/cores/esp32/Server.h)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include `.hpp`

```C++
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "SAMDTimerInterrupt.hpp"     //https://github.com/khoih-prog/SAMD_TimerInterrupt

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "SAMD_ISR_Timer.hpp"         //https://github.com/khoih-prog/SAMD_TimerInterrupt
```

in many files. But be sure to use the following `.h` files **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```C++
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "SAMDTimerInterrupt.h"       //https://github.com/khoih-prog/SAMD_TimerInterrupt

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "SAMD_ISR_Timer.h"           //https://github.com/khoih-prog/SAMD_TimerInterrupt
```


---
---


## New from v1.0.0

Now with these new `16 ISR-based timers` (while consuming only **1 hardware timer**), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The `ISR_Timer_Complex` example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 
You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate


---
---

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```C++
// Select only one to be true for SAMD21. Must must be placed at the beginning before #include "SAMDTimerInterrupt.h"
#define USING_TIMER_TC3         true      // Only TC3 can be used for SAMD51
#define USING_TIMER_TC4         false     // Not to use with Servo library
#define USING_TIMER_TC5         false
#define USING_TIMER_TCC         false
#define USING_TIMER_TCC1        false
#define USING_TIMER_TCC2        false     // Don't use this, can crash on some boards

// Uncomment To test if conflict with Servo library
//#include "Servo.h"

/////////////////////////////////////////////////////////////////

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "SAMDTimerInterrupt.h"

//////////////////////////////////////////////

// TC3, TC4, TC5 max permissible TIMER_INTERVAL_MS is 1398.101 ms, larger will overflow, therefore not permitted
// Use TCC, TCC1, TCC2 for longer TIMER_INTERVAL_MS
#define TIMER_INTERVAL_MS        1000

#if USING_TIMER_TC3
  #define SELECTED_TIMER      TIMER_TC3
#elif USING_TIMER_TC4
  #define SELECTED_TIMER      TIMER_TC4
#elif USING_TIMER_TC5
  #define SELECTED_TIMER      TIMER_TC5
#elif USING_TIMER_TCC
  #define SELECTED_TIMER      TIMER_TCC
#elif USING_TIMER_TCC1
  #define SELECTED_TIMER      TIMER_TCC1
#elif USING_TIMER_TCC2
  #define SELECTED_TIMER      TIMER_TCC
#else
  #error You have to select 1 Timer  
#endif

// Init selected SAMD timer
SAMDTimer ITimer(SELECTED_TIMER);
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```C++
void TimerHandler()
{
  // Doing something here inside ISR
}

// TC3, TC4, TC5 max permissible TIMER_INTERVAL_MS is 1398.101 ms, larger will overflow, therefore not permitted
// Use TCC, TCC1, TCC2 for longer TIMER_INTERVAL_MS
#define TIMER_INTERVAL_MS        1000      // 1s = 1000ms

void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer.attachInterruptInterval_MS(TIMER_INTERVAL_MS, TimerHandler0))
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer. Select another freq. or timer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timer


#### 2.1 Init Hardware Timer and ISR-based Timer

```C++
// Select only one to be true for SAMD21. Must must be placed at the beginning before #include "SAMDTimerInterrupt.h"
#define USING_TIMER_TC3         true      // Only TC3 can be used for SAMD51
#define USING_TIMER_TC4         false     // Not to use with Servo library
#define USING_TIMER_TC5         false
#define USING_TIMER_TCC         false
#define USING_TIMER_TCC1        false
#define USING_TIMER_TCC2        false     // Don't use this, can crash on some boards

// Uncomment To test if conflict with Servo library
//#include "Servo.h"

/////////////////////////////////////////////////////////////////

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "SAMDTimerInterrupt.h"

//////////////////////////////////////////////

// TC3, TC4, TC5 max permissible TIMER_INTERVAL_MS is 1398.101 ms, larger will overflow, therefore not permitted
// Use TCC, TCC1, TCC2 for longer TIMER_INTERVAL_MS
#define TIMER_INTERVAL_MS        1000

#if USING_TIMER_TC3
  #define SELECTED_TIMER      TIMER_TC3
#elif USING_TIMER_TC4
  #define SELECTED_TIMER      TIMER_TC4
#elif USING_TIMER_TC5
  #define SELECTED_TIMER      TIMER_TC5
#elif USING_TIMER_TCC
  #define SELECTED_TIMER      TIMER_TCC
#elif USING_TIMER_TCC1
  #define SELECTED_TIMER      TIMER_TCC1
#elif USING_TIMER_TCC2
  #define SELECTED_TIMER      TIMER_TCC
#else
  #error You have to select 1 Timer  
#endif

// Init selected SAMD timer
SAMDTimer ITimer(SELECTED_TIMER);

// Init SAMD_ISR_Timer
// Each SAMD_ISR_Timer can service 16 different ISR-based timers
ISR_Timer SAMD_ISR_Timer;
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```C++
void TimerHandler(void)
{
  ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In SAMD, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR
}
  
void doingSomething5s()
{
  // Doing something here inside ISR
}

void doingSomething11s()
{
  // Doing something here inside ISR
}

void doingSomething101s()
{
  // Doing something here inside ISR
}

void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer.attachInterruptInterval_MS(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  SAMD_ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  SAMD_ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  SAMD_ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  SAMD_ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```


---
---

### Examples: 

 1. [Argument_None](examples/Argument_None)
 2. [**Argument_None_uS**](examples/Argument_None_uS).
 3. [ISR_16_Timers_Array](examples/ISR_16_Timers_Array)
 4. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
 5. [ISR_Timer_Complex_Ethernet](examples/ISR_Timer_Complex_Ethernet)
 6. [ISR_Timer_Complex_WiFiNINA](examples/ISR_Timer_Complex_WiFiNINA)
 7. [RPM_Measure](examples/RPM_Measure)
 8. [SwitchDebounce](examples/SwitchDebounce)
 9. [TimerInterruptTest](examples/TimerInterruptTest)
10. [TimerInterruptLEDDemo](examples/TimerInterruptLEDDemo)
11. [**Change_Interval**](examples/Change_Interval).
12. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).
13. [**RepeatedAttachInterrupt_uS**](examples/RepeatedAttachInterrupt_uS).
14. [**multiFileProject**](examples/multiFileProject).
15. [**SAMD21_MultiTimers**](examples/SAMD21_MultiTimers). **New**
 

---
---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

https://github.com/khoih-prog/SAMD_TimerInterrupt/blob/89522e1ada1f3267f9860e759e4f88db06e8610f/examples/ISR_16_Timers_Array_Complex/ISR_16_Timers_Array_Complex.ino#L35-L418


---
---

### Debug Terminal Output Samples

### 1. ISR_Timer_Complex_WiFiNINA on Arduino SAMD21 SAMD_NANO_33_IOT using WiFiNINA

The following is the sample terminal output when running example [ISR_Timer_Complex_WiFiNINA](examples/ISR_Timer_Complex_WiFiNINA) on **SAMD_NANO_33_IOT using Built-in WiFiNINA** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after 7.937s !!!**. Then in loop(), it's also activated **every 3s**.

```
Starting ISR_Timer_Complex_WiFiNINA on SAMD_NANO_33_IOT
SAMDTimerInterrupt v1.10.1
CPU Frequency = 48 MHz
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Starting  ITimer OK, millis() = 810
Start Blynk
[1571] WiFiNINA Firmware Version: 1.4.1
[1572] Con2:HueNet1
2s: Delta ms = 2000
[5439] ConW OK
[5439] IP:192.168.2.98
[5439] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on NANO_33_IOT

[5440] BlynkArduinoClient.connect: Connecting to account.duckdns.org:8080
[5673] Ready (ping: 6ms).
IP = 192.168.2.98
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 7937
2s: Delta ms = 2000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3004
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
11s: Delta ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3003
2s: Delta ms = 2000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3004
2s: Delta ms = 2000
11s: Delta ms = 11000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
5s: Delta ms = 5000
2s: Delta ms = 2000
```

---

### 2. TimerInterruptTest on Adafruit SAMD51 ITSYBITSY_M4

The following is the sample terminal output when running example [**TimerInterruptTest**](examples/TimerInterruptTest) on **Adafruit SAMD51 ITSYBITSY_M4** to demonstrate how to start/stop Hardware Timers.

```
Starting TimerInterruptTest on ITSYBITSY_M4
SAMDTimerInterrupt v1.10.1
CPU Frequency = 48 MHz
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 120 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 0x4101c000 , TC3 = 0x 0x4101c000
Starting  ITimer1 OK, millis() = 1820
ITimer0: millis() = 2820, delta = 1000
ITimer0: millis() = 3820, delta = 1000
ITimer0: millis() = 4820, delta = 1000
Stop ITimer0, millis() = 5001
Start ITimer0, millis() = 10002
ITimer0: millis() = 11002, delta = 1000
ITimer0: millis() = 12002, delta = 1000
ITimer0: millis() = 13002, delta = 1000
ITimer0: millis() = 14002, delta = 1000
ITimer0: millis() = 15002, delta = 1000
Stop ITimer0, millis() = 15003
Start ITimer0, millis() = 20004
ITimer0: millis() = 21004, delta = 1000
ITimer0: millis() = 22004, delta = 1000
ITimer0: millis() = 23004, delta = 1000
ITimer0: millis() = 24004, delta = 1000
ITimer0: millis() = 25004, delta = 1000
Stop ITimer0, millis() = 25005
Start ITimer0, millis() = 30006
ITimer0: millis() = 31006, delta = 1000
ITimer0: millis() = 32006, delta = 1000
ITimer0: millis() = 33006, delta = 1000
ITimer0: millis() = 34006, delta = 1000
ITimer0: millis() = 35006, delta = 1000
Stop ITimer0, millis() = 35007
Start ITimer0, millis() = 40008
ITimer0: millis() = 41008, delta = 1000
ITimer0: millis() = 42008, delta = 1000
ITimer0: millis() = 43008, delta = 1000
ITimer0: millis() = 44008, delta = 1000
ITimer0: millis() = 45008, delta = 1000
Stop ITimer0, millis() = 45009
Start ITimer0, millis() = 50010
ITimer0: millis() = 51010, delta = 1000
ITimer0: millis() = 52010, delta = 1000
ITimer0: millis() = 53010, delta = 1000
ITimer0: millis() = 54010, delta = 1000
ITimer0: millis() = 55010, delta = 1000
Stop ITimer0, millis() = 55011
Start ITimer0, millis() = 60012
ITimer0: millis() = 61012, delta = 1000
ITimer0: millis() = 62012, delta = 1000
ITimer0: millis() = 63012, delta = 1000
ITimer0: millis() = 64012, delta = 1000
ITimer0: millis() = 65012, delta = 1000
Stop ITimer0, millis() = 65013
Start ITimer0, millis() = 70014
ITimer0: millis() = 71014, delta = 1000
ITimer0: millis() = 72014, delta = 1000
ITimer0: millis() = 73014, delta = 1000
ITimer0: millis() = 74014, delta = 1000
ITimer0: millis() = 75014, delta = 1000
Stop ITimer0, millis() = 75015
Start ITimer0, millis() = 80016
ITimer0: millis() = 81016, delta = 1000
ITimer0: millis() = 82016, delta = 1000
ITimer0: millis() = 83016, delta = 1000
ITimer0: millis() = 84016, delta = 1000
ITimer0: millis() = 85016, delta = 1000
Stop ITimer0, millis() = 85017
Start ITimer0, millis() = 90018
ITimer0: millis() = 91018, delta = 1000

```

---

### 3. Argument_None on Arduino SAMD21 SAMD_NANO_33_IOT

The following is the sample terminal output when running example [**Argument_None**](examples/Argument_None) on **Arduino SAMD21 SAMD_NANO_33_IOT** to demonstrate how to start/stop Multiple Hardware Timers.

```
Starting Argument_None on SAMD_NANO_33_IOT
SAMDTimerInterrupt v1.10.1
CPU Frequency = 48 MHz
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Starting  ITimer1 OK, millis() = 910
F_CPU (MHz) = 48
TC_Timer::startTimer _Timer = 0x42002000, TCC0 = 0x42002000
Starting  ITimer1 OK, millis() = 911
ITimer0: millis() = 1410, delta = 500
ITimer0: millis() = 1910, delta = 500
ITimer0: millis() = 2410, delta = 500
ITimer0: millis() = 2910, delta = 500
ITimer1: millis() = 2911, delta = 2000
ITimer0: millis() = 3410, delta = 500
ITimer0: millis() = 3910, delta = 500
ITimer0: millis() = 4410, delta = 500
ITimer0: millis() = 4910, delta = 500
ITimer1: millis() = 4911, delta = 2000
ITimer0: millis() = 5410, delta = 500
ITimer0: millis() = 5910, delta = 500
ITimer0: millis() = 6410, delta = 500
ITimer0: millis() = 6910, delta = 500
ITimer1: millis() = 6911, delta = 2000
ITimer0: millis() = 7410, delta = 500
ITimer0: millis() = 7910, delta = 500
ITimer0: millis() = 8410, delta = 500
ITimer0: millis() = 8910, delta = 500
ITimer1: millis() = 8911, delta = 2000
ITimer0: millis() = 9410, delta = 500
ITimer0: millis() = 9910, delta = 500
ITimer0: millis() = 10410, delta = 500
ITimer0: millis() = 10910, delta = 500
ITimer1: millis() = 10911, delta = 2000
ITimer0: millis() = 11410, delta = 500
ITimer0: millis() = 11910, delta = 500
```

---

### 4. ISR_16_Timers_Array on Arduino SAMD21 SAMD_NANO_33_IOT

The following is the sample terminal output when running example [ISR_16_Timers_Array](examples/ISR_16_Timers_Array) on **Arduino SAMD21 SAMD_NANO_33_IOT** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy or blocked**. The 16 independent ISR timers are **programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!**

While software timer, **programmed for 2s, is activated after 10.000s in loop()!!!**.

In this example, 16 independent ISR Timers are used, yet utilized just one Hardware Timer. The Timer Intervals and Function Pointers are stored in arrays to facilitate the code modification.


```
Starting ISR_16_Timers_Array on SAMD_NANO_33_IOT
SAMDTimerInterrupt v1.10.1
CPU Frequency = 48 MHz
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Starting  ITimer OK, millis() = 1421
1s: Delta ms = 1000, ms = 2421
1s: Delta ms = 1000, ms = 3421
2s: Delta ms = 2000, ms = 3421
1s: Delta ms = 1000, ms = 4421
3s: Delta ms = 3000, ms = 4421
1s: Delta ms = 1000, ms = 5421
2s: Delta ms = 2000, ms = 5421
4s: Delta ms = 4000, ms = 5421
1s: Delta ms = 1000, ms = 6421
5s: Delta ms = 5000, ms = 6421
1s: Delta ms = 1000, ms = 7421
2s: Delta ms = 2000, ms = 7421
3s: Delta ms = 3000, ms = 7421
6s: Delta ms = 6000, ms = 7421
1s: Delta ms = 1000, ms = 8421
7s: Delta ms = 7000, ms = 8421
1s: Delta ms = 1000, ms = 9421
2s: Delta ms = 2000, ms = 9421
4s: Delta ms = 4000, ms = 9421
8s: Delta ms = 8000, ms = 9421
1s: Delta ms = 1000, ms = 10421
3s: Delta ms = 3000, ms = 10421
9s: Delta ms = 9000, ms = 10421
1s: Delta ms = 1000, ms = 11421
2s: Delta ms = 2000, ms = 11421
5s: Delta ms = 5000, ms = 11421
10s: Delta ms = 10000, ms = 11421
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10000
1s: Delta ms = 1000, ms = 12421
11s: Delta ms = 11000, ms = 12421
1s: Delta ms = 1000, ms = 13421
2s: Delta ms = 2000, ms = 13421
3s: Delta ms = 3000, ms = 13421
4s: Delta ms = 4000, ms = 13421
6s: Delta ms = 6000, ms = 13421
12s: Delta ms = 12000, ms = 13421
1s: Delta ms = 1000, ms = 14421
13s: Delta ms = 13000, ms = 14421
1s: Delta ms = 1000, ms = 15421
2s: Delta ms = 2000, ms = 15421
7s: Delta ms = 7000, ms = 15421
14s: Delta ms = 14000, ms = 15421
1s: Delta ms = 1000, ms = 16421
3s: Delta ms = 3000, ms = 16421
5s: Delta ms = 5000, ms = 16421
15s: Delta ms = 15000, ms = 16421
1s: Delta ms = 1000, ms = 17421
2s: Delta ms = 2000, ms = 17421
4s: Delta ms = 4000, ms = 17421
8s: Delta ms = 8000, ms = 17421
16s: Delta ms = 16000, ms = 17421
1s: Delta ms = 1000, ms = 18421
1s: Delta ms = 1000, ms = 19421
2s: Delta ms = 2000, ms = 19421
3s: Delta ms = 3000, ms = 19421
6s: Delta ms = 6000, ms = 19421
9s: Delta ms = 9000, ms = 19421
1s: Delta ms = 1000, ms = 20421
1s: Delta ms = 1000, ms = 21421
2s: Delta ms = 2000, ms = 21421
4s: Delta ms = 4000, ms = 21421
5s: Delta ms = 5000, ms = 21421
10s: Delta ms = 10000, ms = 21421
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10000
1s: Delta ms = 1000, ms = 22421
3s: Delta ms = 3000, ms = 22421
7s: Delta ms = 7000, ms = 22421
1s: Delta ms = 1000, ms = 23421
2s: Delta ms = 2000, ms = 23421
11s: Delta ms = 11000, ms = 23421
1s: Delta ms = 1000, ms = 24421
1s: Delta ms = 1000, ms = 25421
2s: Delta ms = 2000, ms = 25421
3s: Delta ms = 3000, ms = 25421
4s: Delta ms = 4000, ms = 25421
6s: Delta ms = 6000, ms = 25421
8s: Delta ms = 8000, ms = 25421
12s: Delta ms = 12000, ms = 25421
1s: Delta ms = 1000, ms = 26421
5s: Delta ms = 5000, ms = 26421
1s: Delta ms = 1000, ms = 27421
2s: Delta ms = 2000, ms = 27421
13s: Delta ms = 13000, ms = 27421
1s: Delta ms = 1000, ms = 28421
3s: Delta ms = 3000, ms = 28421
9s: Delta ms = 9000, ms = 28421
1s: Delta ms = 1000, ms = 29421
2s: Delta ms = 2000, ms = 29421
4s: Delta ms = 4000, ms = 29421
7s: Delta ms = 7000, ms = 29421
14s: Delta ms = 14000, ms = 29421
1s: Delta ms = 1000, ms = 30421
1s: Delta ms = 1000, ms = 31421
2s: Delta ms = 2000, ms = 31421
3s: Delta ms = 3000, ms = 31421
5s: Delta ms = 5000, ms = 31421
6s: Delta ms = 6000, ms = 31421
10s: Delta ms = 10000, ms = 31421
15s: Delta ms = 15000, ms = 31421
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10000
1s: Delta ms = 1000, ms = 32421
1s: Delta ms = 1000, ms = 33421
2s: Delta ms = 2000, ms = 33421
4s: Delta ms = 4000, ms = 33421
8s: Delta ms = 8000, ms = 33421
16s: Delta ms = 16000, ms = 33421
1s: Delta ms = 1000, ms = 34421
3s: Delta ms = 3000, ms = 34421
11s: Delta ms = 11000, ms = 34421
```

---

### 5. Change_Interval on Arduino SAMD21 SAMD_NANO_33_IOT

The following is the sample terminal output when running example [Change_Interval](examples/Change_Interval) on **Arduino SAMD21 SAMD_NANO_33_IOT** to demonstrate how to change Timer Interval on-the-fly

```
Starting Change_Interval on SAMD_NANO_33_IOT
SAMDTimerInterrupt v1.10.1
CPU Frequency = 48 MHz
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Starting  ITimer OK, millis() = 1131
Time = 10001, TimerCount = 17
Time = 20002, TimerCount = 37
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 1000
Time = 30003, TimerCount = 47
Time = 40004, TimerCount = 57
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 500
Time = 50005, TimerCount = 77
Time = 60006, TimerCount = 97
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 1000
Time = 70007, TimerCount = 107
Time = 80008, TimerCount = 117
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 500
Time = 90009, TimerCount = 137
Time = 100010, TimerCount = 157
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 1000
Time = 110011, TimerCount = 167
Time = 120012, TimerCount = 177
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 500
Time = 130013, TimerCount = 197
Time = 140014, TimerCount = 217
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 1000
Time = 150015, TimerCount = 227
Time = 160016, TimerCount = 237
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 500
Time = 170017, TimerCount = 257
Time = 180018, TimerCount = 277
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
Changing Interval, Timer = 1000
Time = 190019, TimerCount = 287
Time = 200020, TimerCount = 297

```

---

### 6. RepeatedAttachInterrupt_uS on SAMD21 SEEED_XIAO_M0

The following is the sample terminal output when running example [RepeatedAttachInterrupt_uS](examples/RepeatedAttachInterrupt_uS) on **SEEED_XIAO_M0** to demonstrate how to change Timer Interval on-the-fly


```
Starting RepeatedAttachInterrupt_uS on SEEED_XIAO_M0
SAMDTimerInterrupt v1.10.1
CPU Frequency = 48 MHz
[TISR] _period = 19995 , frequency = 50.01
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 19995 , _prescaler = 16
[TISR] _compareValue = 59984
myClockTimer (19995) = 19993           <========== OK===============================<=======OK
[TISR] _period = 19995 , frequency = 50.01
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 19995 , _prescaler = 16
[TISR] _compareValue = 59984
myClockTimer (19995) = 19993           <========== OK===============================<=======OK
[TISR] _period = 19995 , frequency = 50.01
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 19995 , _prescaler = 16
[TISR] _compareValue = 59984
myClockTimer (19995) = 19993           <========== OK===============================<=======OK
[TISR] _period = 19000 , frequency = 52.63
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 19000 , _prescaler = 16
[TISR] _compareValue = 56999
myClockTimer (19000) = 18998
[TISR] _period = 20005 , frequency = 49.99
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 20005 , _prescaler = 64
[TISR] _compareValue = 15002
myClockTimer (20005) = 20002
[TISR] _period = 30000 , frequency = 33.33
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 30000 , _prescaler = 64
[TISR] _compareValue = 22498
myClockTimer (30000) = 29996
[TISR] _period = 19995 , frequency = 50.01
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 19995 , _prescaler = 16
[TISR] _compareValue = 59984
myClockTimer (19995) = 19993           <========== OK===============================<=======OK
[TISR] _period = 30000 , frequency = 33.33
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 30000 , _prescaler = 64
[TISR] _compareValue = 22498
myClockTimer (30000) = 29997
[TISR] _period = 19995 , frequency = 50.01
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 19995 , _prescaler = 16
[TISR] _compareValue = 59984
```


---

### 7. SAMD21_MultiTimers on SAMD21 SAMD_NANO_33_IOT

The following is the sample terminal output when running example [SAMD21_MultiTimers](examples/SAMD21_MultiTimers) on **SAMD_NANO_33_IOT** to demonstrate demo the how to use all **6 SAMD21 timers simultaneously**.


```
Starting SAMD21_MultiTimers on SAMD_NANO_33_IOT
SAMDTimerInterrupt v1.10.1
CPU Frequency = 48 MHz
[TISR] _period = 20000.00 , frequency = 50.00
[TISR] _timerNumber = 0
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC3_Timer::startTimer _Timer = 0x 42002c00 , TC3 = 0x 42002c00
[TISR] SAMD21 TC3 period = 20000.00 , _prescaler = 16
[TISR] _compareValue = 59999
Starting  TIMER_TC3 OK, millis() = 962
[TISR] _period = 50000.00 , frequency = 20.00
[TISR] _timerNumber = 1
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC4_Timer::startTimer _Timer = 0x 42003000 , TC4 = 0x 42003000
[TISR] SAMD21 TC4 period = 50000.00 , _prescaler = 64
[TISR] _compareValue = 37499
Starting  TIMER_TC4 OK, millis() = 964
[TISR] _period = 100000.00 , frequency = 10.00
[TISR] _timerNumber = 2
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TC5_Timer::startTimer _Timer = 0x 42003400 , TC5 = 0x 42003400
[TISR] SAMD21 TC5 period = 100000.00 , _prescaler = 256
[TISR] _compareValue = 18749
Starting  TIMER_TC5 OK, millis() = 966
[TISR] _period = 200000.00 , frequency = 5.00
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002000 , TCC0 = 0x 42002000
[TISR] SAMD21 TCC period = 200000.00 , _prescaler = 256
[TISR] _compareValue = 37499
Starting  TIMER_TCC OK, millis() = 968
[TISR] _period = 500000.00 , frequency = 2.00
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002400 , TCC1 = 0x 42002400
[TISR] SAMD21 TCC1 period = 500000.00 , _prescaler = 1024
[TISR] _compareValue = 23436
Starting  TIMER_TCC1 OK, millis() = 970
[TISR] _period = 1000000.00 , frequency = 1.00
[TISR] SAMDTimerInterrupt: F_CPU (MHz) = 48 , TIMER_HZ = 48
[TISR] TCC_Timer::startTimer _Timer = 0x 42002800 , TCC2 = 0x 42002800
[TISR] SAMD21 TCC2 period = 1000000.00 , _prescaler = 1024
[TISR] _compareValue = 46874
========================================
TC3  Actual/Programmed (ms) 20/20
TC4  Actual/Programmed (ms) 50/50
TC5  Actual/Programmed (ms) 100/100
TCC  Actual/Programmed (ms) 200/200
TCC1 Actual/Programmed (ms) 500/500
========================================
TC3  Actual/Programmed (ms) 20/20
TC4  Actual/Programmed (ms) 50/50
TC5  Actual/Programmed (ms) 100/100
TCC  Actual/Programmed (ms) 200/200
TCC1 Actual/Programmed (ms) 500/500
```


---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level (_TIMERINTERRUPT_LOGLEVEL_) from 0 to 4

```C++
// These define's must be placed at the beginning before #include "SAMD_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.

---
---

### Issues

Submit issues to: [SAMD_TimerInterrupt issues](https://github.com/khoih-prog/SAMD_TimerInterrupt/issues)

---
---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards such as SAM-DUE
3. Add more Timers to SAMD51

---

## DONE


 1. Basic hardware timers for SAMD21 and SAMD51.
 2. More hardware-initiated software-enabled timers
 3. Longer time interval
 4. Similar features for remaining Arduino boards such as ESP32, ESP8266, STM32, nRF52, mbed-nRF52, Teensy, etc.
 5. Add Table of Contents
 6. Fix `multiple-definitions` linker error
 7. Add support to many more boards, such as `SAMD21E1xA`, `SAMD21G1xA` and`SAMD21J1xA`
 8. Optimize library code by using `reference-passing` instead of `value-passing`
 9. Optimize code for `setInterval()` of SAMD21 TC3
10. Reverse the change in `setInterval()` of SAMD21 TC3 to fix bug when using SAMD21 TC3.
11. Add TC4, TC5, TCC1 and TCC2 Timers to SAMD21
12. Add example [SAMD21_MultiTimers](examples/SAMD21_MultiTimers) to demo the how to use all 6 SAMD21 timers simultaneously.
13. Add functions `attachInterruptInterval_MS()` and `setInterval_MS()`
14. Avoid conflict with Servo library. Check [Cannot use TimerInterrupt_Generic Library in the same time than Servo Library #11](https://github.com/khoih-prog/TimerInterrupt_Generic/discussions/11)
15. Prevent overflow of TCx by flagging error
16. Using float instead of ulong for interval

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. Use some code from the [**Tamasa's ZeroTimer Library**](https://github.com/EHbtj/ZeroTimer).
2. Use some code from the [**Dennis van Gils' SAMD51_InterruptTimer Library**](https://github.com/Dennis-van-Gils/SAMD51_InterruptTimer).
3. Thanks to [generationmake](https://github.com/generationmake) to report the issue [Doesn't compile with Adafruit Feather M0 #2](https://github.com/khoih-prog/SAMD_TimerInterrupt/issues/2) leading to new release v1.3.1.
4. Thanks to [thiagothimotti](https://github.com/thiagothimotti) to report the **interesting hard-to-find bug** in [Bug when going from a >20000us period to a <20000us period. The timer period become 4 times greater. #3](https://github.com/khoih-prog/SAMD_TimerInterrupt/issues/3) leading to new release v1.4.0.
5. Thanks to [generationmake](https://github.com/generationmake) to make a PR in [change variable period from unsigned long to float #7](https://github.com/khoih-prog/SAMD_TimerInterrupt/pull/7) leading to new release v1.5.0.
6. Thanks to [Alexander Golovanov](https://github.com/homeodor) to propose a PR in [Add more SAMD21 #10](https://github.com/khoih-prog/SAMD_TimerInterrupt/pull/10) leading to the support of many new boards in new release v1.6.0
7. Thanks to [Will Powell](https://github.com/WillPowellUk) to report the bug in [Multiple Definition Error Not fixed by swapping src_cpp or src_h with src #9](https://github.com/khoih-prog/SAMD_TimerInterrupt/issues/9) leading to new release v1.6.0.
8. Thanks to [Dave Hooper](https://github.com/stripwax) to report the bug and propose the fix in [setInterval on a running timer results in a period significantly longer than the specified period #17](https://github.com/khoih-prog/SAMD_TimerInterrupt/issues/17) leading to new release v1.7.0
9. Thanks to [Rui Marinheiro](https://github.com/sailorsail) to start the discussion in [Do I have a brick? I'm unable to upload sketches after using this library! #21](https://github.com/khoih-prog/SAMD_TimerInterrupt/discussions/21) leading to new release v1.8.0 to fix the bug
10. Thanks to [sriccardi-invn](https://github.com/sriccardi-invn) to start the discussion in [Cannot use TimerInterrupt_Generic Library in the same time than Servo Library #11](https://github.com/khoih-prog/TimerInterrupt_Generic/discussions/11) leading to new release v1.10.0 to fix the bug and add more features

---

<table>
  <tr>
    <td align="center"><a href="https://github.com/EHbtj"><img src="https://github.com/EHbtj.png" width="100px;" alt="EHbtj"/><br /><sub><b>⭐️ Tamasa</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/Dennis-van-Gils"><img src="https://github.com/Dennis-van-Gils.png" width="100px;" alt="Dennis-van-Gils"/><br /><sub><b> Dennis van Gils</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/generationmake"><img src="https://github.com/generationmake.png" width="100px;" alt="generationmake"/><br /><sub><b>generationmake</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/thiagothimotti"><img src="https://github.com/thiagothimotti.png" width="100px;" alt="thiagothimotti"/><br /><sub><b>thiagothimotti</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/generationmake"><img src="https://github.com/generationmake.png" width="100px;" alt="generationmake"/><br /><sub><b>generationmake</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/homeodor"><img src="https://github.com/homeodor.png" width="100px;" alt="homeodor"/><br /><sub><b>Alexander Golovanov</b></sub></a><br /></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/WillPowellUk"><img src="https://github.com/WillPowellUk.png" width="100px;" alt="WillPowellUk"/><br /><sub><b>Will Powell</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/stripwax"><img src="https://github.com/stripwax.png" width="100px;" alt="stripwax"/><br /><sub><b>Dave Hooper</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/sailorsail"><img src="https://github.com/sailorsail.png" width="100px;" alt="sailorsail"/><br /><sub><b>Rui Marinheiro</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/sriccardi-invn"><img src="https://github.com/sriccardi-invn.png" width="100px;" alt="sriccardi-invn"/><br /><sub><b>sriccardi-invn</b></sub></a><br /></td>
  </tr> 
</table>

---
---

### Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/SAMD_TimerInterrupt/blob/main/LICENSE)

---

## Copyright

Copyright 2020- Khoi Hoang


