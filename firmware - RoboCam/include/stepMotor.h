#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H

#include "prototypes.h"
#include "power.h"
extern PCA pca;
extern power Power;
extern bool EnableEndOfCourse01;
// String _stepCurrentConfig;           // string global armazena a configuração de passo atual
// unsigned long _stepAround;    // passos por volta
unsigned long currentStep;          // numero de passo atual
unsigned long currentStepRequested; // passos solicitados
unsigned long currentStepTotal;     // é a soma de currentStepRequested + currentStep = totalSteps

class stepper_
{
private:
    unsigned long _pulsesTransitionAround;          // número de transiçoes de pulsos por volta (estimado 8)
    unsigned long _pulsesTransitionTotalyWorkspace; // número total de transições de pulsos no workspace
    unsigned long _pulsesActiveAround;              // número de pulsos com o sensor ativo em 0 por volta
    unsigned long _pulsesActiveTotaly;              // número total de pulsos com o sensor ativo em 0 no workspace
    unsigned long _pulsesInactiveAround;            // número de pulsos com o sensor inativo em 1 por volta
    unsigned long _pulsesInactiveTotaly;            // número total de pulsos com o sensor inativo em 0 no workspace

    String _stepCurrentConfig;                 // configuração atual de passos
    int _stepCurrentConfigFator;               // fator de conversão entre as configurações
    unsigned long _stepTotaly;                 // numero total de passos
    unsigned long _stepRequested;              // numero de passos solicitados
    unsigned long _stepCurrent;                // numero de passo atual
    unsigned long _stepAround;                 // numero de passos por volta
    unsigned long _stepPerPulsesActive;        // número de passos por pulso ativo (sensor em 0)
    unsigned long _stepPerPulsesInactive;      // número de passos por pulso inativo (sensor em 1)
    unsigned long _stepTotalyWorkspace;        // número total de passos no workspace
    unsigned long _stepMaxActiveSwich;         // maximum number of steps with active limit switch
    unsigned long _stepCurrentTotalyWorkspace; // está de acordo com a configuração de passos
    unsigned long _stepCurrentMaxPossible;     // maior numero de passos possivel
    unsigned long _stepCurrentPosition;        // posição atual
    bool _roboInDoca;                          // deve ser true quanto na doca
    bool _dirDoca;                             // deve ser true se na direção da doca
    bool _sleeping;                            // deve ser true quando sleep ativo
    bool _pulseOldState;                       // estado antigo do pulso true = ativo
    bool _stepSurplus;                         // passos execedente
    bool _stateDirRight;                       // deve ser true se para esquerda (right)
    bool _stateDirLeft;                        // dever ser true se para direita (left)
    bool _endCourseProtect;                    // proteção de fim de curso
    bool _recognizeWorkspace;

public:
    void begin();
    void recognizeWorkspace();
    void fullStep();
    void halfStep();
    void quarterStep();
    void eighthStep();
    void sixteenthStep();
    void resolution(uint8_t ms1, uint8_t ms2, uint8_t ms3);
    int unberth();
    void dock();
    void Lefth();
    void Right();

    void Move(unsigned long steps);
    void Check();

    /*acordado*/
    void awake()
    {
        Power.SleepA4988Off();
        Power.ReleDisable();
    };

    /* dormindo */
    void sleeping()
    {
        Power.SleepA4988On();
        Power.ReleEnable();
        _sleeping = true;
    };

    /* seto para a direção da doca */
    void dirDoca()
    {
        Lefth();
        _dirDoca = true;
    };

    /* seto para a direção oposta a doca */
    void dirNotDoca()
    {
        Right();
        _dirDoca = false;
    }

    bool roboInDoca()
    {
        _roboInDoca = bateria.inDoca();
        return _roboInDoca;
    }
    void protecOn() { _endCourseProtect = true; };
    void protecOff() { _endCourseProtect = false; };
    bool posibleMoviment(int *step);

