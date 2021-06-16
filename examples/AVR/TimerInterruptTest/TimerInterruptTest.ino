/****************************************************************************************************************************
   TimerInterruptTest.ino
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
*****************************************************************************************************************************/

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#define USE_TIMER_1     true

#if ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) || defined(ARDUINO_AVR_FEATHER328P) || \
        defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || defined(ARDUINO_AVR_PROTRINKET5FTDI) || \
        defined(ARDUINO_AVR_PROTRINKET3FTDI) )
  #define USE_TIMER_2     true
  #warning Using Timer1, Timer2
#else          
  #define USE_TIMER_3     true
  #warning Using Timer1, Timer3
#endif

#include "TimerInterrupt_Generic.h"

#if !(TIMER_INTERRUPT_USING_AVR)
  #error This is designed only for Arduino AVR board! Please check your Tools->Board setting.
#endif

#if !defined(LED_BUILTIN)
  #define LED_BUILTIN     13
#endif

#if USE_TIMER_1

void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle1 = false;

#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("ITimer1 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(outputPin, toggle1);
  toggle1 = !toggle1;
}

#endif

#if (USE_TIMER_2 || USE_TIMER_3)

void TimerHandler(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle = false;

#if (TIMER_INTERRUPT_DEBUG > 1)
  #if USE_TIMER_2
    Serial.print("ITimer2 called, millis() = ");
  #elif USE_TIMER_3
    Serial.print("ITimer3 called, millis() = ");
  #endif
  
  Serial.println(millis());
#endif

  //timer interrupt toggles outputPin
  digitalWrite(outputPin, toggle);
  toggle = !toggle;
}

#endif

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin  = A0;

#define USING_LOOP_TEST       false

#define TIMER1_INTERVAL_MS    1000
#define TIMER1_FREQUENCY      (float) (1000.0f / TIMER1_INTERVAL_MS)

#define TIMER_INTERVAL_MS     2000
#define TIMER_FREQUENCY       (float) (1000.0f / TIMER_INTERVAL_MS)


#if USING_LOOP_TEST
  #define TIMER1_DURATION_MS    (10UL * TIMER1_INTERVAL_MS)
  #define TIMER_DURATION_MS     (20UL * TIMER_INTERVAL_MS)
#else
  #define TIMER1_DURATION_MS    0
  #define TIMER_DURATION_MS     0
#endif

void setup()
{ 
  pinMode(outputPin1, OUTPUT);
  pinMode(outputPin,  OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  Serial.print(F("\nStarting TimerInterruptTest on "));
  Serial.println(BOARD_TYPE);
  Serial.println(TIMER_INTERRUPT_VERSION);
  Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

#if USE_TIMER_1

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

#endif

#if USE_TIMER_2 

  ITimer2.init();

  if (ITimer2.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler, outputPin, TIMER_DURATION_MS))
  {
    Serial.print(F("Starting  ITimer2 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer2. Select another freq. or timer"));
    
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler, outputPin, TIMER_DURATION_MS))
  {
    Serial.print(F("Starting  ITimer3 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));
    
#endif
}

void loop()
{
#if USING_LOOP_TEST
  static unsigned long lastTimer1 = 0;
  static unsigned long lastTimer  = 0;

  static bool timerPaused         = false;
  static bool timerResumed        = false;

  if (millis() - lastTimer1 > TIMER1_DURATION_MS * 3)
  {
#if USE_TIMER_2

    if (millis() - lastTimer > TIMER_DURATION_MS * 3)
    {
      lastTimer = millis();
      
      Serial.print(F("Re-enable ITimer2, millis() = ")); Serial.println(lastTimer);
      
      ITimer2.reattachInterrupt(TIMER_DURATION_MS);
    }
    
#elif USE_TIMER_3

    if (millis() - lastTimer > TIMER_DURATION_MS * 3)
    {
      lastTimer = millis();
      
      Serial.print(F("Re-enable ITimer3, millis() = ")); Serial.println(lastTimer);
      
      ITimer3.reattachInterrupt(TIMER_DURATION_MS);
    }
    
#endif

    lastTimer1 = millis();
    // try reinit timer
    Serial.print(F("Re-enable ITimer1, millis() = ")); Serial.print(lastTimer1);
    Serial.print(F(" count = ")); Serial.println(ITimer1.getCount() - 1);

    ITimer1.reattachInterrupt(TIMER1_DURATION_MS);
    timerPaused   = false;
    timerResumed  = false;
  }
  else if ( !timerPaused && (millis() - lastTimer1 > TIMER1_DURATION_MS / 2) )
  {
    timerPaused = true;

    Serial.print(F("Pause ITimer1, millis() = ")); Serial.print(millis());
    Serial.print(F(" count = ")); Serial.println(ITimer1.getCount() - 1);
    
    ITimer1.pauseTimer();
  }
  else if ( !timerResumed && (millis() - lastTimer1 > ( TIMER1_DURATION_MS * 3 ) / 2) )
  {
    timerResumed = true;
    
    Serial.print(F("Resume ITimer1, millis() = ")); Serial.print(millis());
    Serial.print(F(" count = ")); Serial.println(ITimer1.getCount() - 1);
    
    ITimer1.resumeTimer();
  }
#endif

}
