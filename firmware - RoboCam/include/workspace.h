#ifndef WORKSPACE_H_
#define WORKSPACE_H_

#include "prototypes.h"

class workspace
{
private:
    /* data */
    String sideDoca;                   // direção da doca left ou right
    unsigned long totalWorkspaceSteps; // total de passos do espaço de trabalho
    unsigned long *stepsAround;        // passos por volta
    String stepConfig;                 // configuração de passos
    unsigned long pulsesPerAround;     // pulsos do sensor de infravermelho por volta
    unsigned long *stepTotal;          // armazena o total de passos
    bool inDoca;                       // deve ser verdadeira caso o robo esteja na doca
public:
    void begin();
    void recognize();
    workspace(/* args */);
    ~workspace();
};

workspace::workspace(/* args */)
{
}

workspace::~workspace()
{
}

void workspace::begin()
{
    sideDoca = "left";
    // stepsAround = &currentStepAround;
    stepTotal = &currentStep;
    pulsesPerAround = 4;
}

void workspace::recognize()
{
    /*defino a direção da doca*/
    if (sideDoca == "left")
    {
        // stepLeft();
    }

    inDoca = bateria.inDoca();

    if (inDoca == false)
    {
        
    }
    
}

#endif