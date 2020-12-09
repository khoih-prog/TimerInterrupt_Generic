# TimerInterrupt_Generic Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt_Generic.svg?)](https://www.ardu-badge.com/TimerInterrupt_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/TimerInterrupt_Generic.svg)](https://github.com/khoih-prog/TimerInterrupt_Generic/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/TimerInterrupt_Generic/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/TimerInterrupt_Generic.svg)](http://github.com/khoih-prog/TimerInterrupt_Generic/issues)

---
---

## Features

This library enables you to use Interrupt from Hardware Timers on supported Arduino boards such as AVR, ESP8266, ESP32, SAMD, SAM DUE, nRF52, STM32F/L/H/G/WB/MP1, Teensy, Nano-33-BLE, etc.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

#### Notes for Teensy boards

You'd certainly experienced that if using other Hardware Timer Libraries, such as [**TimerOne**](https://github.com/PaulStoffregen/TimerOne) or [**TimerThree**](https://github.com/PaulStoffregen/TimerThree), the interval is short, in milliseconds range.

For example, Teensy 4.x, with **super-high clock frequency of 600MHz and Timer1 and Timer3 clock of 150MHz, the maximum interval / frequency is only 55922.3467 us / 17.881939 Hz**. This [**Teensy_TimerInterrupt library**](https://github.com/khoih-prog/Teensy_TimerInterrupt) will provide you up to 16 super-long (**ulong millisecs**) ISR Timers for each used Timer1 or Timer3.

For Teensy 4.x, this library will be expanded to use other available hardware timers, such as **FTM, GPT, QUAD, PIT**, in addition to current **Timer1 and Timer3**.


---

### Why do we need this Hardware Timer Interrupt?

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed on-time or not at all, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

#### Important Notes:

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

---
---

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

#### Currently Supported Boards

  - **ESP8266**
  - **ESP32**
  - **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox etc.**.
  - **Arduino SAMD21 (ZERO, MKR, NANO_33_IOT, etc.)**.
  - **Adafruit SAM21 (Itsy-Bitsy M0, Metro M0, Feather M0, Gemma M0, etc.)**.
  - **Adafruit SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)**.
  - **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)**
  - **STM32 (Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32F1, STM32F3, STM32F4, STM32H7, STM32L0, etc.)**.
  - **STM32F/L/H/G/WB/MP1 (Nucleo-64 L053R8,Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32Fx, STM32H7, STM32Lx, STM32Gx, STM32WB, STM32MP1, etc.) having 64K+ Flash program memory.**
  
  - **Teensy boards** such as :
    - **Teensy 4.1, 4.0**
    - **Teensy 3.6, 3.5, 3.2/3.1, 3.0**
    - **Teensy LC**
    - **Teensy++ 2.0 and Teensy 2.0**

  - **Arduino AVR boards** such as :
    - **Arduino Uno / Nano, Mega / Mini / Arduino Ethernet, Fio, BT, LilyPad, Pro, Pro Mini, NG, UNO Wifi, etc.**
    
  - **Arduino Nano-33-BLE**
    
---
---

