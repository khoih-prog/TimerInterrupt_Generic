/****************************************************************************************************************************
  ISR_Timer-Impl_Generic.h
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
*****************************************************************************************************************************/

#pragma once

#ifndef ISR_TIMER_IMPL_GENERIC_H
#define ISR_TIMER_IMPL_GENERIC_H

#include <string.h>

ISR_Timer::ISR_Timer()
  : numTimers (-1)
{
}

void IRAM_ATTR_PREFIX ISR_Timer::init() 
{
  unsigned long current_millis = millis();

  for (uint8_t i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    memset((void*) &timer[i], 0, sizeof (timer_t));
    timer[i].prev_millis = current_millis;
  }

  numTimers = 0;
  
#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  timerMux = portMUX_INITIALIZER_UNLOCKED;
#endif
}

void IRAM_ATTR_PREFIX ISR_Timer::run() 
{
  uint8_t i;
  unsigned long current_millis;

  // get current time
  current_millis = millis();
  
#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portENTER_CRITICAL_ISR(&timerMux);
#endif

  for (i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {

    timer[i].toBeCalled = TIMER_DEFCALL_DONTRUN;

    // no callback == no timer, i.e. jump over empty slots
    if (timer[i].callback != NULL) 
    {

      // is it time to process this timer ?
      // see http://arduino.cc/forum/index.php/topic,124048.msg932592.html#msg932592

      if ((current_millis - timer[i].prev_millis) >= timer[i].delay) 
      {
        unsigned long skipTimes = (current_millis - timer[i].prev_millis) / timer[i].delay;
        
        // update time
        timer[i].prev_millis += timer[i].delay * skipTimes;

        // check if the timer callback has to be executed
        if (timer[i].enabled) 
        {

          // "run forever" timers must always be executed
          if (timer[i].maxNumRuns == TIMER_RUN_FOREVER) 
          {
            timer[i].toBeCalled = TIMER_DEFCALL_RUNONLY;
          }
          // other timers get executed the specified number of times
          else if (timer[i].numRuns < timer[i].maxNumRuns) 
          {
            timer[i].toBeCalled = TIMER_DEFCALL_RUNONLY;
            timer[i].numRuns++;

            // after the last run, delete the timer
            if (timer[i].numRuns >= timer[i].maxNumRuns) 
            {
              timer[i].toBeCalled = TIMER_DEFCALL_RUNANDDEL;
            }
          }
        }
      }
    }
  }

  for (i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    if (timer[i].toBeCalled == TIMER_DEFCALL_DONTRUN)
      continue;

    if (timer[i].hasParam)
      (*(timerCallback_p)timer[i].callback)(timer[i].param);
    else
      (*(timerCallback)timer[i].callback)();

    if (timer[i].toBeCalled == TIMER_DEFCALL_RUNANDDEL)
      deleteTimer(i);
  }
  
#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portEXIT_CRITICAL_ISR(&timerMux);
#endif
}


// find the first available slot
// return -1 if none found
int IRAM_ATTR_PREFIX ISR_Timer::findFirstFreeSlot() 
{
  // all slots are used
  if (numTimers >= MAX_NUMBER_TIMERS) 
  {
    return -1;
  }

  // return the first slot with no callback (i.e. free)
  for (uint8_t i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    if (timer[i].callback == NULL) 
    {
      return (int) i;
    }
  }

  // no free slots found
  return -1;
}


int IRAM_ATTR_PREFIX ISR_Timer::setupTimer(unsigned long d, void* f, void* p, bool h, unsigned n) 
{
  int freeTimer;

  if (numTimers < 0) 
  {
    init();
  }

  freeTimer = findFirstFreeSlot();
  if (freeTimer < 0) 
  {
    return -1;
  }

  if (f == NULL) 
  {
    return -1;
  }

  timer[freeTimer].delay = d;
  timer[freeTimer].callback = f;
  timer[freeTimer].param = p;
  timer[freeTimer].hasParam = h;
  timer[freeTimer].maxNumRuns = n;
  timer[freeTimer].enabled = true;
  timer[freeTimer].prev_millis = millis();

  numTimers++;

  return freeTimer;
}


int IRAM_ATTR_PREFIX ISR_Timer::setTimer(unsigned long d, timerCallback f, unsigned n) 
{
  return setupTimer(d, (void *)f, NULL, false, n);
}

int IRAM_ATTR_PREFIX ISR_Timer::setTimer(unsigned long d, timerCallback_p f, void* p, unsigned n) 
{
  return setupTimer(d, (void *)f, p, true, n);
}

int IRAM_ATTR_PREFIX ISR_Timer::setInterval(unsigned long d, timerCallback f) 
{
  return setupTimer(d, (void *)f, NULL, false, TIMER_RUN_FOREVER);
}

int IRAM_ATTR_PREFIX ISR_Timer::setInterval(unsigned long d, timerCallback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, TIMER_RUN_FOREVER);
}

