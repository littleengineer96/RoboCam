#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "script.h"
#include "prototypes.h"
#include "power.h"
extern stepper_ myStepper;
// #include "stepMotor.h"
// class Script myScript;
// extern Script myScript;
// ==== STEP ====

// COMMAND WITH ARGUMENT
#define _COMMAND_MOVE_METER "MOVE METER:" // move em metros MOVE METER:10
#define _COMMAND_MOVE_STEP "MOVE STEP:"   // move em passos MOVE STEP:50

// COMMAND WITHOUT ARGUMENT
#define _COMMAND_LEFT_STEP "LEFT STEP"        // define a direção para a direita
#define _COMMAND_RIGHT_STEP "RIGHT STEP"      // define a direção para a esquerda
#define _COMMAND_STOP_STEP "STOP STEP"        // inicia a desaceleração até para
#define _COMMAND_FORCE_STOP_STEP "FSTOP STEP" // para imediatamente.

#define _COMMAND_FULL "FULL STEP"
#define _COMMAND_HALF "HALF STEP"
#define _COMMAND_QUARTER "QUARTER STEP"
#define _COMMAND_EIGHTH "EIGHTH STEP"
#define _COMMAND_SIXTEENTH "SIXTEENTH STEP"

#define _COMMAND_SLEEP_STEP_ON "SLEEP STEP ON"
#define _COMMAND_SLEEP_STEP_OFF "SLEEP STEP OFF"

#define _COMMAND_EN_STEP_ON "EN STEP ON"
#define _COMMAND_EN_STEP_OFF "EN STEP OFF"

#define _COMMAND_RST_STEP "RST STEP"

// ======== SERVO SG90 =======
#define _COMMAND_TILT "TILT:"
#define _COMMAND_PAN "PAN:"
#define _COMMAND_SERVO_ON "SERVO ON"
#define _COMMAND_SERVO_OFF "SERVO OFF"
#define _COMMAND_TIME_PER_ANGLE_TILT "TPA TILT:" // time per angle in ms
#define _COMMAND_TIME_PER_ANGLE_PAN "TPA PAN:"   // time per angle in ms

#define _COMMAND_RELE_OFF "RELE OFF" // disabilita o rele
#define _COMMAND_RELE_ON "RELE ON"   // habilita o rele

#define _COMMAND_STEP_SPEED "STEP SPEED:"
#define _COMMAND_STEP_ACEL "STEP ACEL:"
#define _COMMAND_CAM_ON "CAM ON"
#define _COMMAND_CAM_OFF "CAM OFF"

#define _COMMAND_CLEAR_LIST "CLEAR LIST"       // limpa a lista com nome dos scripts
#define _COMMAND_RST_DEV "RST DEV"             // reinicia o dispositivo
#define _COMMAND_SPIFFS_FORMAT "SPIFFS FORMAT" //   LIMPA TODA A MEMORIA

#define _COMMAND_DELAY "DELAY:" // delay em ms

#define _COMMAND_DISABLE_ALL "DISABLE ALL"
#define _COMMAND_ENABLE_ALL "ENABLE ALL"

#define _COMMAND_POWER_OFF "POWER OFF"

#define _COMMAND_SENSEIR_ON "IR ON"
#define _COMMAND_SENSEIR_OFF "IR OFF"

#define _COMMAND_RETURN_DOCA "RETURN DOCA"
#define _COMMAND_RECOGNIZE_WS "RECOGNIZE WS"

#define _COMMAND_PROTECT_ON "PROTECT ON"
#define _COMMAND_PROTECT_OFF "PROTECT OFF"

#define _COMMAND_GOTO_RIGHT "GOTO RIGHT"
#define _COMMAND_GOTO_LEFT "GOTO LEFT"

class interpreter
{
private:
    /* data */
public:
    int commandList(String script);
    int commandLine(String line, bool check);
    int run(String Command, int arg, bool check);
    interpreter(/* args */);
    ~interpreter();
};

interpreter::interpreter(/* args */)
{
}

interpreter::~interpreter()
{
}