## Prerequisites

 1. [`Arduino IDE 1.8.13+` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. [`Arduino AVR core 1.8.3+`](https://github.com/arduino/ArduinoCore-avr) for Arduino AVR boards. Use Arduino Board Manager to install.
 3. [`ESP32 core 1.0.4+`](https://github.com/espressif/arduino-esp32/releases) for ESP32 boards
 4. [`ESP8266 core 2.7.4+` for Arduino](https://github.com/esp8266/Arduino#installing-with-boards-manager) for ESP8266 boards. To use ESP8266 core 2.7.1+ for LittleFS.
 5. [`Teensy core 1.53+`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0) boards. **Not ready in v1.0.0.**
 6. [`Arduino SAM DUE core 1.6.12+`](https://www.arduino.cc/en/Guide/ArduinoDue) for SAM DUE ARM Cortex-M3 boards
 7. [`Arduino SAMD core 1.8.9+`](https://www.arduino.cc/en/Guide/ArduinoM0) for SAMD ARM Cortex-M0+ boards (Nano 33 IoT, etc.).
 8. [`Adafruit SAMD core 1.6.4+`](https://www.adafruit.com/) for SAMD ARM Cortex-M0+ and M4 boards (Itsy-Bitsy M4, etc.)
 9. [`Seeeduino SAMD core 1.8.1+`](https://www.seeedstudio.com/) for SAMD21/SAMD51 boards (XIAO M0, Wio Terminal, etc.)
10. [`Adafruit nRF52 v0.21.0+`](https://www.adafruit.com/) for nRF52 boards such as AdaFruit Feather nRF52840 Express, NINA_B302_ublox, etc.
11. [`Arduino Core for STM32 v1.9.0+`](https://github.com/khoih-prog/Arduino_Core_STM32) for STM32F/L/H/G/WB/MP1 boards. To install go to Arduino IDE, select Boards Manager, search for **`STM32`**
12. [`Blynk library 0.6.1+`](https://github.com/blynkkk/blynk-library) to use with certain example.
13. Depending on which Ethernet card you're using:
   - [`Ethernet library v2.0.0+`](https://www.arduino.cc/en/Reference/Ethernet) for W5100, W5200 and W5500.
   - [`Ethernet2 library v1.0.4+`](https://github.com/khoih-prog/Ethernet2) for W5500 (Deprecated, use Arduino Ethernet library).
   - [`Ethernet3 library v1.5.3+`](https://github.com/sstaub/Ethernet3) for W5500/WIZ550io/WIZ850io/USR-ES1 with Wiznet W5500 chip.
   - [`EthernetLarge library v2.0.0+`](https://github.com/OPEnSLab-OSU/EthernetLarge) for W5100, W5200 and W5500. ***Ready*** from v1.0.1.
   - [`UIPEthernet library v2.0.9+`](https://github.com/UIPEthernet/UIPEthernet) for ENC28J60.
14. [`WiFiNINA_Generic library v1.8.0+`](https://github.com/khoih-prog/WiFiNINA_Generic) to use WiFiNINA modules/shields. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiNINA_Generic.svg?)](https://www.ardu-badge.com/WiFiNINA_Generic) if using WiFiNINA for boards such as Nano 33 IoT, nRF52, Teensy, etc.
15. [`Blynk_WiFiNINA_WM library 1.0.4+`](hhttps://github.com/khoih-prog/Blynk_WiFiNINA_WM) to use with Blynk-WiFiNINA-related example. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/Blynk_WiFiNINA_WM.svg?)](https://www.ardu-badge.com/Blynk_WiFiNINA_WM)
16. To use with certain example
   - [`SimpleTimer library`](https://github.com/schinken/SimpleTimer) for [ISR_16_Timers_Array examples](examples/NRF52/ISR_16_Timers_Array).
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**TimerInterrupt_Generic**](https://github.com/khoih-prog/TimerInterrupt_Generic), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/TimerInterrupt_Generic.svg?)](https://www.ardu-badge.com/TimerInterrupt_Generic) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**TimerInterrupt_Generic**](https://github.com/khoih-prog/TimerInterrupt_Generic) page.
2. Download the latest release `TimerInterrupt_Generic-master.zip`.
3. Extract the zip file to `TimerInterrupt_Generic-master` directory 
4. Copy whole `TimerInterrupt_Generic-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**TimerInterrupt_Generic** library](https://platformio.org/lib/show/11437/TimerInterrupt_Generic) or [**TimerInterrupt_Generic** library](https://platformio.org/lib/show/11463/TimerInterrupt_Generic) by using [Library Manager](https://platformio.org/lib/show/11437/TimerInterrupt_Generic/installation). Search for **TimerInterrupt_Generic** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Packages' Patches

 1. **To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840/nRF52832 boards**, you have to copy the whole [nRF52 0.21.0](Packages_Patches/adafruit/hardware/nrf52/0.21.0) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/0.21.0). 

Supposing the Adafruit nRF52 version is 0.21.0. These files must be copied into the directory:
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.21.0/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.21.0/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.21.0/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.21.0/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.21.0/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.21.0/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/0.21.0/cores/nRF5/Udp.h`**

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`**

 2. **To be able to compile and run on Teensy boards**, you have to copy the file [Teensy boards.txt](Packages_Patches/hardware/teensy/avr/boards.txt) into Teensy hardware directory (./arduino-1.8.13/hardware/teensy/avr/boards.txt). 

Supposing the Arduino version is 1.8.13. These files must be copied into the directory:

- `./arduino-1.8.13/hardware/teensy/avr/boards.txt`
- ***`./arduino-1.8.13/hardware/teensy/avr/cores/teensy4/Stream.h`***

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `./arduino-x.yy.zz/hardware/teensy/avr/boards.txt`
- ***`./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy4/Stream.h`***

 3. **To be able to compile and run on SAM DUE boards**, you have to copy the whole [SAM DUE](Packages_Patches/arduino/hardware/sam/1.6.12) directory into Arduino sam directory (~/.arduino15/packages/arduino/hardware/sam/1.6.12). 

Supposing the Arduino SAM core version is 1.6.12. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/1.6.12/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/x.yy.zz/platform.txt`

 4. ***To be able to compile without error and automatically detect and display BOARD_NAME on Arduino SAMD (Nano-33-IoT, etc) boards***, you have to copy the whole [Arduino SAMD cores 1.8.9](Packages_Patches/arduino/hardware/samd/1.8.9) directory into Arduino SAMD directory (~/.arduino15/packages/arduino/hardware/samd/1.8.9).
 
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

 5. ***To be able to automatically detect and display BOARD_NAME on Adafruit SAMD (Itsy-Bitsy M4, etc) boards***, you have to copy the file [Adafruit SAMD platform.txt](Packages_Patches/adafruit/hardware/samd/1.6.4) into Adafruit samd directory (~/.arduino15/packages/adafruit/hardware/samd/1.6.4). 

Supposing the Adafruit SAMD core version is 1.6.4. This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/1.6.4/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/platform.txt`

 6. ***To be able to automatically detect and display BOARD_NAME on Seeeduino SAMD (XIAO M0, Wio Terminal, etc) boards***, you have to copy the files in [Seeeduino SAMD core](Packages_Patches/Seeeduino/hardware/samd/1.8.1) into Adafruit samd directory (~/.arduino15/packages/Seeeduino/hardware/samd/1.8.1). 

Supposing the Seeeduino SAMD core version is 1.8.1. This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.1/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.1/cores/arduino/Arduino.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Arduino.h`

7. **To use Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.) boards**, you have to copy the files [STM32 variant.h](Packages_Patches/STM32/hardware/stm32/1.9.0) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/1.9.0). You have to modify the files corresponding to your boards, this is just an illustration how to do.

Supposing the STM32 stm32 core version is 1.9.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/variants/NUCLEO_F767ZI/variant.h` for Nucleo-144 NUCLEO_F767ZI.
- `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/variants/NUCLEO_L053R8/variant.h` for Nucleo-64 NUCLEO_L053R8.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/NUCLEO_F767ZI/variant.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/NUCLEO_L053R8/variant.h`

---

### Optional Libraries' Patches

##### Notes: These patches are totally optional and necessary only when you use the related Ethernet library and get certain error or issues.

1. If your application requires 2K+ HTML page, the current [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) must be modified if you are using W5200/W5500 Ethernet shields. W5100 is not supported for 2K+ buffer. If you use boards requiring different CS/SS pin for W5x00 Ethernet shield, for example ESP32, ESP8266, nRF52, etc., you also have to modify the following libraries to be able to specify the CS/SS pin correctly.

2. To fix [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet), just copy these following files into the [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) directory to overwrite the old files:

- [Ethernet.h](LibraryPatches/Ethernet/src/Ethernet.h)
- [Ethernet.cpp](LibraryPatches/Ethernet/src/Ethernet.cpp)
- [EthernetServer.cpp](LibraryPatches/Ethernet/src/EthernetServer.cpp)
- [w5100.h](LibraryPatches/Ethernet/src/utility/w5100.h)
- [w5100.cpp](LibraryPatches/Ethernet/src/utility/w5100.cpp)

3. To fix [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge), just copy these following files into the [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge) directory to overwrite the old files:

- [EthernetLarge.h](LibraryPatches/EthernetLarge/src/EthernetLarge.h)
- [EthernetLarge.cpp](LibraryPatches/EthernetLarge/src/EthernetLarge.cpp)
- [EthernetServer.cpp](LibraryPatches/EthernetLarge/src/EthernetServer.cpp)
- [w5100.h](LibraryPatches/EthernetLarge/src/utility/w5100.h)
- [w5100.cpp](LibraryPatches/EthernetLarge/src/utility/w5100.cpp)

4. To fix [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2), just copy these following files into the [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2) directory to overwrite the old files:

- [Ethernet2.h](LibraryPatches/Ethernet2/src/Ethernet2.h)
- [Ethernet2.cpp](LibraryPatches/Ethernet2/src/Ethernet2.cpp)

To add UDP Multicast support, necessary for this [**UPnP_Generic library**](https://github.com/khoih-prog/UPnP_Generic):

- [EthernetUdp2.h](LibraryPatches/Ethernet2/src/EthernetUdp2.h)
- [EthernetUdp2.cpp](LibraryPatches/Ethernet2/src/EthernetUdp2.cpp)

5. To fix [`Ethernet3 library`](https://github.com/sstaub/Ethernet3), just copy these following files into the [`Ethernet3 library`](https://github.com/sstaub/Ethernet3) directory to overwrite the old files:

- [Ethernet3.h](LibraryPatches/Ethernet3/src/Ethernet3.h)
- [Ethernet3.cpp](LibraryPatches/Ethernet3/src/Ethernet3.cpp)

6. **To be able to compile and run on nRF52 boards with ENC28J60 using UIPEthernet library**, you have to copy these following files into the UIPEthernet `utility` directory to overwrite the old files:

- For [UIPEthernet v2.0.8](https://github.com/UIPEthernet/UIPEthernet)

  - [UIPEthernet.h](LibraryPatches/UIPEthernet/UIPEthernet.h)
  - [UIPEthernet.cpp](LibraryPatches/UIPEthernet/UIPEthernet.cpp)
  - [Enc28J60Network.h](LibraryPatches/UIPEthernet/utility/Enc28J60Network.h)
  - [Enc28J60Network.cpp](LibraryPatches/UIPEthernet/utility/Enc28J60Network.cpp)

- For [UIPEthernet v2.0.9](https://github.com/UIPEthernet/UIPEthernet)

  - [UIPEthernet.h](LibraryPatches/UIPEthernet-2.0.9/UIPEthernet.h)
  - [UIPEthernet.cpp](LibraryPatches/UIPEthernet-2.0.9/UIPEthernet.cpp)
  - [Enc28J60Network.h](LibraryPatches/UIPEthernet-2.0.9/utility/Enc28J60Network.h)
  - [Enc28J60Network.cpp](LibraryPatches/UIPEthernet-2.0.9/utility/Enc28J60Network.cpp)

7. To fix [`ESP32 compile error`](https://github.com/espressif/arduino-esp32), just copy the following file into the [`ESP32`](https://github.com/espressif/arduino-esp32) cores/esp32 directory (e.g. ./arduino-1.8.12/hardware/espressif/cores/esp32) to overwrite the old file:

- [Server.h](LibraryPatches/esp32/cores/esp32/Server.h)

8. Check if you need to install the UIPthernet patch [new SAMD core F3/F4 compatibility](https://github.com/UIPEthernet/UIPEthernet/commit/c6d6519a260166b722b9cee5dd1f0fb2682e6782) to avoid errors `#include HardwareSPI.h` on some SAMD boards (Nucleo-32 F303K8, etc.)

---
---

## Important Notes

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using **xyz-Impl.h instead of standard xyz.cpp**, possibly creates certain `Multiple Definitions` Linker error in certain use cases. Although it's simple to just modify several lines of code, either in the library or in the application, the library is adding 2 more source directories

1. **scr_h** for new h-only files
2. **src_cpp** for standard h/cpp files

besides the standard **src** directory.

To use the **old standard cpp** way, locate this library' directory, then just 

1. **Delete the all the files in src directory.**
2. **Copy all the files in src_cpp directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

To re-use the **new h-only** way, just 

1. **Delete the all the files in src directory.**
2. **Copy the files in src_h directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

---

### HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)

Please have a look at [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to have more detailed description and solution of the issue.

#### 1.  ESP32 has 2 ADCs, named ADC1 and ADC2

#### 2. ESP32 ADCs functions

- ADC1 controls ADC function for pins **GPIO32-GPIO39**
- ADC2 controls ADC function for pins **GPIO0, 2, 4, 12-15, 25-27**

#### 3.. ESP32 WiFi uses ADC2 for WiFi functions

Look in file [**adc_common.c**](https://github.com/espressif/esp-idf/blob/main/components/driver/adc_common.c#L61)

> In ADC2, there're two locks used for different cases:
> 1. lock shared with app and Wi-Fi:
>    ESP32:
>         When Wi-Fi using the ADC2, we assume it will never stop, so app checks the lock and returns immediately if failed.
>    ESP32S2:
>         The controller's control over the ADC is determined by the arbiter. There is no need to control by lock.
> 
> 2. lock shared between tasks:
>    when several tasks sharing the ADC2, we want to guarantee
>    all the requests will be handled.
>    Since conversions are short (about 31us), app returns the lock very soon,
>    we use a spinlock to stand there waiting to do conversions one by one.
> 
> adc2_spinlock should be acquired first, then adc2_wifi_lock or rtc_spinlock.


- In order to use ADC2 for other functions, we have to **acquire complicated firmware locks and very difficult to do**
- So, it's not advisable to use ADC2 with WiFi/BlueTooth (BT/BLE).
- Use ADC1, and pins GPIO32-GPIO39
- If somehow it's a must to use those pins serviced by ADC2 (**GPIO0, 2, 4, 12, 13, 14, 15, 25, 26 and 27**), use the **fix mentioned at the end** of [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to work with ESP32 WiFi/BlueTooth (BT/BLE).

---

### HOWTO Use PWM analogWrite() with ESP8266 running Timer1 Interrupt

Please have a look at [ESP8266TimerInterrupt Issue 8: **ESP8266Timer and PWM --> wdt reset**](https://github.com/khoih-prog/ESP8266TimerInterrupt/issues/8) to have more detailed description and solution of the issue.

#### 1. ESP8266 has only 2 hardware timers, named Timer0 and Timer1

#### 2. ESP8266 hardware timers' functions

- Timer0 has been used for WiFi and it's not advisable to use while using WiFi (if not using WiFi, why select ESP8266 ??)
- Timer1 is used by this [**ESP8266TimerInterrupt Library**](https://github.com/khoih-prog/ESP8266TimerInterrupt)

#### 3. How to use PWM analogWrite() functions while using this library

  1. If possible, use software timer instead of [**ESP8266TimerInterrupt Hardware Timer1**](https://github.com/khoih-prog/ESP8266TimerInterrupt) 
  2. If using [**ESP8266TimerInterrupt Hardware Timer1**](https://github.com/khoih-prog/ESP8266TimerInterrupt) is a must, you can either
  - use external DAC such as AD5662, AD5667, AD5696.
  - use software PWM such as mentioned in [ESP8266 PWM REVISITED (AND REIMPLEMENTED)](https://lurchi.wordpress.com/2016/06/29/esp8266-pwm-revisited-and-reimplemented/)
  

---
---

## More useful Information


### 1. For ESP32

The ESP32 has two timer groups, each one with two general purpose hardware timers.  All the timers are based on 64 bits counters and 16 bit prescalers. 

The timer counters can be configured to count up or down and support automatic reload and software reload.

They can also generate alarms when they reach a specific value, defined by the software. The value of the counter can be read by 
the software program.

---

### 2. Notes for ESP8266

The ESP8266 timers are **badly designed**, using only 23-bit counter along with maximum 256 prescaler. They're only better than UNO / Mega.

The ESP8266 has two hardware timers, but timer0 has been used for WiFi and it's not advisable to use. Only timer1 is available.

The timer1's 23-bit counter terribly can count only up to 8,388,607. So the timer1 maximum interval is very short. Using 256 prescaler, maximum timer1 interval is only **26.843542 seconds !!!**

The timer1 counters can be configured to support automatic reload.

---

### 3. For Arduino AVR

From [Arduino 101: Timers and Interrupts](https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072)

#### 1. Timer0:

Timer0 is a 8-bit timer.

In the Arduino world timer0 is been used for the timer functions, like delay(), millis() and micros(). If you change Timer0 registers, this may influence the Arduino timer function. So you should know what you are doing.

#### 2. Timer1:

Timer1 is a 16-bit timer.
In the Arduino world the Servo library uses timer1 on Arduino Uno (Timer5 on Arduino Mega).

#### 3. Timer2:

Timer2 is a 8-bit timer like Timer0.
In the Arduino work the tone() function uses Timer2.

#### 4. Timer3, Timer4, Timer5:

Timer 3,4,5 are only available on Arduino Mega boards. These timers are all 16-bit timers.

---

### 4. For STM32F/L/H/G/WB/MP1

The Timers of STM32s are numerous, yet very sophisticated and powerful.

In general, across the STM32 microcontrollers families, the timer peripherals that have the same name also have the same features set, but there are a few exceptions. 

For example, the **TIM1** timer peripheral is shared across the STM32F1 Series, STM32F2 Series and STM32F4 Series, but for the specific case of STM32F30x microcontrollers family, the TIM1 timer peripheral features a bit richer features set than the TIM1 present in the other families.

The general purpose timers embedded by the STM32 microcontrollers share the same backbone structure; they differ only on the level of features embedded by a given timer peripheral. 

The level of features integration for a given timer peripheral is decided based on the applications field that it targets.

The timer peripherals can be classified as:
• Advanced-configuration timers like TIM1 and TIM8 among others.
• General-purpose configuration timers like TIM2 and TIM3 among others
• Lite-configuration timers like TIM9, TIM10, TIM12 and TIM16 among others
• Basic-configuration timers like TIM6 and TIM7 among others.

For example, **STM32F103C8T6** has one advance timer, while **STM32F103VET6** has two advanced timers. **Nucleo-144 STM32F767ZI boards have 14 Timers, TIM1-TIM14**.


<p align="center">
    <img src="https://github.com/khoih-prog/STM32_TimerInterrupt/blob/main/pics/STM32Timers.png">
</p>


More information can be found at [**Embedded-Lab STM32 TIMERS**](http://embedded-lab.com/blog/stm32-timers/)

To be sure which Timer is available for the board you're using, check the Core Package's related files. For example, for **Nucleo-144 STM32F767ZI**, check these files:
1. `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system/Drivers/CMSIS/Device/ST/STM32F7xx/Include/stm32f7xx.h`
2. `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system/Drivers/CMSIS/Device/ST/STM32F7xx/Include/stm32f767xx.h`

The information will be as follows:

```
typedef struct
{
  __IO uint32_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
  __IO uint32_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
  __IO uint32_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
  __IO uint32_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
  __IO uint32_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
  __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  __IO uint32_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
  __IO uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
  __IO uint32_t CCR5;        /*!< TIM capture/compare mode register5,       Address offset: 0x58 */
  __IO uint32_t CCR6;        /*!< TIM capture/compare mode register6,       Address offset: 0x5C */
  __IO uint32_t AF1;         /*!< TIM Alternate function option register 1, Address offset: 0x60 */
  __IO uint32_t AF2;         /*!< TIM Alternate function option register 2, Address offset: 0x64 */

} TIM_TypeDef;
```

and

```
#define PERIPH_BASE            0x40000000UL /*!< Base address of : AHB/ABP Peripherals   
/*!< Peripheral memory map */
#define APB1PERIPH_BASE        PERIPH_BASE

/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00UL)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400UL)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800UL)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00UL)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000UL)

/*!< APB2 peripherals */
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000UL)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400UL)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)

...

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
...
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
...
#define TIM9                ((TIM_TypeDef *) TIM9_BASE)
#define TIM10               ((TIM_TypeDef *) TIM10_BASE)
#define TIM11               ((TIM_TypeDef *) TIM11_BASE)

```

---
---

## New from v1.1.0

Now with these new `16 ISR-based timers` (while consuming only **1 hardware timer**), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_16_Timers_Array**](examples/SAMD/ISR_16_Timers_Array) and [**ISR_Timer_Complex_Ethernet**](examples/SAMDUE/ISR_Timer_Complex_Ethernet) examples will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 
You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

#### Currently Supported Boards

  - **ESP8266**
  - **ESP32**
  - **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox etc.**.
  - **Arduino SAMD21 (ZERO, MKR, NANO_33_IOT, etc.)**.
  - **Adafruit SAM21 (Itsy-Bitsy M0, Metro M0, Feather M0, Gemma M0, etc.)**.
  - **Adafruit SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)**.
  - **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)**
  - **STM32 (Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32F1, STM32F3, STM32F4, STM32H7, STM32L0, etc.)**.
  - **STM32F/L/H/G/WB/MP1 (Nucleo-64 L053R8,Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32Fx, STM32H7, STM32Lx, STM32Gx, STM32WB, STM32MP1, etc.) having 64K+ Flash program memory.**
  - **Teensy boards** such as :
    - **Teensy 4.1, 4.0**
    - **Teensy 3.6, 3.5, 3.2/3.1, 3.0**
    - **Teensy LC**
    - **Teensy++ 2.0 and Teensy 2.0**

---
---

## Usage for ESP32

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

`Timer0, Timer1, Timer2 and Timer3` are supported for ESP32.

---

## Usage for NRF52

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER1
NRF52Timer ITimer(NRF_TIMER_1);
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```
void TimerHandler(void)
{
  // Doing something here inside ISR
}

#define TIMER_INTERVAL_MS        1000      // 1s = 1000ms
void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer. Select another freq. or timer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timers


#### 2.1 Init Hardware Timer and ISR-based Timer

```
/// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER2
NRF52Timer ITimer(NRF_TIMER_2);

// Init NRF52_ISR_Timer
// Each NRF52_ISR_Timer can service 16 different ISR-based timers
ISR_Timer NRF52_ISR_Timer;
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler(void)
{
  NRF52_ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In NRF52, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
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
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  NRF52_ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  NRF52_ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  NRF52_ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  NRF52_ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---

## Usage for SAMD

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
// Depending on the board, you can select SAMD21 Hardware Timer from TC3-TCC
// SAMD21 Hardware Timer from TC3 or TCC
// SAMD51 Hardware Timer only TC3
SAMDTimer ITimer0(TIMER_TC3);
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```
void TimerHandler0(void)
{
  // Doing something here inside ISR
}

#define TIMER0_INTERVAL_MS        1000      // 1s = 1000ms
void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timers


#### 2.1 Init Hardware Timer and ISR-based Timer

```
// Depending on the board, you can select SAMD21 Hardware Timer from TC3-TCC
// SAMD21 Hardware Timer from TC3 or TCC
// SAMD51 Hardware Timer only TC3
SAMDTimer ITimer0(TIMER_TC3);

// Init SAMD_ISR_Timer
// Each SAMD_ISR_Timer can service 16 different ISR-based timers
ISR_Timer SAMD_ISR_Timer;
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler(void)
{
  SAMD_ISR_Timer.run();
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
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
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

## Usage for SAM DUE

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
// Interval in microsecs
attachDueInterrupt(TIMER1_INTERVAL_MS * 1000, TimerHandler1, "ITimer1");
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```
void TimerHandler(void)
{
  // Doing something here inside ISR
}

#define TIMER_INTERVAL_MS        1000      // 1s = 1000ms

uint16_t attachDueInterrupt(double microseconds, timerCallback callback, const char* TimerName)
{
  DueTimerInterrupt dueTimerInterrupt = DueTimer.getAvailable();
  
  dueTimerInterrupt.attachInterruptInterval(microseconds, callback);

  uint16_t timerNumber = dueTimerInterrupt.getTimerNumber();
  
  Serial.print(TimerName);
  Serial.print(" attached to Timer(");
  Serial.print(timerNumber);
  Serial.println(")");

  return timerNumber;
}

void setup()
{
  ....
  
  // Interval in microsecs
  attachDueInterrupt(TIMER_INTERVAL_MS * 1000, TimerHandler, "ITimer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timers


#### 2.1 Init Hardware Timer and ISR-based Timer

```
// Interval in microsecs
attachDueInterrupt(TIMER1_INTERVAL_MS * 1000, TimerHandler1, "ITimer1");
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
// Init SAMDUE_ISR_Timer
// Each SAMDUE_ISR_Timer can service 16 different ISR-based timers
ISR_Timer SAMDUE_ISR_Timer;

void TimerHandler(void)
{
  SAMDUE_ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_MS          1L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In SAM DUE, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
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

uint16_t attachDueInterrupt(double microseconds, timerCallback callback, const char* TimerName)
{
  DueTimerInterrupt dueTimerInterrupt = DueTimer.getAvailable();
  
  dueTimerInterrupt.attachInterruptInterval(microseconds, callback);

  uint16_t timerNumber = dueTimerInterrupt.getTimerNumber();
  
  Serial.print(TimerName);
  Serial.print(" attached to Timer(");
  Serial.print(timerNumber);
  Serial.println(")");

  return timerNumber;
}

void setup()
{
  ....
  
  // Interval in microsecs
  attachDueInterrupt(HW_TIMER_INTERVAL_MS * 1000, TimerHandler, "ITimer");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  SAMDUE_ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  SAMDUE_ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  SAMDUE_ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  SAMDUE_ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---

## Usage for Teensy

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
// You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

// Init Teensy timer TEENSY_TIMER_1
TeensyTimer ITimer0(TEENSY_TIMER_1);
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```
void TimerHandler0(void)
{
  // Doing something here inside ISR
}

// For Teensy 4.0/4.1, F_BUS_ACTUAL = 150 MHz => max period is only 55922 us (~17.9 Hz)
#define TIMER0_INTERVAL_MS        50L

void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timers


#### 2.1 Init Hardware Timer and ISR-based Timer

```
// You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

// Init Teensy timer TEENSY_TIMER_1
TeensyTimer ITimer(TEENSY_TIMER_1);

// Init Teensy_ISR_Timer
// Each Teensy_ISR_Timer can service 16 different ISR-based timers
ISR_Timer Teensy_ISR_Timer;
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler(void)
{
  Teensy_ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In Teensy, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR every 2 seconds
}
  
void doingSomething5s()
{
  // Doing something here inside ISR every 5 seconds
}

void doingSomething11s()
{
  // Doing something here inside ISR  every 11 seconds
}

void doingSomething101s()
{
  // Doing something here inside ISR every 101 seconds
}

void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  Teensy_ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---

## Usage for STM32F/L/H/G/WB/MP1

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
// Init STM32 timer TIM1
STM32Timer ITimer0(TIM1);
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```
void TimerHandler0(void)
{
  // Doing something here inside ISR
}

#define TIMER0_INTERVAL_MS        1000      // 1s = 1000ms
void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timers


#### 2.1 Init Hardware Timer and ISR-based Timer

```
// Init STM32 timer TIM1
STM32Timer ITimer(TIM1);

// Init STM32_ISR_Timer
// Each STM32_ISR_Timer can service 16 different ISR-based timers
ISR_Timer STM32_ISR_Timer;
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler(void)
{
  STM32_ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_US          100L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
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
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  STM32_ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  STM32_ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  STM32_ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  STM32_ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---

## Usage for NRF52840-based board using mbed-RTOS such as Nano-33-BLE.

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER1
NRF52_MBED_Timer ITimer(NRF_TIMER_1);
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```
void TimerHandler(void)
{
  // Doing something here inside ISR
}

#define TIMER_INTERVAL_MS        1000      // 1s = 1000ms
void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer. Select another freq. or timer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timers


#### 2.1 Init Hardware Timer and ISR-based Timer

```
/// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER2
NRF52_MBED_Timer ITimer(NRF_TIMER_2);

// Init NRF52_ISR_Timer
// Each NRF52_ISR_Timer can service 16 different ISR-based timers
ISR_Timer NRF52_ISR_Timer;
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler(void)
{
  NRF52_ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In NRF52, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
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
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  NRF52_ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  NRF52_ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  NRF52_ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  NRF52_ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---

## Usage for Arduino AVR

Before using any Timer, you have to make sure the **Timer has not been used by any other purpose.**

Only **Timer1 and Timer2** are supported for Nano, UNO, etc. boards possessing 3 timers.

**Timer3, Timer4 and Timer5** are only available for Arduino Mega boards.

---
---

## Examples: 


### 1. ESP32

 1. [Argument_None](examples/ESP32/Argument_None)
 2. [ISR_RPM_Measure](examples/ESP32/ISR_RPM_Measure)
 3. [ISR_Switch](examples/ESP32/ISR_Switch) 
 4. [ISR_Timer_4_Switches](examples/ESP32/ISR_Timer_4_Switches) 
 5. [ISR_Timer_Complex](examples/ESP32/ISR_Timer_Complex)
 6. [ISR_Timer_Switch](examples/ESP32/ISR_Timer_Switch)
 7. [ISR_Timer_Switches](examples/ESP32/ISR_Timer_Switches) 
 8. [RPM_Measure](examples/ESP32/RPM_Measure)
 9. [SwitchDebounce](examples/ESP32/SwitchDebounce)
10. [TimerInterruptTest](examples/ESP32/TimerInterruptTest)
11. [**Change_Interval**](examples/ESP32/Change_Interval). New.

### 2. ESP8266

 1. [Argument_None](examples/ESP8266/Argument_None)
 2. [ISR_RPM_Measure](examples/ESP8266/ISR_RPM_Measure)
 3. [ISR_Switch](examples/ESP8266/ISR_Switch) 
 4. [ISR_Timer_4_Switches](examples/ESP8266/ISR_Timer_4_Switches) 
 5. [ISR_Timer_Complex](examples/ESP8266/ISR_Timer_Complex)
 6. [ISR_Timer_Switch](examples/ESP8266/ISR_Timer_Switch)
 7. [ISR_Timer_Switches](examples/ESP8266/ISR_Timer_Switches) 
 8. [RPM_Measure](examples/ESP8266/RPM_Measure)
 9. [SwitchDebounce](examples/ESP8266/SwitchDebounce)
10. [TimerInterruptTest](examples/ESP8266/TimerInterruptTest)
11. [**Change_Interval**](examples/ESP8266/Change_Interval)


### 3. NRF52

 1. [Argument_None](examples/NRF52/Argument_None)
 2. [ISR_16_Timers_Array](examples/NRF52/ISR_16_Timers_Array)
 3. [ISR_RPM_Measure](examples/NRF52/ISR_RPM_Measure)
 4. [ISR_Timer_Complex_Ethernet](examples/NRF52/ISR_Timer_Complex_Ethernet)
 5. [ISR_Timer_Complex_WiFiNINA](examples/NRF52/ISR_Timer_Complex_WiFiNINA)
 6. [RPM_Measure](examples/NRF52/RPM_Measure)
 7. [SwitchDebounce](examples/NRF52/SwitchDebounce)
 8. [TimerInterruptLEDDemo](examples/NRF52/TimerInterruptLEDDemo). 
 9. [TimerInterruptTest](examples/NRF52/TimerInterruptTest)
10. [**ISR_16_Timers_Array_Complex**](examples/NRF52/ISR_16_Timers_Array_Complex). New.
11. [**Change_Interval**](examples/NRF52/Change_Interval). New.
12. [**FakeAnalogWrite**](examples/NRF52/FakeAnalogWrite). New.
 
 
### 4. SAMD21/SAMD51

 1. [Argument_None](examples/SAMD/Argument_None)
 2. [ISR_16_Timers_Array](examples/SAMD/ISR_16_Timers_Array)
 3. [ISR_RPM_Measure](examples/SAMD/ISR_RPM_Measure)
 4. [ISR_Timer_Complex_Ethernet](examples/SAMD/ISR_Timer_Complex_Ethernet)
 5. [ISR_Timer_Complex_WiFiNINA](examples/SAMD/ISR_Timer_Complex_WiFiNINA)
 6. [RPM_Measure](examples/SAMD/RPM_Measure)
 7. [SwitchDebounce](examples/SAMD/SwitchDebounce)
 8. [TimerInterruptTest](examples/SAMD/TimerInterruptTest)
 9. [TimerInterruptLEDDemo](examples/SAMD/TimerInterruptLEDDemo)
10. [**ISR_16_Timers_Array_Complex**](examples/SAMD/ISR_16_Timers_Array_Complex). New.
11. [**Change_Interval**](examples/SAMD/Change_Interval). New.

### 5. SAM DUE

 1. [Argument_None](examples/SAMDUE/Argument_None)
 2. [ISR_16_Timers_Array](examples/SAMDUE/ISR_16_Timers_Array)
 3. [ISR_RPM_Measure](examples/SAMDUE/ISR_RPM_Measure)
 4. [ISR_Timer_Complex_Ethernet](examples/SAMDUE/ISR_Timer_Complex_Ethernet)
 5. [RPM_Measure](examples/SAMDUE/RPM_Measure)
 6. [SwitchDebounce](examples/SAMDUE/SwitchDebounce)
 7. [TimerInterruptTest](examples/SAMDUE/TimerInterruptTest)
 8. [TimerInterruptLEDDemo](examples/SAMDUE/TimerInterruptLEDDemo)
 9. [**ISR_16_Timers_Array_Complex**](examples/SAMDUE/ISR_16_Timers_Array_Complex). New.
10. [**Change_Interval**](examples/SAMDUE/Change_Interval). New.

### 6. STM32F/L/H/G/WB/MP1

 1. [Argument_None](examples/STM32/Argument_None)
 2. [ISR_16_Timers_Array](examples/STM32/ISR_16_Timers_Array)
 3. [ISR_RPM_Measure](examples/STM32/ISR_RPM_Measure)
 4. [ISR_Timer_Complex](examples/STM32/ISR_Timer_Complex)
 5. [RPM_Measure](examples/STM32/RPM_Measure)
 6. [SwitchDebounce](examples/STM32/SwitchDebounce)
 7. [TimerInterruptTest](examples/STM32/TimerInterruptTest)
 8. [TimerInterruptLEDDemo](examples/STM32/TimerInterruptLEDDemo)
 9. [**ISR_16_Timers_Array_Complex**](examples/STM32/ISR_16_Timers_Array_Complex). New.
10. [**Change_Interval**](examples/STM32/Change_Interval). New.
 
### 7. Teensy

 1. [Argument_None](examples/TEENSY/Argument_None)
 2. [ISR_16_Timers_Array](examples/TEENSY/ISR_16_Timers_Array)
 3. [ISR_RPM_Measure](examples/TEENSY/ISR_RPM_Measure)
 4. [ISR_Timer_Complex](examples/TEENSY/ISR_Timer_Complex)
 5. [RPM_Measure](examples/TEENSY/RPM_Measure)
 6. [SwitchDebounce](examples/TEENSY/SwitchDebounce)
 7. [TimerInterruptTest](examples/TEENSY/TimerInterruptTest)
 8. [TimerInterruptLEDDemo](examples/TEENSY/TimerInterruptLEDDemo)
 9. [**ISR_16_Timers_Array_Complex**](examples/TEENSY/ISR_16_Timers_Array_Complex). New.
10. [**Change_Interval**](examples/TEENSY/Change_Interval). New.

### 8. Arduino AVR

 1. [Argument_Complex](examples/AVR/Argument_Complex)
 2. [Argument_None](examples/AVR/Argument_None)
 3. [Argument_Simple](examples/AVR/Argument_Simple) 
 4. [ISR_RPM_Measure](examples/AVR/ISR_RPM_Measure) 
 5. [ISR_Switch](examples/AVR/ISR_Switch)
 6. [ISR_Timer_Complex](examples/AVR/ISR_Timer_Complex)
 7. [ISR_Timer_Switch](examples/AVR/ISR_Timer_Switch)
 8. [ISR_Timer_Switches](examples/AVR/ISR_Timer_Switches) 
 9. [RPM_Measure](examples/AVR/RPM_Measure)
10. [SwitchDebounce](examples/AVR/SwitchDebounce)
11. [TimerDuration](examples/AVR/TimerDuration)
12. [TimerInterruptTest](examples/AVR/TimerInterruptTest)
13. [**ISR_16_Timers_Array_Complex**](examples/AVR/ISR_16_Timers_Array_Complex). New
14. [**ISR_Timers_Array_Simple**](examples/AVR/TimerInterruptTest). New.
15. [**Change_Interval**](examples/AVR/Change_Interval). New.

### 9. Nano-33-BLE. New

 1. [Argument_None](examples/NANO33BLE/Argument_None)
 2. [ISR_16_Timers_Array](examples/NANO33BLE/ISR_16_Timers_Array) 
 3. [ISR_16_Timers_Array_Complex](examples/NANO33BLE/ISR_16_Timers_Array_Complex) 
 4. [SwitchDebounce](examples/NANO33BLE/SwitchDebounce)
 5. [TimerInterruptLEDDemo](examples/NANO33BLE/TimerInterruptLEDDemo)
 6. [TimerInterruptTest](examples/NANO33BLE/TimerInterruptTest)
 7. [**FakeAnalogWrite**](examples/NANO33BLE/FakeAnalogWrite). New.
 8. [**Change_Interval**](examples/NANO33BLE/Change_Interval). New.

---
---

### Example [**ISR_16_Timers_Array_Complex**](examples/TEENSY/ISR_16_Timers_Array_Complex) for Teensy boards

```
#if !( defined(CORE_TEENSY) || defined(TEENSYDUINO) )
  #error This code is designed to run on Teensy platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// Don't define Teensy_TIMER_INTERRUPT_DEBUG > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG      0

#include "TimerInterrupt_Generic.h"

#include "ISR_Timer_Generic.h"

#include <SimpleTimer.h>              // https://github.com/schinken/SimpleTimer

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

#ifndef LED_BLUE
  #define LED_BLUE          2
#endif

#ifndef LED_RED
  #define LED_RED           3
#endif

#define HW_TIMER_INTERVAL_US      10000L

volatile uint32_t startMillis = 0;

// You can select Teensy Hardware Timer  from TEENSY_TIMER_1 or TEENSY_TIMER_3

// Init Teensy timer TEENSY_TIMER_1
TeensyTimer ITimer(TEENSY_TIMER_1);

// Init Teensy_ISR_Timer
// Each Teensy_ISR_Timer can service 16 different ISR-based timers
ISR_Timer Teensy_ISR_Timer;

#define LED_TOGGLE_INTERVAL_MS        2000L

void TimerHandler(void)
{
  static bool toggle  = false;
  static int timeRun  = 0;

  Teensy_ISR_Timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * 1000) / HW_TIMER_INTERVAL_US) )
  {
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  (void);

/////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#if USE_COMPLEX_STRUCT

  typedef struct 
  {
    irqCallback   irqCallbackFunc;
    uint32_t      TimerInterval;
    unsigned long deltaMillis;
    unsigned long previousMillis;
  } ISRTimerData;
  
  // In NRF52, avoid doing something fancy in ISR, for example Serial.print()
  // The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
  // Or you can get this run-time error / crash
  
  void doingSomething(int index);

#else

  volatile unsigned long deltaMillis    [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  volatile unsigned long previousMillis [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  // You can assign any interval for any timer here, in milliseconds
  uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
  {
    5000L,  10000L,  15000L,  20000L,  25000L,  30000L,  35000L,  40000L,
    45000L, 50000L,  55000L,  60000L,  65000L,  70000L,  75000L,  80000L
  };
  
  void doingSomething(int index)
  {
    unsigned long currentMillis  = millis();
    
    deltaMillis[index]    = currentMillis - previousMillis[index];
    previousMillis[index] = currentMillis;
  }

#endif

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomething0()
{
  doingSomething(0);
}

void doingSomething1()
{
  doingSomething(1);
}

void doingSomething2()
{
  doingSomething(2);
}

void doingSomething3()
{
  doingSomething(3);
}

void doingSomething4()
{
  doingSomething(4);
}

void doingSomething5()
{
  doingSomething(5);
}

void doingSomething6()
{
  doingSomething(6);
}

void doingSomething7()
{
  doingSomething(7);
}

void doingSomething8()
{
  doingSomething(8);
}

void doingSomething9()
{
  doingSomething(9);
}

void doingSomething10()
{
  doingSomething(10);
}

void doingSomething11()
{
  doingSomething(11);
}

void doingSomething12()
{
  doingSomething(12);
}

void doingSomething13()
{
  doingSomething(13);
}

void doingSomething14()
{
  doingSomething(14);
}

void doingSomething15()
{
  doingSomething(15);
}

#if USE_COMPLEX_STRUCT

  ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
  {
    //irqCallbackFunc, TimerInterval, deltaMillis, previousMillis
    { doingSomething0,    5000L, 0, 0 },
    { doingSomething1,   10000L, 0, 0 },
    { doingSomething2,   15000L, 0, 0 },
    { doingSomething3,   20000L, 0, 0 },
    { doingSomething4,   25000L, 0, 0 },
    { doingSomething5,   30000L, 0, 0 },
    { doingSomething6,   35000L, 0, 0 },
    { doingSomething7,   40000L, 0, 0 },
    { doingSomething8,   45000L, 0, 0 },
    { doingSomething9,   50000L, 0, 0 },
    { doingSomething10,  55000L, 0, 0 },
    { doingSomething11,  60000L, 0, 0 },
    { doingSomething12,  65000L, 0, 0 },
    { doingSomething13,  70000L, 0, 0 },
    { doingSomething14,  75000L, 0, 0 },
    { doingSomething15,  80000L, 0, 0 }
  };
  
  void doingSomething(int index)
  {
    unsigned long currentMillis  = millis();
    
    curISRTimerData[index].deltaMillis    = currentMillis - curISRTimerData[index].previousMillis;
    curISRTimerData[index].previousMillis = currentMillis;
  }

#else

  irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
  {
    doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
    doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
    doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
    doingSomething12, doingSomething13, doingSomething14, doingSomething15
  };

#endif
///////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static unsigned long previousMillis = startMillis;

  unsigned long currMillis = millis();

  Serial.printf("SimpleTimer : %lus, ms = %lu, Dms : %lu\n", SIMPLE_TIMER_MS / 1000, currMillis, currMillis - previousMillis);

  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT    
    Serial.printf("Timer : %d, programmed : %lu, actual : %lu\n", i, curISRTimerData[i].TimerInterval, curISRTimerData[i].deltaMillis);
#else
    Serial.printf("Timer : %d, programmed : %lu, actual : %lu\n", i, TimerInterval[i], deltaMillis[i]);
#endif    
  }

  previousMillis = currMillis;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting ISR_16_Timers_Array_Complex on " + String(BOARD_NAME));
  Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    startMillis = millis();
    Serial.printf("Starting  ITimer OK, millis() = %ld\n", startMillis);
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each Teensy_ISR_Timer
  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISRTimerData[i].previousMillis = startMillis;
    Teensy_ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
    previousMillis[i] = startMillis;
    Teensy_ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
#endif    
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of Teensy_ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of Teensy_ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call Teensy_ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
```
---
---

### Debug Terminal Output Samples

1. The following is the sample terminal output when running example [ISR_Timer_Complex_Ethernet](examples/SAMDUE/ISR_Timer_Complex_Ethernet) on **Arduino SAM DUE** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after 10.917s !!!**. Then in loop(), it's also activated **every 3s**.

```
Starting ISR_Timer_Complex_Ethernet on SAM DUE
TimerInterrupt_Generic v1.3.1
Using Timer(0) = TC0, channel = 0, IRQ = TC0_IRQn
Timer(0), us = 50000.00
ITimer attached to Timer(0)
[5] Getting IP...
[7] MAC: FE-8A-F1-EA-DE-82
_pinCS = 0
W5100 init, using SS_PIN_DEFAULT = 10, new ss_pin = 10, W5100Class::ss_pin = 10
W5100::init: W5100, SSIZE =4096
2s: Delta ms = 2000
2s: Delta ms = 2000
[7728] IP:192.168.2.134
[7728] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on Arduino Due

[7732] BlynkArduinoClient.connect: Connecting to account.duckdns.org:8080
[7849] Ready (ping: 6ms).
IP = 192.168.2.134
2s: Delta ms = 2000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 10917
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
11s: Delta ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
11s: Delta ms = 11000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
11s: Delta ms = 11000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
11s: Delta ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000

```

---

2. The following is the sample terminal output when running example [ISR_Timer_Complex_Ethernet](examples/NRF52/ISR_Timer_Complex_Ethernet) on **Adafruit NRF52840_FEATHER EXPRESS using W5500 Ethernet* to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after 4.867s !!!**. Then in loop(), it's also activated **every 3s**.

```
Starting ISR_Timer_Complex_Ethernet on NRF52840_FEATHER
TimerInterrupt_Generic v1.3.1
NRF52TimerInterrupt: F_CPU (MHz) = 64, Timer = NRF_TIMER2
NRF52TimerInterrupt: _fre = 1000000.00, _count = 50000
Starting  ITimer OK, millis() = 1419
[1419] Getting IP...
[1419] MAC: FE-BE-97-DA-C3-EA
_pinCS = 0
W5100 init, using SS_PIN_DEFAULT = 10, new ss_pin = 10, W5100Class::ss_pin = 10
W5100::init: W5500, SSIZE =4096
[3104] IP:192.168.2.129
[3104] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on ARDUINO_NRF52_ADAFRUIT

[3106] BlynkArduinoClient.connect: Connecting to account.duckdns.org:8080
[3218] Ready (ping: 8ms).
IP = 192.168.2.129
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 4867
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
11s: Delta ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
11s: Delta ms = 11000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
21s: Delta ms = 21000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
11s: Delta ms = 11000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
11s: Delta ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
```

---

3. The following is the sample terminal output when running example [ISR_16_Timers_Array](examples/SAMD/ISR_16_Timers_Array) on **Arduino SAMD21 SAMD_NANO_33_IOT** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy or blocked**. The 16 independent ISR timers are **programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!**

While software timer, **programmed for 2s, is activated after 10.000s in loop()!!!**.

In this example, 16 independent ISR Timers are used, yet utilized just one Hardware Timer. The Timer Intervals and Function Pointers are stored in arrays to facilitate the code modification.


```
Starting ISR_16_Timers_Array on SAMD_NANO_33_IOT
TimerInterrupt_Generic v1.3.1
CPU Frequency = 48 MHz
F_CPU (MHz) = 48, TIMER_HZ = 48
TC_Timer::startTimer _Timer = 0x42002c00, TC3 = 0x42002c00
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

4. The following is the sample terminal output when running example [**TimerInterruptTest**](examples/TEENSY/TimerInterruptTest) on **Teensy 4.1** to demonstrate how to start/stop and the accuracy of Hardware Timers.

```
Starting TimerInterruptTest on Teensy 4.0/4.1
TimerInterrupt_Generic v1.3.1
CPU Frequency = 600 MHz
TEENSY_TIMER_1, F_BUS_ACTUAL (MHz) = 150, request interval = 30000, actual interval (us) = 29999
Prescale = 7, _timerCount = 17578
Starting  ITimer0 OK, millis() = 1128
Stop ITimer0, millis() = 5001
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
Start ITimer0, millis() = 10002
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
TeensyTimerInterrupt:resumeTimer TEENSY_TIMER_1
Stop ITimer0, millis() = 15003
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
Start ITimer0, millis() = 20004
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
TeensyTimerInterrupt:resumeTimer TEENSY_TIMER_1
Stop ITimer0, millis() = 25005
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
Start ITimer0, millis() = 30006
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
TeensyTimerInterrupt:resumeTimer TEENSY_TIMER_1
Stop ITimer0, millis() = 35007
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
Start ITimer0, millis() = 40008
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
TeensyTimerInterrupt:resumeTimer TEENSY_TIMER_1
Stop ITimer0, millis() = 45009
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
Start ITimer0, millis() = 50010
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1
TeensyTimerInterrupt:resumeTimer TEENSY_TIMER_1
Stop ITimer0, millis() = 55011
TeensyTimerInterrupt:stopTimer TEENSY_TIMER_1

```

---

5. The following is the sample terminal output when running example [ISR_Timer_Complex](examples/ESP32/ISR_Timer_Complex) on **ESP32_DEV** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after 3.435s !!!**

```

Starting Argument_None on ESP32_DEV
TimerInterrupt_Generic v1.3.1
CPU Frequency = 240 MHz
ESP32TimerInterrupt: _timerNo = 1, _fre = 1000000.00, _count = 0 - 50000
Starting  ITimer OK, millis() = 2140
[2341] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on ESP32

[2342] Protocol connect: timeout =9000
[2346] BlynkArduinoClient.connect: Connecting to ****.duckdns.org:8080
[2506] Ready (ping: 35ms).
Blynk connected
2s: D ms = 2000   <=== ISR Timer still very accurate while system busy
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3435 <=== Software Timer not accurate when system busy
2s: D ms = 2000   <=== ISR Timer still very accurate no matter system busy or not
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
5s: D ms = 5000
11s: D ms = 11000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
5s: D ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: D ms = 2000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
5s: D ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
11s: D ms = 11000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: D ms = 2000
2s: D ms = 2000
5s: D ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
11s: D ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
```

---

6. The following is the sample terminal output when running example [ISR_Timer_Complex](examples/ESP8266/ISR_Timer_Complex) on **ESP8266_NODEMCU** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after 4.258s !!!**

```
Starting ISR_Timer_Complex on ESP8266_NODEMCU
TimerInterrupt_Generic v1.3.1
CPU Frequency = 160 MHz
ESP8266TimerInterrupt: _fre = 312500.00, _count = 15625
Starting  ITimer OK, millis() = 64
doingSomething2s: Delta ms = 2000   // ISR_Timer, 2000ms interval, still working even if blocked by WiFi/Blynk connecting tasks
doingSomething2s: Delta ms = 2000
doingSomething5s: Delta ms = 5000
Delta ms = 5000                     //ISR_Timer, 5000ms interval, blynking an LED
doingSomething2s: Delta ms = 2000
[6159] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on NodeMCU

[6161] Protocol connect: timeout =9000
[6164] Connecting to ****.duckdns.org:8080
[6255] Ready (ping: 5ms).
Blynk connected
doingSomething2s: Delta ms = 2000  <=== ISR Timer still very accurate while system busy
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 4258  //Software Timer, 2000ms interval, blocked by WiFi/Blynk connecting tasks
doingSomething2s: Delta ms = 2000
doingSomething5s: Delta ms = 5000
doingSomething10s: Delta ms = 5000
Delta ms = 5000
doingSomething2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000  //Software Timer, 2000ms interval, blocked by delay() in loop() to demonstrate bad task.
doingSomething2s: Delta ms = 2000
doingSomething5s: Delta ms = 5000
Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3001 <=== Software Timer not accurate when system busy
doingSomething2s: Delta ms = 2000
doingSomething2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000 <=== Software Timer not accurate when system busy
doingSomething2s: Delta ms = 2000
doingSomething5s: Delta ms = 5000
doingSomething10s: Delta ms = 10000
```

---

7. The following is the sample terminal output when running example [ISR_Timer_Complex](examples/STM32/ISR_Timer_Complex) on **STM32F7 Nucleo-144 F767ZI using Built-in LAN8742A Ethernet and STM32Ethernet Library** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after 9.782s !!!**. Then in loop(), it's also activated **every 3s**.

```
Starting ISR_Timer_Complex on NUCLEO_F767ZI
TimerInterrupt_Generic v1.3.1
CPU Frequency = 216 MHz
STM32TimerInterrupt: Timer Input Freq (Hz) = 216000000, _fre = 1000000.00, _count = 50000
Starting  ITimer OK, millis() = 6
[9] MAC:FE-E1-88-EC-DD-95
2s: Delta ms = 2000
2s: Delta ms = 2000
[6626] IP:192.168.2.116
[6626] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on STM32 NUCLEO_F767ZI

[6636] BlynkArduinoClient.connect: Connecting to account.duckdns.org:8080
[6721] Ready (ping: 6ms).
IP = 192.168.2.116
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 9782
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
11s: Delta ms = 11000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
11s: Delta ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
```

---

8. The following is the sample terminal output when running example [**TimerInterruptTest**](examples/STM32/TimerInterruptTest) on **STM32F7 Nucleo-144 F767ZI** to demonstrate how to start/stop Hardware Timers.

```

Starting TimerInterruptTest on NUCLEO_F767ZI
TimerInterrupt_Generic v1.3.1
CPU Frequency = 216 MHz
STM32TimerInterrupt: Timer Input Freq (Hz) = 216000000, _fre = 1000000.00, _count = 1000000
Starting  ITimer0 OK, millis() = 108
STM32TimerInterrupt: Timer Input Freq (Hz) = 108000000, _fre = 1000000.00, _count = 3000000
Starting  ITimer1 OK, millis() = 119
Stop ITimer0, millis() = 5001
Start ITimer0, millis() = 10002
Stop ITimer1, millis() = 15001
Stop ITimer0, millis() = 15003
Start ITimer0, millis() = 20004
Stop ITimer0, millis() = 25005
Start ITimer1, millis() = 30002
Start ITimer0, millis() = 30006
Stop ITimer0, millis() = 35007
Start ITimer0, millis() = 40008
Stop ITimer1, millis() = 45003
Stop ITimer0, millis() = 45009
Start ITimer0, millis() = 50010
Stop ITimer0, millis() = 55011
Start ITimer1, millis() = 60004
Start ITimer0, millis() = 60012
Stop ITimer0, millis() = 65013
Start ITimer0, millis() = 70014
Stop ITimer1, millis() = 75005
Stop ITimer0, millis() = 75015
Start ITimer0, millis() = 80016
Stop ITimer0, millis() = 85017
Start ITimer1, millis() = 90006
Start ITimer0, millis() = 90018
Stop ITimer0, millis() = 95019
Start ITimer0, millis() = 100020
Stop ITimer1, millis() = 105007
Stop ITimer0, millis() = 105021
Start ITimer0, millis() = 110022
Stop ITimer0, millis() = 115023
Start ITimer1, millis() = 120008
Start ITimer0, millis() = 120024
Stop ITimer0, millis() = 125025
Start ITimer0, millis() = 130026
Stop ITimer1, millis() = 135009
Stop ITimer0, millis() = 135027
Start ITimer0, millis() = 140028

```

---

9. The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/NANO33BLE/ISR_16_Timers_Array_Complex)  on **Nano 33 BLE** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after more than 3.000s in loop().

```
Starting ISR_16_Timers_Array_Complex on Nano 33 BLE
TimerInterrupt_Generic v1.3.1
NRF52_MBED_TimerInterrupt: Timer = NRF_TIMER3
NRF52_MBED_TimerInterrupt: _fre = 1000000.00, _count = 10000
Starting  ITimer OK, millis() = 714
simpleTimer2s, ms=3714, Dms=3000
Timer : 0, programmed : 5000, actual : 0
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=6730, Dms=3016
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=9746, Dms=3016
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=12762, Dms=3016
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 10008
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=15778, Dms=3016
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 10008
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=18794, Dms=3016
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 10008
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=21810, Dms=3016
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 9996
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 20004
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=24826, Dms=3016
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 9996
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 20004
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=27842, Dms=3016
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 9996
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 20004
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=30858, Dms=3016
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 20004
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=33874, Dms=3016
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 20004
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=36890, Dms=3016
Timer : 0, programmed : 5000, actual : 4992
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 20004
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=39906, Dms=3016
Timer : 0, programmed : 5000, actual : 4992
Timer : 1, programmed : 10000, actual : 10004
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 20004
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=42922, Dms=3016
Timer : 0, programmed : 5000, actual : 5001
Timer : 1, programmed : 10000, actual : 9993
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 19997
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=45938, Dms=3016
Timer : 0, programmed : 5000, actual : 5001
Timer : 1, programmed : 10000, actual : 9993
Timer : 2, programmed : 15000, actual : 14994
Timer : 3, programmed : 20000, actual : 19997
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=48954, Dms=3016
Timer : 0, programmed : 5000, actual : 5001
Timer : 1, programmed : 10000, actual : 9993
Timer : 2, programmed : 15000, actual : 14994
Timer : 3, programmed : 20000, actual : 19997
Timer : 4, programmed : 25000, actual : 25006
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=51970, Dms=3016
Timer : 0, programmed : 5000, actual : 5001
Timer : 1, programmed : 10000, actual : 10002
Timer : 2, programmed : 15000, actual : 14994
Timer : 3, programmed : 20000, actual : 19997
Timer : 4, programmed : 25000, actual : 24997
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=54986, Dms=3016
Timer : 0, programmed : 5000, actual : 5001
Timer : 1, programmed : 10000, actual : 10002
Timer : 2, programmed : 15000, actual : 14994
Timer : 3, programmed : 20000, actual : 19997
Timer : 4, programmed : 25000, actual : 24997
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=58002, Dms=3016
Timer : 0, programmed : 5000, actual : 5003
Timer : 1, programmed : 10000, actual : 10002
Timer : 2, programmed : 15000, actual : 14994
Timer : 3, programmed : 20000, actual : 19997
Timer : 4, programmed : 25000, actual : 24997
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=61018, Dms=3016
Timer : 0, programmed : 5000, actual : 5003
Timer : 1, programmed : 10000, actual : 10006
Timer : 2, programmed : 15000, actual : 15007
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 24997
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=64034, Dms=3016
Timer : 0, programmed : 5000, actual : 5003
Timer : 1, programmed : 10000, actual : 10006
Timer : 2, programmed : 15000, actual : 15007
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 24997
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=67050, Dms=3016
Timer : 0, programmed : 5000, actual : 4992
Timer : 1, programmed : 10000, actual : 10006
Timer : 2, programmed : 15000, actual : 15007
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 24997
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65001
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=70066, Dms=3016
Timer : 0, programmed : 5000, actual : 4992
Timer : 1, programmed : 10000, actual : 10006
Timer : 2, programmed : 15000, actual : 15007
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 24997
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65001
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=73082, Dms=3016
Timer : 0, programmed : 5000, actual : 5003
Timer : 1, programmed : 10000, actual : 9995
Timer : 2, programmed : 15000, actual : 15007
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 24997
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35004
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65001
Timer : 13, programmed : 70000, actual : 70004
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=76098, Dms=3016
Timer : 0, programmed : 5000, actual : 5003
Timer : 1, programmed : 10000, actual : 9995
Timer : 2, programmed : 15000, actual : 14998
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 25004
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35004
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65001
Timer : 13, programmed : 70000, actual : 70004
Timer : 14, programmed : 75000, actual : 75007
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=79114, Dms=3016
Timer : 0, programmed : 5000, actual : 5003
Timer : 1, programmed : 10000, actual : 9995
Timer : 2, programmed : 15000, actual : 14998
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 25004
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35004
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65001
Timer : 13, programmed : 70000, actual : 70004
Timer : 14, programmed : 75000, actual : 75007
Timer : 15, programmed : 80000, actual : 0
simpleTimer2s, ms=82130, Dms=3016
Timer : 0, programmed : 5000, actual : 5002
Timer : 1, programmed : 10000, actual : 10005
Timer : 2, programmed : 15000, actual : 14998
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25004
Timer : 5, programmed : 30000, actual : 30001
Timer : 6, programmed : 35000, actual : 35004
Timer : 7, programmed : 40000, actual : 40008
Timer : 8, programmed : 45000, actual : 45002
Timer : 9, programmed : 50000, actual : 50003
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65001
Timer : 13, programmed : 70000, actual : 70004
Timer : 14, programmed : 75000, actual : 75007
Timer : 15, programmed : 80000, actual : 80009

```

---
---

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

---

### Currently Supported Boards

  - **ESP8266**
  - **ESP32**
  - **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox etc.**.
  - **Arduino SAMD21 (ZERO, MKR, NANO_33_IOT, etc.)**.
  - **Adafruit SAM21 (Itsy-Bitsy M0, Metro M0, Feather M0, Gemma M0, etc.)**.
  - **Adafruit SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)**.
  - **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)**
  - **STM32 (Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32F1, STM32F3, STM32F4, STM32H7, STM32L0, etc.)**.
  - **STM32F/L/H/G/WB/MP1 (Nucleo-64 L053R8,Nucleo-144, Nucleo-64, Nucleo-32, Discovery, STM32Fx, STM32H7, STM32Lx, STM32Gx, STM32WB, STM32MP1, etc.) having 64K+ Flash program memory.**
  
  - **Teensy boards** such as :
    - **Teensy 4.1, 4.0**
    - **Teensy 3.6, 3.5, 3.2/3.1, 3.0**
    - **Teensy LC**
    - **Teensy++ 2.0 and Teensy 2.0**

  - **Arduino AVR boards** such as :
    - **Arduino Uno / Nano, Mega / Mini / Arduino Ethernet, Fio, BT, LilyPad, Pro, Pro Mini, NG, UNO Wifi, etc.**
  - **Arduino Nano-33-BLE**

---
---

### Issues ###

Submit issues to: [TimerInterrupt_Generic issues](https://github.com/khoih-prog/TimerInterrupt_Generic/issues)

---

## TO DO

1. Search for bug and improvement.


## DONE


1. Basic hardware timers for AVR, ESP8266, ESP32, SAMD, SAM DUE, nRF52, Teensy, etc.
2. More hardware-initiated software-enabled timers
3. Longer time interval

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. Use some code from the [**Ivan Seidel's DueTimer Library**](https://github.com/ivanseidel/DueTimer).
2. Use some code from the [**Tamasa's ZeroTimer Library**](https://github.com/EHbtj/ZeroTimer).
3. Use some code from the [**Dennis van Gils' SAMD51_InterruptTimer Library**](https://github.com/Dennis-van-Gils/SAMD51_InterruptTimer).
4. Thanks to good work of [Miguel Wisintainer](https://github.com/tcpipchip) for working with, developing, debugging and testing.
5. Thanks to [Holger Lembke](https://github.com/holgerlembke) to report [ESP8266TimerInterrupt Issue 8: **ESP8266Timer and PWM --> wdt reset**](https://github.com/khoih-prog/ESP8266TimerInterrupt/issues/8), leading to the [HOWTO Use PWM analogWrite() with ESP8266 running Timer1 Interrupt](https://github.com/khoih-prog/ESP8266TimerInterrupt#howto-use-pwm-analogwrite-with-esp8266-running-timer1-interrupt) notes.
6. Thanks to [Jelmer](https://github.com/jjwbruijn) to report and make PR in [Moved the implementation header file to a separate .cpp file](https://github.com/khoih-prog/ESP32TimerInterrupt/pull/6). 

<table>
  <tr>
    <td align="center"><a href="https://github.com/ivanseidel"><img src="https://github.com/ivanseidel.png" width="100px;" alt="ivanseidel"/><br /><sub><b>⭐️ Ivan Seidel</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/EHbtj"><img src="https://github.com/EHbtj.png" width="100px;" alt="EHbtj"/><br /><sub><b>⭐️ Tamasa</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/Dennis-van-Gils"><img src="https://github.com/Dennis-van-Gils.png" width="100px;" alt="Dennis-van-Gils"/><br /><sub><b> Dennis van Gils</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/tcpipchip"><img src="https://github.com/tcpipchip.png" width="100px;" alt="tcpipchip"/><br /><sub><b> Miguel Wisintainer</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/holgerlembke"><img src="https://github.com/holgerlembke.png" width="100px;" alt="holgerlembke"/><br /><sub><b>Holger Lembke</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/jjwbruijn"><img src="https://github.com/jjwbruijn.png" width="100px;" alt="jjwbruijn"/><br /><sub><b>Jelmer</b></sub></a><br /></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/TimerInterrupt_Generic/blob/main/LICENSE)

---

## Copyright

Copyright 2020- Khoi Hoang