    bool goToLeft();
    bool goToRight();
    bool returnDoca();
    bool endCourseLefth() { return !digitalRead(INPIN_IR_COURSE_01); };
    bool endCourseRight() { return !digitalRead(INPIN_IR_COURSE_02); };
    bool getPulses() { return digitalRead(INPIN_IR_LINHA); };
    bool getSleep() { return _sleeping; };
    bool getDirDoca() { return _dirDoca; };
    bool getRoboInDoca() { return _roboInDoca; };

    unsigned long getPulsesTransitionAround() { return _pulsesTransitionAround; };
    unsigned long getPulsesActiveAround() { return _pulsesActiveAround; };
    unsigned long getPulsesInactiveAround() { return _pulsesInactiveAround; };
    unsigned long getPulsesTransitionTotaly() { return _pulsesTransitionTotalyWorkspace; };
    unsigned long getPulsesActiveTotaly() { return _pulsesActiveTotaly; };
    unsigned long getPulsesInactiveTotaly() { return _pulsesInactiveTotaly; };

    // unsigned long getStepTotalyWorkspace() { return _stepTotalyWorkspace; };
    unsigned long getStepTotalyWorkspace() { return _stepCurrentTotalyWorkspace; };
    unsigned long getStepCurrentPosition() { return _stepCurrentPosition; };

    unsigned long getStepPerPulseActive() { return _stepPerPulsesActive; };
    unsigned long getStepPerPulseInactive() { return _stepPerPulsesInactive; };

    void setStepConfig(String config);
    String getStepConfig() { return _stepCurrentConfig; };

    stepper_(/* args */);
    ~stepper_();
};

stepper_::stepper_(/* args */)
{
}

stepper_::~stepper_()
{
}

void stepper_::begin()
{
    // _stepCurrentConfig = "Full Step";
    // _stepAround = 200;

    engine.init();
    digitalWrite(OUTPIN_A4988_SLEEP, HIGH);
    stepper = engine.stepperConnectToPin(OUTPIN_A4988_STEP);
    if (stepper)
    {
        stepper->setDirectionPin(OUTPIN_A4988_DIR);
        stepper->setEnablePin(OUTPIN_A4988_EN, false);
        // stepper->setAutoEnable(true);
        // If auto enable/disable need delays, just add (one or both):
        // stepper->setDelayToEnable(50);
        // stepper->setDelayToDisable(1000);
        // stepper->setSpeedInUs(100); // the parameter is us/step !!!
        // stepper->setAcceleration(500);
        stepper->setSpeedInUs(150); // the parameter is us/step !!!
        stepper->setAcceleration(200);
        // stepper->stopMove(1000);
        // stepper->move(10000);
    }

    fullStep();
    Lefth();
    _stepMaxActiveSwich = _stepAround;
    _endCourseProtect = true;
}
void step_update()
{
    if (stepper && (MyMoves.movRoboLeft || MyMoves.movRoboRight))
    {
        Serial.println("Step Move");
        if (MyMoves.movRoboLeft)
        {
            stepper->setDirectionPin(OUTPIN_A4988_DIR, true);
            MyMoves.movRoboLeft = false;
        }
        else
        {
            stepper->setDirectionPin(OUTPIN_A4988_DIR, false);
            MyMoves.movRoboRight = false;
        }
        stepper->move(10000);
    }
}