int interpreter::commandLine(String line, bool check)
{
    int ret = 1;
    line.toUpperCase();

    // deve ter o tamando minimo igual ao tamanho do menor comando
    if (line.length() >= 4)
    {
        String arg, command;
        int argInt;

        // comando com argumento
        if (line.indexOf(":") > 0)
        {

            command = line.substring(0, line.indexOf(":") + 1);
            arg = line.substring(line.indexOf(":") + 1, line.length());
            argInt = arg.toInt();
        }
        else // comando sem argumento
        {
            command = line;
            argInt = -1;
        }

        Serial.println("Line: " + line);
        Serial.println("Command: " + command);
        Serial.println("Arg: " + arg);
        Serial.println("ArgInt: " + arg);

        ret = run(command, argInt, check);
        if (ret)
        {
            if (ret == 1)
            {
                Serial.println("[interpreter] run: command error");
            }
            else if (ret == 2)
            {
                Serial.println("[interpreter] run: argument error");
            }
            else
            {
                Serial.println("[interpreter] run: error");
            }
        }
    }

    return ret;
}
int interpreter::commandList(String script)
{
    String command;
    unsigned long numberCommands = 0;
    while (script.length() > 4)
    {
        command = script.substring(0, script.indexOf("\r\n"));
        script = script.substring(script.indexOf("\r\n") + 2, script.length());
        numberCommands += 1;
        Serial.println("[commandList] " + String(numberCommands) + ":" + command + " length:" + script.length());
        commandLine(command, false);
    }
    return 1;
}
/**
 * @brief
 * @param Command Comando a ser executado
 * @param arg Comando a ser executado
 * @param check setar para true caso apenas queira testar o comando
 * @return ret
 * @retval 0 = success
 * @retval 1 = comando não existe
 * @retval 2 = argumento invalido
 */
