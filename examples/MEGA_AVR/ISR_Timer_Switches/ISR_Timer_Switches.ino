/****************************************************************************************************************************
  ISR_Timer_Switches.ino
  For Arduino megaAVR ATMEGA4809-based boards (UNO WiFi Rev2, NANO_EVERY, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt_Generic
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
 *****************************************************************************************************************************/
/****************************************************************************************************************************
   ISR_Timer_Switches demontrates the use of ISR combining with Timer Interrupt to avoid being blocked by
   other CPU-monopolizing task

   In this complex example: CPU is connecting to WiFi, Internet and finally Blynk service (https://docs.blynk.cc/)
   Many important tasks are fighting for limited CPU resource in this no-controlled single-tasking environment.
   In certain period, mission-critical tasks (you name it) could be deprived of CPU time and have no chance
   to be executed. This can lead to disastrous results at critical time.
   We hereby will use interrupt to detect whenever one of 2 SWs is active, then use a hardware timer to poll and switch
   ON/OFF the corresponding sample relay (lamp)
   We'll see this ISR-based operation will have highest priority, preempts all remaining tasks to assure its
   functionality.
*****************************************************************************************************************************/

// To be used with UNO WiFi Rev2 and Blynk v0.6.1 only
// Compiler error with Blynk v1.0.0

#if !defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #error To be used with UNO WiFi Rev2 only.
#else
  #warning To be used with Blynk v0.6.1 only. Compiler error with Blynk v1.0.0
#endif

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG true

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_16MHZ     true
#define USING_8MHZ      false
#define USING_250KHZ    false

#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false

#include "TimerInterrupt_Generic.h"
#include "ISR_Timer_Generic.h"

#include <BlynkSimpleWiFiNINA_UNO_WiFi.h>     // https://github.com/khoih-prog/Blynk_WiFiNINA_WM

#define BLYNK_HARDWARE_PORT     8080

#define USE_LOCAL_SERVER        true

// If local server
#if USE_LOCAL_SERVER
char blynk_server[]   = "yourname.duckdns.org";
//char blynk_server[]   = "192.168.2.110";
#else
char blynk_server[]   = "";
#endif

char auth[]     = "****";
char ssid[]     = "****";
char pass[]     = "****";

#define TIMER_INTERVAL_MS           100

#define DEBOUNCE_TIME               25
#define LONG_BUTTON_PRESS_TIME_MS   100
#define DEBUG_ISR                   0

// Lamp 1
#define VPIN                        V1
#define LAMPSTATE_PIN               V2
// Lamp 2
#define V2PIN                       V3
#define LAMP2STATE_PIN              V4

//Blynk Color in format #RRGGBB
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"

// Lamp1 and Relay1
WidgetLED  LampStatus(LAMPSTATE_PIN);

volatile unsigned long  lastDebounceTime  = 0;
volatile bool           buttonPressed     = false;
volatile bool           alreadyTriggered  = false;

volatile bool LampState    = false;
volatile bool SwitchReset  = true;

#define RELAY_PIN     A0        // Pin A0 of Mega2560 / UNO / Nano
#define BUTTON_PIN    2         // Pin 2 can be use for external interrupt source on Maga/UNO/Nano

// Lamp2 and Relay2
WidgetLED  Lamp2Status(LAMP2STATE_PIN);

volatile unsigned long  lastDebounceTime2  = 0;
volatile bool           buttonPressed2     = false;
volatile bool           alreadyTriggered2  = false;

volatile bool Lamp2State    = false;
volatile bool Switch2Reset  = true;

#define RELAY2_PIN     A1
#define BUTTON2_PIN    3          // Pin 2 can be use for external interrupt source on Maga/UNO/Nano

unsigned int myWiFiTimeout        =  3200L;  //  3.2s WiFi connection timeout   (WCT)
unsigned int buttonInterval       =  511L;   //  0.5s update button state

// Lamp1
void Falling();
void Rising();

void lightOn();
void lightOff();
void ButtonCheck();
void ToggleRelay();

// Lamp2
void Falling2();
void Rising2();

void light2On();
void light2Off();
void ButtonCheck2();
void ToggleRelay2();

BlynkTimer blynkTimer;

BLYNK_CONNECTED()
{
  // Lamp 1
  LampStatus.on();
  Blynk.setProperty(LAMPSTATE_PIN, "color", LampState ? BLYNK_RED : BLYNK_GREEN );

  // Lamp 2
  Lamp2Status.on();
  Blynk.setProperty(LAMP2STATE_PIN, "color", Lamp2State ? BLYNK_RED : BLYNK_GREEN );

  Blynk.syncVirtual(VPIN);
}

// Lamp1
// Make this a autoreleased pushbutton
BLYNK_WRITE(VPIN)
{
  if (param.asInt())
    ToggleRelay();
}

void Rising()
{
  unsigned long currentTime  = millis();
  unsigned long TimeDiff;

  TimeDiff = currentTime - lastDebounceTime;
  if ( digitalRead(BUTTON_PIN) && (TimeDiff > DEBOUNCE_TIME) )
  {
    buttonPressed = false;
    lastDebounceTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Falling, FALLING);
  }
}