int IRAM_ATTR_PREFIX ISR_Timer::setTimeout(unsigned long d, timerCallback f) 
{
  return setupTimer(d, (void *)f, NULL, false, TIMER_RUN_ONCE);
}

int IRAM_ATTR_PREFIX ISR_Timer::setTimeout(unsigned long d, timerCallback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, TIMER_RUN_ONCE);
}

bool IRAM_ATTR_PREFIX ISR_Timer::changeInterval(unsigned numTimer, unsigned long d) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return false;
  }

  // Updates interval of existing specified timer
  if (timer[numTimer].callback != NULL) 
  {
#if ( defined(ESP32) || ESP32 )
    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
    portENTER_CRITICAL(&timerMux);
#endif
  
    timer[numTimer].delay = d;
    timer[numTimer].prev_millis = millis();

#if ( defined(ESP32) || ESP32 )
    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
    portEXIT_CRITICAL_ISR(&timerMux);
#endif

    return true;
  }
  
  // false return for non-used numTimer, no callback
  return false;
}

void IRAM_ATTR_PREFIX ISR_Timer::deleteTimer(unsigned timerId) 
{
  if (timerId >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  // nothing to delete if no timers are in use
  if (numTimers == 0) 
  {
    return;
  }

  // don't decrease the number of timers if the specified slot is already empty
  if (timer[timerId].callback != NULL) 
  {
#if ( defined(ESP32) || ESP32 )
    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
    portENTER_CRITICAL(&timerMux);
#endif
  
    memset((void*) &timer[timerId], 0, sizeof (timer_t));
    timer[timerId].prev_millis = millis();

    // update number of timers
    numTimers--;
    
#if ( defined(ESP32) || ESP32 )
    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
    portEXIT_CRITICAL(&timerMux);
#endif    
  }
}

// function contributed by code@rowansimms.com
void IRAM_ATTR_PREFIX ISR_Timer::restartTimer(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return;
  }
  
#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portENTER_CRITICAL(&timerMux);
#endif  

  timer[numTimer].prev_millis = millis();
  
#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portEXIT_CRITICAL(&timerMux);
#endif  
}


bool IRAM_ATTR_PREFIX ISR_Timer::isEnabled(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return false;
  }

  return timer[numTimer].enabled;
}


void IRAM_ATTR_PREFIX ISR_Timer::enable(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = true;
}


void IRAM_ATTR_PREFIX ISR_Timer::disable(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = false;
}

void IRAM_ATTR_PREFIX ISR_Timer::enableAll() 
{
  // Enable all timers with a callback assigned (used)

#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portENTER_CRITICAL(&timerMux);
#endif

  for (uint8_t i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == TIMER_RUN_FOREVER) 
    {
      timer[i].enabled = true;
    }
  }
  
#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portEXIT_CRITICAL(&timerMux);
#endif   
}

void IRAM_ATTR_PREFIX ISR_Timer::disableAll() 
{
  // Disable all timers with a callback assigned (used)

#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portENTER_CRITICAL(&timerMux);
#endif

  for (uint8_t i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == TIMER_RUN_FOREVER) 
    {
      timer[i].enabled = false;
    }
  }
  
#if ( defined(ESP32) || ESP32 )
  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portEXIT_CRITICAL(&timerMux);
#endif  
}

void IRAM_ATTR_PREFIX ISR_Timer::toggle(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = !timer[numTimer].enabled;
}


int IRAM_ATTR_PREFIX ISR_Timer::getNumTimers() 
{
  return numTimers;
}

#endif    // ISR_TIMER_IMPL_GENERIC_H
