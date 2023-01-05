#ifndef POWER_H_
#define POWER_H_

#include "prototypes.h"
#include "pca.h"
extern PCA pca;

class power
{
private:
    int EspCamPin;
    int RelePin;
    int IP5306Pin;
    int ServoPin;
    int SenseIRPin;
    int SleepA4988Pin;

    bool StateEspCam;
    bool StateRele;
    bool StateIP5306;
    bool StateServo;
    bool StateSenseIR;
    bool StateSleepA4988;

public:
    void begin();
    void setEspCam(int pin)
    {
        EspCamPin = pin;
    };
    void setRele(int pin)
    {
        RelePin = pin;
    };
    void setIP5306(int pin)
    {
        IP5306Pin = pin;
    };
    void setServo(int pin)
    {
        ServoPin = pin;
    };
    void setSenseIR(int pin)
    {
        SenseIRPin = pin;
    };
    void setSleepA4988(int pin)
    {
        SleepA4988Pin = pin;
    };

    void turnOn(int pin);
    void turnOff(int pin);

    /* enable */
    void EspCamOn() { turnOn(EspCamPin); };
    void ReleEnable() { turnOn(RelePin); };
    void IP5306On() { turnOn(IP5306Pin); };
    void ServoOn() { turnOn(ServoPin); };
    void SenseIROn() { turnOn(SenseIRPin); };
    void SleepA4988On() { turnOn(SleepA4988Pin - 1); }; // -1 serve para eliminar o conflito com o ServoPin
    void EnableAll();
    
    /* disable */
    void EspCamOff() { turnOff(EspCamPin); };
    void ReleDisable() { turnOff(RelePin); };
    void IP5306Off() { turnOff(IP5306Pin); };
    void ServoOff() { turnOff(ServoPin); };
    void SenseIROff() { turnOff(SenseIRPin); };
    void SleepA4988Off() { turnOff(SleepA4988Pin - 1); }; // -1 serve para eliminar o conflito com o ServoPin
    void DisableAll();
    void showPins();

    bool getEspCam() { return StateEspCam; };
    bool getRele() { return StateRele; };
    bool getIP5306() { return StateIP5306; };
    bool getServo() { return StateServo; };
    bool getSenseIR() { return StateSenseIR; };
    bool getSleepA4988() { return StateSleepA4988; };

    power(/* args */);
    ~power();
};

power::power(/* args */)
{
}

power::~power()
{
}

void power::begin()
{
    EspCamPin = OUTPIN_SW_ESPCAM;
    RelePin = OUTPIN_SW_RELE;
    IP5306Pin = OUTPIN_KEY_IP5306;
    ServoPin = PCA_PIN_SW_SG90;
    SenseIRPin = PCA_PIN_SW_IR;
    SleepA4988Pin = OUTPIN_A4988_SLEEP;
}

void power::turnOn(int pin)
{
    switch (pin)
    {
    case OUTPIN_SW_ESPCAM:
        pinMode(EspCamPin, OUTPUT);
        digitalWrite(EspCamPin, HIGH);
        StateEspCam = true;
        break;
    case OUTPIN_SW_RELE:
        pinMode(RelePin, OUTPUT);
        digitalWrite(RelePin, LOW);
        StateRele = true;
        break;
    case OUTPIN_KEY_IP5306:
        pinMode(IP5306Pin, OUTPUT);
        digitalWrite(IP5306Pin, HIGH);
        delay(2200);
        digitalWrite(IP5306Pin, LOW);
        StateIP5306 = true;
        break;
    case PCA_PIN_SW_SG90:
        pca.setPin_SW_SG90(ServoPin);
        pca.write_SW_SG90(HIGH);
        StateServo = true;
        break;
    case PCA_PIN_SW_IR:
        pca.setPin_SW_IR(SenseIRPin);
        pca.write_SW_IR(LOW);
        StateSenseIR = true;
        break;
    case OUTPIN_A4988_SLEEP - 1:
        // SleepA4988Pin += 1;
        pinMode(SleepA4988Pin, OUTPUT);
        digitalWrite(SleepA4988Pin, LOW);
        StateSleepA4988 = true;
        break;
    default:
        break;
    }
}

void power::turnOff(int pin)
{
    switch (pin)
    {
    case OUTPIN_SW_ESPCAM:
        pinMode(EspCamPin, OUTPUT);
        digitalWrite(EspCamPin, LOW);
        StateEspCam = false;
        break;
    case OUTPIN_SW_RELE:
        pinMode(RelePin, OUTPUT);
        digitalWrite(RelePin, HIGH);
        StateRele = false;
        break;
    case OUTPIN_KEY_IP5306:
        pinMode(IP5306Pin, OUTPUT);
        digitalWrite(IP5306Pin, HIGH);
        delay(2200);
        digitalWrite(IP5306Pin, LOW);
        StateIP5306 = false;
        break;
    case PCA_PIN_SW_SG90:
        pca.write_SW_SG90(LOW);
        StateServo = false;
        break;
    case PCA_PIN_SW_IR:
        pca.write_SW_IR(HIGH);
        StateSenseIR = false;
        break;
    case OUTPIN_A4988_SLEEP - 1:
        // SleepA4988Pin += 1;
        Serial.println("SleepOff");
        pinMode(SleepA4988Pin, OUTPUT);
        digitalWrite(SleepA4988Pin, HIGH);
        StateSleepA4988 = false;
        break;
    default:
        break;
    }
}

void power::showPins()
{
    Serial.println("EspCamPin: " + String(EspCamPin));
    Serial.println("RelePin: " + String(RelePin));
    Serial.println("IP5306Pin: " + String(IP5306Pin));
    Serial.println("ServoPin: " + String(ServoPin));
    Serial.println("SenseIRPin: " + String(SenseIRPin));
    Serial.println("SleepA4988Pin: " + String(SleepA4988Pin));
}

void power::EnableAll()
{
  IP5306On();
  ServoOn();
  SleepA4988Off();
  EspCamOn();
  SenseIROn();
  ReleDisable(); 
}

void power::DisableAll()
{
  IP5306Off();
  ServoOff();
  SleepA4988On();
  EspCamOff();
  SenseIROff();
  ReleEnable();
}
power Power;
#endif