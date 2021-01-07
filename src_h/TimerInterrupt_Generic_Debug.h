/****************************************************************************************************************************
  TimerInterrupt_Generic_Debug.h
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
 *****************************************************************************************************************************/

#pragma once

#ifndef TIMERINTERRUPT_GENERIC_DEBUG_H
#define TIMERINTERRUPT_GENERIC_DEBUG_H

#ifdef TIMERINTERRUPT_DEBUG_PORT
  #define TISR_DBG_PORT      TIMERINTERRUPT_DEBUG_PORT
#else
  #define TISR_DBG_PORT      Serial
#endif

// Change _TIMERINTERRUPT_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _TIMERINTERRUPT_LOGLEVEL_
  #define _TIMERINTERRUPT_LOGLEVEL_       1
#endif

#define TISR_LOGERROR(x)         if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.println(x); }
#define TISR_LOGERROR0(x)        if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print(x); }
#define TISR_LOGERROR1(x,y)      if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(y); }
#define TISR_LOGERROR2(x,y,z)    if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(z); }
#define TISR_LOGERROR3(x,y,z,w)  if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(z); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(w); }

#define TISR_LOGWARN(x)          if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.println(x); }
#define TISR_LOGWARN0(x)         if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print(x); }
#define TISR_LOGWARN1(x,y)       if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(y); }
#define TISR_LOGWARN2(x,y,z)     if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(z); }
#define TISR_LOGWARN3(x,y,z,w)   if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(z); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(w); }

#define TISR_LOGINFO(x)          if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.println(x); }
#define TISR_LOGINFO0(x)         if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print(x); }
#define TISR_LOGINFO1(x,y)       if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(y); }
#define TISR_LOGINFO2(x,y,z)     if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(z); }
#define TISR_LOGINFO3(x,y,z,w)   if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(z); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(w); }

#define TISR_LOGDEBUG(x)         if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.println(x); }
#define TISR_LOGDEBUG0(x)        if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print(x); }
#define TISR_LOGDEBUG1(x,y)      if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(y); }
#define TISR_LOGDEBUG2(x,y,z)    if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(z); }
#define TISR_LOGDEBUG3(x,y,z,w)  if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(z); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(w); }

#endif    //TIMERINTERRUPT_GENERIC_DEBUG_H