int interpreter::run(String Command, int arg, bool check)
{
    int ret = 1;

    if (Command.startsWith(_COMMAND_MOVE_STEP))
    {
        if ((arg > 0) && (arg < 999999))
        {
            if (check == false)
            {
                if (myStepper.posibleMoviment(&arg))
                {
                    // stepper->move(arg);
                    myStepper.Move(arg);
                }
            }
            ret = 0;
        }
        else
        {
            ret = 2;
        }
    }
    else if (Command.startsWith(_COMMAND_MOVE_METER))
    {
        if ((arg > 0) && (arg < 999999))
        {
            if (check == false)
            {
                stepper->moveTo(arg);
            }
            ret = 0;
        }
        else
        {
            ret = 2;
        }
    }
    else if (Command.startsWith(_COMMAND_LEFT_STEP))
    {
        if (check == false)
        {
            // stepper->setDirectionPin(OUTPIN_A4988_DIR, true);
            // stepLeft();
            myStepper.Lefth();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_RIGHT_STEP))
    {
        if (check == false)
        {
            // stepper->setDirectionPin(OUTPIN_A4988_DIR, false);
            // stepRigh();
            myStepper.Right();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_STOP_STEP))
    {
        if (check == false)
        {
            stepper->stopMove();
        }

        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_FORCE_STOP_STEP))
    {
        if (check == false)
        {
            stepper->forceStop();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_FULL))
    {
        if (check == false)
        {
            myStepper.fullStep();
        }

        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_HALF))
    {
        if (check == false)
        {
            myStepper.halfStep();
        }

        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_QUARTER))
    {
        if (check == false)
        {
            myStepper.quarterStep();
        }

        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_EIGHTH))
    {
        if (check == false)
        {
            myStepper.eighthStep();
        }

        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_SIXTEENTH))
    {
        if (check == false)
        {
            myStepper.sixteenthStep();
        }

        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_SLEEP_STEP_ON))
    {
        if (check == false)
        {
            Power.SleepA4988On();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_SLEEP_STEP_OFF))
    {
        if (check == false)
        {
            Power.SleepA4988Off();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_EN_STEP_ON))
    {
        if (check == false)
        {
            /* code */
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_EN_STEP_OFF))
    {
        if (check == false)
        {
            /* code */
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_RST_STEP))
    {
        if (check == false)
        {
            /* code */
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_TILT))
    {
        if ((arg >= 0) && (arg <= 180))
        {
            if (check == false)
            {
                MyMoves.movCanTilt = true;
                MyMoves.valueCanTilt = arg;
                SERVO_update(&MyMoves, &servoPan, &servoTilt);
            }
            ret = 0;
        }
        else
        {
            ret = 2;
        }
    }
    else if (Command.startsWith(_COMMAND_TIME_PER_ANGLE_TILT))
    {
        if ((arg >= 0) && (arg <= 10000))
        {
            if (check == false)
            {
                MyMoves.timePerAngleTilt = arg;
            }
            ret = 0;
        }
        else
        {
            ret = 2;
        }
    }

    else if (Command.startsWith(_COMMAND_PAN))
    {
        if ((arg >= 0) && (arg <= 180))
        {
            if (check == false)
            {
                MyMoves.movCanPan = true;
                MyMoves.valueCanPan = arg;
                SERVO_update(&MyMoves, &servoPan, &servoTilt);
            }
            ret = 0;
        }
        else
        {
            ret = 2;
        }
    }
    else if (Command.startsWith(_COMMAND_TIME_PER_ANGLE_PAN))
    {
        if ((arg >= 0) && (arg <= 10000))
        {
            if (check == false)
            {
                MyMoves.timePerAnglePan = arg;
            }
            ret = 0;
        }
        else
        {
            ret = 2;
        }
    }
    else if (Command.startsWith(_COMMAND_SERVO_ON))
    {
        if (check == false)
        {
            Power.ServoOn();
        }

        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_SERVO_OFF))
    {
        if (check == false)
        {
            Power.ServoOff();
        }

        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_RELE_ON))
    {
        if (check == false)
        {
            Power.ReleEnable();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_RELE_OFF))
    {
        if (check == false)
        {
            Power.ReleDisable();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_STEP_SPEED))
    {
        if ((arg >= 50) && (arg <= 10000))
        {
            if (check == false)
            {
                stepper->setSpeedInUs(arg);
            }
            ret = 0;
        }
        else
        {
            ret = 2;
        }
    }
    else if (Command.startsWith(_COMMAND_STEP_ACEL))
    {
        if ((arg >= 0) && (arg <= 200))
        {
            if (check == false)
            {
                stepper->setAcceleration(arg);
            }
            ret = 0;
        }
        else
        {
            ret = 2;
        }
    }
    else if (Command.startsWith(_COMMAND_CAM_ON))
    {
        if (check == false)
        {
            Power.EspCamOn();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_CAM_OFF))
    {
        if (check == false)
        {
            Power.EspCamOff();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_CLEAR_LIST))
    {
        if (check == false)
        {
            if (SPIFFS.begin(true))
            {
                File configFile = SPIFFS.open(ADDR_SCRIPTS_LIST, "w");
                configFile.close();
            }
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_RST_DEV))
    {
        if (check == false)
        {
            // ESP.restart();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_SPIFFS_FORMAT))
    {
        if (check == false)
        {
            SPIFFS.format();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_DELAY))
    {
        if ((arg > 0) && (arg <= 100000))
        {
            if (check == false)
            {
                delay(arg);
            }
            ret = 0;
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_ENABLE_ALL))
    {
        if (check == false)
        {
            Power.EnableAll();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_DISABLE_ALL))
    {
        if (check == false)
        {
            Power.DisableAll();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_POWER_OFF))
    {
        if (check == false)
        {
            // Power.DisableAll();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_SENSEIR_ON))
    {
        if (check == false)
        {
            Power.SenseIROn();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_SENSEIR_OFF))
    {
        if (check == false)
        {
            Power.SenseIROff();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_RETURN_DOCA))
    {
        if (check == false)
        {
            // Power.SenseIROff();
            // myStepper.recognizeWorkspace();
            myStepper.returnDoca();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_RECOGNIZE_WS))
    {
        if (check == false)
        {
            Power.SenseIROn();
            Power.ReleDisable();
            Power.SleepA4988Off();
            myStepper.recognizeWorkspace();

            // myStepper.returnDoca();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_PROTECT_ON))
    {
        if (check == false)
        {
            myStepper.protecOn();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_PROTECT_OFF))
    {
        if (check == false)
        {
            myStepper.protecOff();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_GOTO_LEFT))
    {
        if (check == false)
        {
            myStepper.goToLeft();
        }
        ret = 0;
    }
    else if (Command.startsWith(_COMMAND_GOTO_RIGHT))
    {
        if (check == false)
        {
            myStepper.goToRight();
        }
        ret = 0;
    }
    return ret;
}

#endif