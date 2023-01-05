#ifndef MYBLYNK_H_
#define MYBLYNK_H_


#if ESP32
#include <BlynkSimpleEsp32.h>
#else
#include <BlynkSimpleEsp8266.h>
#endif

#include "prototypes.h"

//========== DECLARATION OF FUNCTION ======================
bool BLYNK_connected_wifi();
bool BLYNK_connected();
void BLYNK_connect(char token[], char server[], char port[]);

bool BLYNK_connected_wifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return false;
    }

    return true;
}

void BLYNK_connect(char token[], char server[], char port[])
{
    String pt = port;
    uint16_t portServer = pt.toInt();
    // Blynk.config(blynk_token, blynk_server, 8080);
    // Serial.println("Connectando ao Blynk server");
    // Serial.println("Token:" + String(token));
    // Serial.println("Server:" + String(server));
    // Serial.println("Port:" + String(portServer));

    Blynk.config(token, server, portServer);

    // return Blynk.connect();
    if (Blynk.connect())
    {
        Serial.println("\nConnected to Blynk ");
    }
    else
    {
        Serial.println("\nError Connect to Blynk ");
    }
}

bool BLYNK_connected()
{
    return Blynk.connected();
}

BLYNK_CONNECTED()
{
    // MyMoves.movCanPan = true;
    // MyMoves.valueCanPan = 90;
    // MyMoves.currentAnglePan = 90;

    // MyMoves.movCanTilt = true;
    // MyMoves.valueCanTilt = 90;
    // MyMoves.currentAngleTilt = 90;

}

BLYNK_WRITE(MOV_CAN_PAN)
{
    int value = param.asInt();
    if (value >= PAN_VALUE_MIN && value <= PAN_VALUE_MAX)
    {
        MyMoves.movCanPan = true;
        MyMoves.valueCanPan = param.asInt();
    }
    else
    {
        terminal.println("MOV_CAN_PAN: value outside range");
        terminal.flush();
    }
}

BLYNK_WRITE(MOV_CAN_TILT)
{
    int value = param.asInt();
    if (value >= TILT_VALUE_MIN && value <= TILT_VALUE_MAX)
    {
        MyMoves.movCanTilt = true;
        MyMoves.valueCanTilt = param.asInt();
    }
    else
    {
        terminal.println("MOV_CAN_TILT: value outside range");
        terminal.flush();
    }
}

BLYNK_WRITE(MOV_ROBO_RIGHT)
{

    MyMoves.valueRoboRight = param.asInt();

    if (MyMoves.valueRoboRight)
    {
        terminal.println("MOV_ROBO_RIGH!");
        terminal.flush();
        MyMoves.movRoboRight = true;
    }
    // else
    // {
    //   MyMoves.movRoboRight = false;
    // }
}

BLYNK_WRITE(MOV_ROBO_LEFT)
{
    MyMoves.valueRoboLeft = param.asInt();

    if (MyMoves.valueRoboLeft)
    {
        terminal.println("MOV_ROBO_LEFT!");
        terminal.flush();
        MyMoves.movRoboLeft = true;
    }
    // else
    // {
    //   MyMoves.movRoboLeft = false;
    // }
}

#endif