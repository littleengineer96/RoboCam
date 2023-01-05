#ifndef IO_H_
#define IO_H_

#include "prototypes.h"
extern stepper_ myStepper;

void io_begin();
void io_setup();
void io_inputs();
void io_outputs();
void io_interruptsInput();
void IRAM_ATTR lineIR();
void IRAM_ATTR endOfCourse01();
void IRAM_ATTR endOfCourse02();

bool EnableEndOfCourse01; 
bool EnableEndOfCourse02;
bool EnableLineIR;
void io_begin()
{
    io_setup();
}

void io_setup()
{
    io_inputs();
    io_outputs();
    io_interruptsInput();
}

void io_inputs()
{
    // pinMode(INPIN_IR_LINHA, INPUT);
    // pinMode(INPIN_IR_COURSE_01, INPUT_PULLUP);
    // pinMode(INPIN_IR_COURSE_02, INPUT_PULLUP);
    pinMode(INPIN_OPEN_AP, INPUT);
    pinMode(INPIN_IO0, INPUT);
}

void io_outputs()
{
    pinMode(OUTPIN_SERVO_PAN, OUTPUT);
    pinMode(OUTPIN_SERVO_TILT, OUTPUT);

    pinMode(OUTPIN_A4988_STEP, OUTPUT);
    pinMode(OUTPIN_A4988_DIR, OUTPUT);
    pinMode(OUTPIN_A4988_EN, OUTPUT);
    pinMode(OUTPIN_A4988_SLEEP, OUTPUT);

    pinMode(OUTPIN_LED_ERRO, OUTPUT);
    pinMode(OUTPIN_LED_NET, OUTPUT);
    pinMode(OUTPIN_SW_ESPCAM, OUTPUT);
    pinMode(OUTPIN_SW_RELE, OUTPUT);
    pinMode(OUTPIN_KEY_IP5306, OUTPUT);

    pinMode(LED_BORD, OUTPUT);
}

void io_interruptsInput()
{
    pinMode(INPIN_IR_LINHA, INPUT);
    pinMode(INPIN_IR_COURSE_01, INPUT_PULLUP);
    pinMode(INPIN_IR_COURSE_02, INPUT_PULLUP);

    // attachInterrupt(INPIN_IR_LINHA, lineIR, CHANGE);
    attachInterrupt(INPIN_IR_COURSE_01, endOfCourse01, FALLING);
    attachInterrupt(INPIN_IR_COURSE_02, endOfCourse02, FALLING);

    EnableEndOfCourse01 = true;
    EnableEndOfCourse02 = true;
}

void IRAM_ATTR lineIR()
{
}

void IRAM_ATTR endOfCourse01()
{
    if (EnableEndOfCourse01 && myStepper.endCourseLefth())
    {
        stepper->forceStop();
    }
    
}

void IRAM_ATTR endOfCourse02()
{
    if (EnableEndOfCourse02 && myStepper.endCourseRight())
    {
        stepper->forceStop();
    }
}

#endif