void stepper_::resolution(uint8_t ms1, uint8_t ms2, uint8_t ms3)
{
    uint8_t ret = 0;
    if ((ms1 == 0) && (ms2 == 0) && (ms3 == 0))
    {
        Serial.println("Full Step");
        _stepCurrentConfig = "Full Step";
        _stepAround = 200;
        _stepCurrentConfigFator = 1;
        _stepCurrentTotalyWorkspace = _stepTotalyWorkspace / 2;
    }
    else if ((ms1 == 1) && (ms2 == 0) && (ms3 == 0))
    {
        Serial.println("Half Step");
        _stepCurrentConfig = "Half Step";
        _stepAround = 400;
        _stepCurrentConfigFator = 2;
        _stepCurrentTotalyWorkspace = _stepTotalyWorkspace;
    }
    else if ((ms1 == 0) && (ms2 == 1) && (ms3 == 0))
    {
        Serial.println("Quarter Step");
        _stepCurrentConfig = "Quarter Step";
        _stepAround = 800;
        _stepCurrentConfigFator = 4;
        _stepCurrentTotalyWorkspace = _stepTotalyWorkspace * 4;
    }
    else if ((ms1 == 1) && (ms2 == 1) && (ms3 == 0))
    {
        Serial.println("Eighth Step");
        _stepCurrentConfig = "Eighth Step";
        _stepAround = 1600;
        _stepCurrentConfigFator = 8;
        _stepCurrentTotalyWorkspace = _stepTotalyWorkspace * 6;
    }
    else if ((ms1 == 1) && (ms2 == 1) && (ms3 == 1))
    {
        Serial.println("Sixteenth Step");
        _stepCurrentConfig = "Sixteenth Step";
        _stepAround = 3200;
        _stepCurrentConfigFator = 16;
        _stepCurrentTotalyWorkspace = _stepTotalyWorkspace * 16;
    }
    else
    {
        Serial.println("Invalid microstep resolution");
        ret = 1; // indica erro
    }

    if (ret == 0)
    {
        pca.write_M1_A4988(ms1);
        pca.write_M2_A4988(ms2);
        pca.write_M3_A4988(ms3);
    }
}
void stepper_::setStepConfig(String config)
{
    if (config == "Full Step")
    {
        fullStep();
    }
    else if (config == "Half Step")
    {
        halfStep();
    }
    else if (config == "Quarter Step")
    {
        quarterStep();
    }
    else if (config == "Eighth Step")
    {
        eighthStep();
    }
    else if (config == "Sixteenth Step")
    {
        sixteenthStep();
    }
}
void stepper_::fullStep()
{
    resolution(0, 0, 0);
}

void stepper_::halfStep()
{
    resolution(1, 0, 0);
}

void stepper_::quarterStep()
{
    resolution(0, 1, 0);
}

void stepper_::eighthStep()
{
    resolution(1, 1, 0);
}

void stepper_::sixteenthStep()
{
    resolution(1, 1, 1);
}

void stepCommandInterpreter(String line)
{
    String command;
    String argument;

    int end = line.indexOf("\r\n");
    command = line.substring(0, 1);
    argument = line.substring(1, end);

    int arg = argument.toInt();
}

void stepper_::Lefth()
{
    stepper->setDirectionPin(OUTPIN_A4988_DIR, true);
    stepper->stopMove();
    delay(900);
    stepper->forceStop();
    _stateDirLeft = true;
    _stateDirRight = false;
    delay(100);
}

void stepper_::Right()
{
    stepper->setDirectionPin(OUTPIN_A4988_DIR, false);
    stepper->stopMove();
    delay(900);
    stepper->forceStop();
    _stateDirRight = true;
    _stateDirLeft = false;
    delay(100);
}

void stepper_::Move(unsigned long steps)
{
    if (_stateDirRight && _stepCurrentTotalyWorkspace)
    {
        if ((_stepCurrentPosition + steps) > _stepCurrentTotalyWorkspace)
        {
            unsigned long stepExedente = (_stepCurrentPosition + steps) - _stepCurrentTotalyWorkspace;
            if (stepExedente <= steps)
            {
                steps -= stepExedente;
            }
        }
    }
    else if (_stateDirLeft && _stepCurrentTotalyWorkspace)
    {
        if (steps > _stepCurrentPosition)
        {
            unsigned long stepExedente = steps - _stepCurrentPosition;
            steps -= stepExedente;
        }
    }

    _stepRequested = steps;
    _stepTotaly += steps;

    Power.SleepA4988Off();
    stepper->move(steps);

    if (_stateDirRight)
    {
        _stepCurrentPosition += steps;
    }
    else if (_stateDirLeft)
    {
        if (steps < _stepCurrentPosition)
        {
            _stepCurrentPosition -= steps;
        }
        else
        {
            _stepCurrentPosition = 0;
        }
    }
}

void stepper_::Check()
{
    if (stepper->getCurrentPosition() == currentStepTotal)
    { /* fim do movimento */
        currentStep = currentStepTotal;
        currentStep = 0;
        Power.SleepA4988On();
    }
}

