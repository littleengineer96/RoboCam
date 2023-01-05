#ifndef SERIAL_H_
#define SERIAL_H_

#include "prototypes.h"

class serial
{
private:
    /* data */
public:
    void check();
    void pinSet();
    int pinValid(int pin);

    serial(/* args */);
    ~serial();
};

serial::serial(/* args */)
{
}

serial::~serial()
{
}

void serial::check()
{
    if (Serial.available())
    {
        String msg = Serial.readString();

        msg.toUpperCase();

        int start = msg.indexOf("IO");
        int end = msg.indexOf(":");

        String pin;
        String value;

        if ((start >= 0) && (end >= 0))
        {
            Serial.println("\nReceivend: " + msg);

            /*IOpin:value ex: IO0:1*/
            pin = msg.substring(2, end);
            start = end + 1;
            end = msg.indexOf("\n\r");
            value = msg.substring(start, end);

            int pinNumber = pin.toInt();
            int valueNumber = pin.toInt();

            Serial.println("Pin: " + pin + " value: " + value);
            if ((pinNumber >= 0) && (pinNumber <= 33))
            {
                int valid = pinValid(pinNumber);
                if (valid == 0)
                {
                    Serial.println("Valid IO pin");
                    if ((valueNumber >= 0) && (valueNumber <= 1))
                    {
                        Serial.println("Valid IO value");
                    }
                    else
                    {
                        Serial.println("Invalid IO value");
                    }
                }
                else
                {
                    Serial.println("Invalid IO pin");
                }
            }
        }

        /* PCApin:value ex: PCA1:1*/
        start = msg.indexOf("PCA");
        end = msg.indexOf(":");

        if ((start >= 0) && (end >= 0))
        {
            Serial.println("\nReceivend: " + msg);
            
            pin = msg.substring(start + 3, end);

            start = end + 1;
            end = msg.indexOf("\n\r");
            value = msg.substring(start, end);

            int pinNumber = pin.toInt();
            int valueNumber = pin.toInt();

            if ((pinNumber >= 0) && (pinNumber <= 7))
            {
                Serial.println("Valid PCA pin");
                if ((valueNumber >= 0) && (valueNumber <= 1))
                {
                    Serial.println("Valid PCA value");
                }
                else
                {
                    Serial.println("Invalid PCA value");
                }
            }
            else
            {
                Serial.println("Invalid PCA pin");
            }
        }
    }
}

int serial::pinValid(int pin)
{
    int ret = 0;

    switch (pin)
    {
    case 1:
        ret = 1;
        break;
    case 3:
        ret = 1;
        break;
    case 6:
        ret = 1;
        break;
    case 7:
        ret = 1;
        break;
    case 8:
        ret = 1;
        break;
    case 9:
        ret = 1;
        break;
    case 10:
        ret = 1;
        break;
    case 11:
        ret = 1;
        break;
    case 20:
        ret = 1;
        break;
    case 24:
        ret = 1;
        break;
    case 28:
        ret = 1;
        break;
    case 29:
        ret = 1;
        break;
    case 30:
        ret = 1;
        break;
    case 31:
        ret = 1;
        break;
    default:
        break;
    }

    return ret;
}
#endif