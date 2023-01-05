#ifndef SERVOSG90_H_
#define SERVOSG90_H_

#include "prototypes.h"

extern power Power;
// void SERVO_set_angle(RoboCan *MovServo, Servo *ServPan, Servo *ServTilt);
void SERVO_update(RoboCan *MovServo, Servo *ServPan, Servo *ServTilt);
int SERVO_write(int valueAngle, int currentAngle, int timePerAngle, Servo *servo);

void servo_begin()
{
    MyMoves.timePerAnglePan = TIME_PER_ANGLE;
    MyMoves.timePerAngleTilt = TIME_PER_ANGLE;
    
    servoPan.attach(OUTPIN_SERVO_PAN);
    servoTilt.attach(OUTPIN_SERVO_TILT);
    MyMoves.valueCanPan = servoPan.read();
    MyMoves.valueCanTilt = servoTilt.read();

    MyMoves.movCanTilt = true;
    MyMoves.valueCanTilt = 45;

    MyMoves.movCanPan = true;
    MyMoves.valueCanPan = 90;
}

void SERVO_update(RoboCan *MovServo, Servo *ServPan, Servo *ServTilt)
{
    // garantindo que exista tensão na saida;
    if (bateria.ValueV5 < 4)
    {
        Power.IP5306On();
    }

    pca.write_SW_SG90(HIGH);

    if (MovServo->movCanPan)
    {
        MovServo->movCanPan = false;
        MovServo->currentAnglePan = SERVO_write(MovServo->valueCanPan, MovServo->currentAnglePan, MovServo->timePerAnglePan, ServPan);
        /*teste*/
        MovServo->currentAnglePan = ServPan->read();
    }

    if (MovServo->movCanTilt)
    {
        MovServo->movCanTilt = false;
        MovServo->currentAngleTilt = SERVO_write(MovServo->valueCanTilt, MovServo->currentAngleTilt, MovServo->timePerAngleTilt, ServTilt);
        /*teste*/
        MovServo->currentAngleTilt = ServTilt->read();
    }
}
int SERVO_write(int valueAngle, int currentAngle, int timePerAngle, Servo *servo)
{
    Serial.println("SERVO_write:"+String(valueAngle));

    if ((valueAngle - currentAngle) > 0)
    {
        /*increment angle*/
        while (currentAngle <= valueAngle)
        {
            // Serial.println(".");
            currentAngle += 1;
            servo->write(currentAngle);
            delay(timePerAngle);
        }
    }
    else if ((valueAngle - currentAngle) < 0)
    {
        /*decrement angle*/
        while (currentAngle >= valueAngle)
        {
            // Serial.println("..");
            currentAngle -= 1;
            servo->write(currentAngle);
            delay(timePerAngle);
        }
    }

    return currentAngle;
}

#endif