bool stepper_::returnDoca()
{
    String function = "[stepper_::returnDoca]";

    EnableEndOfCourse01 = false;

    if (roboInDoca())
    {
        dirNotDoca();
        Serial.println(function + "Robo na doca");
    }
    else
    {
        Serial.println(function + "Robo fora da doca");
        dirDoca();
        String aux = _stepCurrentConfig;
        // halfStep();
        awake();

        _stepMaxActiveSwich = _stepAround / 2; // 1 volta completa é muito

        goToLeft();

        if (endCourseLefth())
        {
            for (size_t j = 0; j < _stepMaxActiveSwich; j++)
            {

                if (roboInDoca())
                {
                    Serial.println(function + "Robo na doca");
                    _roboInDoca = true;
                    stepper->forceStop();
                    delay(10);
                    Right();
                    break;
                }
                stepper->move(1);
                delay(10);
            }

            if (_roboInDoca == false)
            {
                Serial.println(function + "Erro: doca desenergizada ou _stepMaxActiveSwich baixo");
            }
        }
        else
        {
            Serial.println(function + "Erro ao se aproximar da doca - TIMEOUT");
        }

        setStepConfig(aux);
    }

    EnableEndOfCourse01 = true;
    return _roboInDoca;
}

void stepper_::recognizeWorkspace()
{
    String function = "[stepper_::recognizeWorkspace]";

    _recognizeWorkspace = false;
    _stepTotalyWorkspace = 0;
    _stepCurrentTotalyWorkspace = 0;

    if (goToLeft() == false)
    {
        Serial.println(function + "erro ao ir para a doca");
        return;
    }

    if (goToLeft())
    {
        stepper->clearPulseCounter();
        _stepCurrent = 0;
        _stepTotaly = 0;
        _stepRequested = 0;
        _stepTotalyWorkspace = 0;

        _pulsesTransitionAround = 0;
        _pulsesTransitionTotalyWorkspace = 0;
        _pulsesActiveAround = 0;
        _pulsesActiveTotaly = 0;
        _pulsesInactiveAround = 0;
        _pulsesInactiveTotaly = 0;

        _stepTotaly = 0;
        _stepRequested = 0;
        _stepCurrent = 0;
        _stepPerPulsesActive = 0;
        _stepPerPulsesInactive = 0;
        _stepTotalyWorkspace = 0;
        _stepSurplus = 0;
        _stepCurrentTotalyWorkspace = 0;
        _stepCurrentMaxPossible = 0;
        _stepCurrentPosition = 0;

        Serial.println(function + "Robo na doca");
        // Right();
        String aux = _stepCurrentConfig;
        halfStep();

        _pulseOldState = getPulses();
        Serial.println(function + "Zerando Variaveis");

        bool aux1 = false;
        bool aux2 = true;
        dirNotDoca();
        delay(1000);

        unsigned long timeOut = 60000;
        unsigned long t0 = millis();

        while (timeOut > (millis() - t0))
        {
            if (endCourseRight())
            {
                stepper->forceStop();
                dirDoca();
                break;
            }
            else
            {
                Move(1);

                _stepTotalyWorkspace += 1;

                /* transições */
                if (_pulseOldState != getPulses())
                {
                    if (_stepSurplus == 0)
                    {
                        _stepSurplus = _stepTotalyWorkspace;
                    }

                    _pulsesTransitionTotalyWorkspace += 1;
                    _pulseOldState = getPulses();

                    if ((_stepTotalyWorkspace >= 4 * _stepAround) && (_stepTotalyWorkspace <= (_stepAround)*5))
                    {
                        _pulsesTransitionAround += 1;
                    }

                    if (_pulseOldState == false)
                    {
                        _pulsesActiveTotaly += 1;
                        if ((_stepTotalyWorkspace >= 4 * _stepAround) && (_stepTotalyWorkspace <= (_stepAround)*5))
                        {
                            _pulsesActiveAround += 1;
                        }
                    }
                    else
                    {
                        _pulsesInactiveTotaly += 1;
                        if ((_stepTotalyWorkspace >= 4 * _stepAround) && (_stepTotalyWorkspace <= (_stepAround)*5))
                        {
                            _pulsesInactiveAround += 1;
                        }
                    }
                }

                /* espera acontecer a primeira transição para estar sincronizado com o ciclo */
                /* sensor ativo */
                if ((_pulseOldState == false) && ((_stepTotalyWorkspace >= 4 * _stepAround) && (_stepTotalyWorkspace <= (_stepAround)*5)) && (aux1 == false))
                {
                    if (aux1 == false)
                    {
                        _stepPerPulsesActive += 1;
                        aux1 = getPulses();
                    }
                }

                /* sensor inativo*/
                else if ((_pulseOldState == true) && ((_stepTotalyWorkspace >= 4 * _stepAround) && (_stepTotalyWorkspace <= (_stepAround)*5)) && aux2)
                {
                    if (aux2)
                    {
                        _stepPerPulsesInactive += 1;
                        aux2 = getPulses();
                    }
                }
                delay(2);
            }
        }

        if (endCourseRight() == false)
        {
            Serial.println(function + "Erro ao se aproximar do fim de curso right");
        }
        else
        {
            _recognizeWorkspace = true;
        }

        setStepConfig(aux);
        _stepCurrentPosition = _stepCurrentTotalyWorkspace;
    }
    else
    {
        Serial.println(function + "Erro ao se aproximar da doca");
    }
    // EnableEndOfCourse01 = true;
}

