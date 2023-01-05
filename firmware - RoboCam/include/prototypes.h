#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <BlynkSimpleEsp32.h>
// #include <Servo.h>
// #include "FastAccelStepper.h"
// #include <Arduino.h>
// #include <TimeLib.h>
// #include <BlynkSimpleEsp32.h>
// #include <WidgetRTC.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <SPI.h>
#include <Servo.h>
#include <ESPmDNS.h>
#include "FastAccelStepper.h"
#include <Wire.h>
#include <pcf8574.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>

#include "globaldef.h"
#include "bateria.h"
#include "ntp.h"
#include "globalvar.h"
#include "pca.h"
#include "stepMotor.h"
// #include "isr.h"
#include "memoriaFlash.h"
#include "saveStatus.h"
// #include "setting.h"
// #include "setupBlynk.h"
#include "setupOTA.h"
#include "MyBlynk.h"
#include "MyInterrupt.h"
#include "power.h"
#include "servoSG90.h"
#include "connection.h"
#include "io.h"
// #include "interpreter.h"
// #include "script.h"
// #include "interpreter.h"
#include "script.h"
#include "interface.h"
#include "serial.h"
// settings
void Setting_Pins(void);
void Comunication(int baud_rate);
void Input_Pins(void);
void Output_Pins(void);
void Initialize_Variables(void);
void Setting_Servo(void);

// ISR
void Start_Timer(uint64_t time_in_us);
void My_Timer();

// void saveConfigCallback();
// void MyWiFi();

// connection
int Assistant_WiFi();
int WiFi_Manager();
int Reconnect_WiFi();
int Blynk_Connect();
String WiFi_State();
bool WiFi_Connected();
void Save_Config_Callback();

void Programming_OTA(void);
// void isrInit();
// void resetESP(void *rEsp);
void blynkConnect(void);
// void blynkLed(void *rEsp);
void saveStatus(void);
void readStatus(void);

int BLYNK_connect();
void BLYNK_connection(unsigned int connection_attempts);

// MRFC522.H
// void MRFC522_get_id();
// void MRFC522_setup();

// stepMotor.h

#endif