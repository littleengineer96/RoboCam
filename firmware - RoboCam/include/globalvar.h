#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include "prototypes.h"
// #include "script.h"
// #include "interpreter.h"
// extern struct Verify wifi;
// extern struct Verify blynk;
extern struct ESP_32 MyESP32;
extern struct RoboCan MyMoves;

extern WidgetTerminal terminal;

extern const char esp32_rede[20];
extern const char esp32_pass[20];

// extern char blynk_token[35];
// extern char blynk_server[17];
// extern char blynk_port[5];

extern hw_timer_t *timer;

// extern bool bTouchState;
// extern bool f_Reconnect_WIFI;
// extern bool f_Reconnect_Blynk;
extern unsigned long current_time;
extern bool state_led;

extern unsigned long timeCheckWifi;
extern unsigned long timeUpdateRTC;

// extern unsigned long TimeCheck;
// extern unsigned long TimeOutConnect;
// extern unsigned long TimeOut;
// extern unsigned long CountTime;

// void inite(uint8_t chg_pin);

// extern String output2State;

extern Servo servoPan;
extern Servo servoTilt;

// extern WidgetTerminal terminal();

// extern FastAccelStepperEngine engine;
// extern FastAccelStepper *stepper;

extern WiFiServer server;
extern WiFiClient client;
// extern WebServer server;


extern FastAccelStepperEngine engine;
extern FastAccelStepper *stepper;
extern BateriaManager bateria;
extern PCF8574 pca8574;

extern bool scriptPage;
// extern Script myScript;
extern struct menuhtml menu;

extern WiFiUDP ntpUDP;
extern NTPClient ntp;
// extern power Power;
// extern interpreter myInterpreter;
// extern interpreter myInterpreter;
#endif