bool stepper_::posibleMoviment(int *step)
{
    /*movimento impossivel */
    if (_stateDirLeft && endCourseLefth())
    {
        return false;
    }
    /*movimento impossivel*/
    else if (_stateDirRight && endCourseRight())
    {
        return false;
    }
    /*movimento possivel*/
    else
    {
        if (roboInDoca() && _stateDirRight)
        {
            int stepNecessary = unberth();
        }

        return true;
    }
}

/*destracar da doca*/
int stepper_::unberth()
{
    int steps = 0;
    // if (roboInDoca() || endCourseLefth())
    if (roboInDoca())
    {
        String aux = _stepCurrentConfig;
        halfStep();

        unsigned long timeOut = 60000;
        unsigned long t0 = millis();
        while (timeOut > (millis() - t0))
        {
            if (endCourseLefth() == false)
            {
                break;
            }
            stepper->move(1);
            steps += 1;
        }

        if (endCourseLefth() == false)
        {
            Lefth();
            unsigned long timeOut = 60000;
            unsigned long t0 = millis();
            while (timeOut > (millis() - t0))
            {
                if (endCourseLefth())
                {
                    break;
                }
                stepper->move(1);
            }
        }
        Right();
        steps = steps / _stepCurrentConfigFator;
        setStepConfig(aux);
    }
    return steps;
}
void stepper_::dock()
{
}

/*vai para direita*/
bool stepper_::goToLeft()
{
    if (endCourseLefth() == false)
    {
        String auxStepCurrentConfig = _stepCurrentConfig;
        Lefth();
        if (_recognizeWorkspace)
        {
            if (_stepCurrentPosition > 1500)
            {
                Move(_stepCurrentPosition - 500);
            }
        }

        unsigned long timeOut = 60000;
        unsigned long t0 = millis();
        while (timeOut > (millis() - t0))
        {
            if (endCourseLefth())
            {
                break;
            }
            stepper->move(1);
            delay(1);
        }
        setStepConfig(auxStepCurrentConfig);
        _stepCurrentPosition = 0;
    }

    return endCourseLefth();
}

/* vai para esquerda */
bool stepper_::goToRight()
{
    if (endCourseRight() == false)
    {
        String auxStepCurrentConfig = _stepCurrentConfig;
        Right();
        // halfStep();
        if (_recognizeWorkspace)
        {
            if ((_stepCurrentTotalyWorkspace - _stepCurrentPosition) > 1500)
            {
                Move(_stepCurrentPosition + 500);
            }
        }

        unsigned long timeOut = 60000;
        unsigned long t0 = millis();
        while (timeOut > (millis() - t0))
        {
            if (endCourseRight())
            {
                break;
            }

            Move(1);
            // steps += 1;
        }

        setStepConfig(auxStepCurrentConfig);
        _stepCurrentPosition = _stepCurrentTotalyWorkspace;
    }

    return endCourseRight();
}

#endif