void Falling()
{
  unsigned long currentTime  = millis();

  if ( !digitalRead(BUTTON_PIN) && (currentTime > lastDebounceTime + DEBOUNCE_TIME))
  {
    lastDebounceTime = currentTime;
    buttonPressed = true;
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Rising, RISING);
  }
}

// Lamp2
// Make this a autoreleased pushbutton
BLYNK_WRITE(V2PIN)
{
  if (param.asInt())
    ToggleRelay2();
}

void Rising2()
{
  unsigned long currentTime  = millis();
  unsigned long TimeDiff;

  TimeDiff = currentTime - lastDebounceTime2;
  if ( digitalRead(BUTTON2_PIN) && (TimeDiff > DEBOUNCE_TIME) )
  {
    buttonPressed2 = false;
    lastDebounceTime2 = currentTime;
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), Falling2, FALLING);
  }
}

void Falling2()
{
  unsigned long currentTime  = millis();

  if ( !digitalRead(BUTTON2_PIN) && (currentTime > lastDebounceTime2 + DEBOUNCE_TIME))
  {
    lastDebounceTime2 = currentTime;
    buttonPressed2 = true;
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), Rising2, RISING);
  }
}

// Share for both buttons
void HWCheckButton()
{
  // Button 1
  if (!alreadyTriggered && buttonPressed)
  {
    alreadyTriggered = true;
    ButtonCheck();

  }
  else if (!buttonPressed)
  {
    // Reset flag when button released to avoid triggered repeatedly
    alreadyTriggered = false;
    ButtonCheck();
  }

  // Button 2
  if (!alreadyTriggered2 && buttonPressed2)
  {
    alreadyTriggered2 = true;
    ButtonCheck2();

  }
  else if (!buttonPressed2)
  {
    // Reset flag when button released to avoid triggered repeatedly
    alreadyTriggered2 = false;
    ButtonCheck2();
  }

}

void heartBeatPrint(void)
{
  static int num = 1;

  Serial.print(F("B"));

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void checkButton()
{
  heartBeatPrint();

  // Lamp 1
  if (LampState)
    Blynk.setProperty(LAMPSTATE_PIN, "color", BLYNK_RED);
  else
    Blynk.setProperty(LAMPSTATE_PIN, "color", BLYNK_GREEN);

  // Lamp2
  if (Lamp2State)
    Blynk.setProperty(LAMP2STATE_PIN, "color", BLYNK_RED);
  else
    Blynk.setProperty(LAMP2STATE_PIN, "color", BLYNK_GREEN);
}

//Lamp 1
void ButtonCheck()
{
  boolean SwitchState = (digitalRead(BUTTON_PIN));

  if (!SwitchState && SwitchReset)
  {
    ToggleRelay();
    SwitchReset = false;
  }
  else if (SwitchState)
  {
    SwitchReset = true;
  }
}

void ToggleRelay()
{
  if (LampState)
    lightOff();
  else
    lightOn();
}

void lightOn()
{
  digitalWrite(RELAY_PIN, HIGH);
  LampState = true;
}

void lightOff()
{
  digitalWrite(RELAY_PIN, LOW);
  LampState = false;
}

// Lamp2
void ButtonCheck2()
{
  boolean SwitchState = (digitalRead(BUTTON2_PIN));

  if (!SwitchState && Switch2Reset)
  {
    ToggleRelay2();
    Switch2Reset = false;
  }
  else if (SwitchState)
  {
    Switch2Reset = true;
  }
}

void ToggleRelay2()
{
  if (Lamp2State)
    light2Off();
  else
    light2On();
}

void light2On()
{
  digitalWrite(RELAY2_PIN, HIGH);
  Lamp2State = true;
}

void light2Off()
{
  digitalWrite(RELAY2_PIN, LOW);
  Lamp2State = false;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.print(F("\nStarting ISR_Timer_Switches on ")); Serial.println(BOARD_NAME);
  Serial.println(MEGA_AVR_TIMER_INTERRUPT_VERSION);
  Serial.println(TIMER_INTERRUPT_GENERIC_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  Serial.print(F("TCB Clock Frequency = ")); 

#if USING_16MHZ  
  Serial.println(F("16MHz for highest accuracy"));
#elif USING_8MHZ  
  Serial.println(F("8MHz for very high accuracy"));
#else
  Serial.println(F("250KHz for lower accuracy but longer time"));
#endif

  LampState = false;

  // Lamp 1
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(RELAY_PIN, LOW);
  LampState = false;

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Falling, FALLING);

  // Lamp 2
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  digitalWrite(RELAY2_PIN, LOW);
  Lamp2State = false;

  attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), Falling2, FALLING);

  // Interval in millisecs
  // Be sure to place this HW Timer well ahead blocking calls, because it needs to be initialized.
  if (ITimer1.attachInterruptInterval(TIMER_INTERVAL_MS, HWCheckButton))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

  Blynk.begin(auth, ssid, pass, blynk_server, BLYNK_HARDWARE_PORT);

  if (Blynk.connected())
    Serial.println(F("Blynk connected"));
  else
    Serial.println(F("Blynk not connected yet"));

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  blynkTimer.setInterval(buttonInterval, checkButton);
}

void loop()
{
  Blynk.run();
  blynkTimer.run();
}
