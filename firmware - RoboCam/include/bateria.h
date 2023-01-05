#ifndef BATERIA_H_
#define BATERIA_H_

#include "power.h"
#include "prototypes.h"
extern power Power;
class BateriaManager
{
private:
    int NumberPinVin;
    int NumberPinV5;
    int NumberPinVs3;
    int NumberPinVs2;
    int NumberPinVs1;

    float ValueVref = 3.30;
    float ValueResolution = 4096.0;
    float ValueR1 = 100000.0;
    float ValueR2 = 10000.0;

    float ValueCarga = 0; // armazena o valor da carga em porcentagem
    float ValueTensaoMax = 0;
    float ValueTensaoMin = 0;

    bool StateReady = false; // flag para indicar que foi iniciado corretamente;

public:
    float ValueMedVin;
    float ValueMedV5;
    float ValueMedVs3;
    float ValueMedVs2;
    float ValueMedVs1;
    float ValueMedVtotal;

    float ValueVin; // tensão de entrada
    float ValueV5;  // tensão de 5v

    float ValueVs3;    // tensão das ultimas celulas
    float ValueVs2;    // tensão das celulas intermediarias
    float ValueVs1;    // tensão das primeiras celulas
    float ValueVtotal; // tensão dos bancos em serie;

    float ValuePercentVs3;   // valor percentual bank3
    float ValuePercentVs2;   // valor percentual bank2
    float ValuePercentVs1;   // valor percentual bank1
    float ValuePercentTotal; // valor percentual dos bancos em serie

    bool StateSerieOn;         // indica se a ligação em serie está ativa
    bool StateReturnDoca;      // flag para indicar se o robo deve voltar para a doca caso a bateria atinja um valor de tensão especificado
    bool StateCharging;        // indica se as baterias estão sendo carregadas.
    bool StateConnectedToDoca; // indica se está conectado a doca.

    unsigned long ValueTimeCheck;
    unsigned long contMedia;
    void begin();
    float correction(float valor);
    double analogReadAdjusted(byte pinNumber);
    void setVref(float value) { ValueVref = value; };
    void setResolution(float value) { ValueResolution = value; };
    void setR1(float value) { ValueR1 = value; };
    void setR2(float value) { ValueR2 = value; };
    void setTimeCheck(unsigned long value) { ValueTimeCheck = value; };

    void setPinVin(uint8_t pin) { NumberPinVin = pin; };
    void setPinV5(uint8_t pin) { NumberPinV5 = pin; };
    void setPinVs3(uint8_t pin) { NumberPinVs3 = pin; };
    void setPinVs2(uint8_t pin) { NumberPinVs2 = pin; };
    void setPinVs1(uint8_t pin) { NumberPinVs1 = pin; };

    float perCent(float value, float inferiorLimit, float upperLimit);

    void setLimitTension(float inferiorLimit, float upperLimit)
    {
        ValueTensaoMin = inferiorLimit;
        ValueTensaoMax = upperLimit;
    };

    bool inDoca();
    double getVin();

    void get();
    void check();
    float tensao(float vout, unsigned long R1, unsigned long R2);
    void showParam();

    BateriaManager(/* args */);
    ~BateriaManager();
};

BateriaManager::BateriaManager(/* args */)
{
}

BateriaManager::~BateriaManager()
{
}

/* converte os valores do ADC para tensão*/
float BateriaManager::tensao(float vout, unsigned long R1, unsigned long R2)
{
    // float vin;

    /* convertendo o valor do ADC para tensão*/
    vout = vout * ValueVref / ValueResolution;

    /* caso seja tensão advinda do divisor de tensão */
    if ((R1 != 0) && (R2 != 0))
    {
        vout = (R1 + R2) * vout / R2;
    }

    return vout;
}

/* configura os parametros para iniciar a classe*/
// void BateriaManager::begin(float vref, float resol, float r1, float r2, unsigned long time)
void BateriaManager::begin()
{
    setVref(VALUE_VREF);
    setResolution(VALUE_RESOL_ADC);
    setR1(VALUE_R1);
    setR2(VALUE_R2);
    setTimeCheck(0);
    setLimitTension(LIMIT_INF, LIMIT_SUP);
    setPinVin(PIN_VIN);
    setPinV5(PIN_5V);
    setPinVs3(PIN_S3);
    setPinVs2(PIN_S2);
    setPinVs1(PIN_S1);

    StateReady = true;
    ValueMedVin = 0;
    ValueMedV5 = 0;
    ValueMedVs3 = 0;
    ValueMedVs2 = 0;
    ValueMedVs1 = 0;
    ValueMedVtotal = 0;

    // Fazendo a primeira leitura
    get();
    contMedia = 1;
}

/* verifica se chegou o momento de fazer as leituras de tensão*/
void BateriaManager::check()
{
    if ((ValueTimeCheck == 0) && StateReady)
    {

        if (contMedia == 0)
        {
            get();
        }
        else
        {
            // Serial.println("[Bateria] ERROR CONT MEDIA = 0 ");
        }
        ValueVin = ValueMedVin / (float)contMedia;
        ValueV5 = ValueMedV5 / (float)contMedia;
        ValueVs3 = ValueMedVs3 / (float)contMedia;
        ValueVs2 = ValueMedVs2 / (float)contMedia;
        ValueVs1 = ValueMedVs1 / (float)contMedia;
        ValueVtotal = ValueMedVtotal / (float)contMedia;

        ValueVin = tensao(ValueVin, ValueR1, ValueR2);
        ValueV5 = tensao(ValueV5, ValueR1, ValueR2);
        ValueVs3 = tensao(ValueVs3, ValueR1, ValueR2);
        ValueVs2 = tensao(ValueVs2, ValueR1, ValueR2);
        ValueVs1 = tensao(ValueVs1, ValueR1, ValueR2);

        contMedia = 0;
        ValueMedVin = 0;
        ValueMedV5 = 0;
        ValueMedVs3 = 0;
        ValueMedVs2 = 0;
        ValueMedVs1 = 0;
        ValueMedVtotal = 0;

        /* Correção 1*/
        // ValueVs1 = ValueVs1 * 1.644;
        // ValueV5 = ValueV5 * 1.462;
        // ValueVs2 = ValueVs2 * 1.241;
        // ValueVs3 = ValueVs3 * 1.122;
        // ValueVin = ValueVin * 1.462;

        /* Correção 2*/
        // ValueVs1 = ValueVs1 * 0.9472;
        // // ValueV5 = ValueV5 * 1.462;
        // ValueVs2 = ValueVs2 * 0.9728;
        // ValueVs3 = ValueVs3 * 0.9880;
        // // ValueVin = ValueVin * 1.462;

        // Serial.println("\n============= ADC ===================");
        // Serial.println("Value no correction Vs1: " + String(ValueVs1));
        // Serial.println("Value no correction Vs2: " + String(ValueVs2));
        // Serial.println("Value no correction Vs3: " + String(ValueVs3));
        // Serial.println("Value no correction Vin: " + String(ValueVin));
        // Serial.println("Value no correction V5: " + String(ValueV5));

        ValueVs1 = ValueVs1 * correction(ValueVs1);
        ValueVs2 = ValueVs2 * correction(ValueVs2);
        ValueVs3 = ValueVs3 * correction(ValueVs3);
        ValueV5 = ValueV5 * correction(ValueV5);
        ValueVin = ValueVin * correction(ValueVin);

        ValuePercentVs1 = perCent(ValueVs1, ValueTensaoMin, ValueTensaoMax);
        ValuePercentVs2 = perCent(ValueVs2, ValueTensaoMin * 2.0, ValueTensaoMax * 2.0);
        ValuePercentVs3 = perCent(ValueVs3, ValueTensaoMin * 3.0, ValueTensaoMax * 3.0);

        // Serial.println("Value with correction Vs1: " + String(ValueVs1));
        // Serial.println("Value with correction Vs2: " + String(ValueVs2));
        // Serial.println("Value with correction Vs3: " + String(ValueVs3));
        // Serial.println("Value with correction Vin: " + String(ValueVin));
        // Serial.println("Value with correction V5: " + String(ValueV5));

        /* verifica se precisa ir para a doca para recarregar */
        if ((StateReturnDoca == false) && (StateSerieOn == true) && (ValueVs3 <= LIMIT_INF))
        {
            Serial.println("[BateriaManager::check] Bateria baixa: ir para doca");
            Serial.println("[BateriaManager::check] StateReturnDoca: true");
            StateReturnDoca = true;
        }
        else if ((StateReturnDoca == false) && (ValueVs1 <= LIMIT_INF))
        {
            Serial.println("[BateriaManager::check] Bank 1 baixo: ir para doca");
            Serial.println("[BateriaManager::check] StateReturnDoca: true");
            StateReturnDoca = true;
        }
        else if (StateReturnDoca && (ValueVs1 >= (LIMIT_INF * 1.4)) && (ValueVs3 >= (LIMIT_INF * 1.4)))
        {
            Serial.println("[BateriaManager::check] StateReturnDoca: false");
            StateReturnDoca = false;
        }

        /* verifica a ligação dos bancos de bateria */
        // if ((StateSerieOn == false) && (ValueVin >= 4) && (Power.getRele() == false))
        if ((Power.getRele() == false) && (StateSerieOn == false))
        {
            Serial.println("[BateriaManager::check] Ligação em Serie identificada.");
            // Serial.println("[BateriaManager::check] StateSerieOn: true");
            StateSerieOn = true;
        }
        else if ((ValueVin >= 4) && (Power.getRele()) && StateSerieOn)
        // else if ((StateSerieOn) && (ValueVin < 4) && (Power.getRele()))
        // else if (Power.getRele())
        {
            Serial.println("[BateriaManager::check] Ligação em Serie não identificada.");
            // Serial.println("[BateriaManager::check] StateSerieOn: false");
            StateSerieOn = false;
        }

        /* verifica a conexão com a doca */
        // if ((ValueVin >= 4) && (StateConnectedToDoca == false))
        if ((ValueVin >= 4) && (StateConnectedToDoca == false))
        {
            Serial.println("[BateriaManager::check] Conexão com a doca identificada.");
            // Serial.println("[BateriaManager::check] StateConnectedToDoca: true");
            // Serial.println("[BateriaManager::check] StateReturnDoca: false");
            StateReturnDoca = false;
            StateConnectedToDoca = true;
            StateCharging = true;
        }
        else if ((ValueVin < 4) && (StateConnectedToDoca == true))
        {
            Serial.println("[BateriaManager::check] Desconexão com a doca identificada.");
            Serial.println("[BateriaManager::check] StateConnectedToDoca: false");
            StateConnectedToDoca = false;
            StateCharging = false;
        }

        /* verifica quais bancos estão sendo carregados */
        if (StateConnectedToDoca && (StateSerieOn == false) && (StateCharging == false))
        {
            Serial.println("[BateriaManager::check] Carregando bancos de baterias");
            Serial.println("[BateriaManager::check] StateCharging: true");
            StateCharging = true;
        }
        else if (StateSerieOn && StateConnectedToDoca && (StateCharging == false))
        {
            Serial.println("[BateriaManager::check] Carregando banco de baterias 1");
            Serial.println("[BateriaManager::check] StateCharging: true");
            StateCharging = true;
        }
        else if (StateCharging && (StateConnectedToDoca == false))
        {
            Serial.println("[BateriaManager::check] recarga não identificado");
            Serial.println("[BateriaManager::check] StateCharging: false");
            StateCharging = false;
        }

        ValueTimeCheck = RELOAD_CHECK_BAT;
    }
    else if ((ValueTimeCheck % 10) == 0)
    {
        get();
        // contMedia += 1;
    }
    else if (StateReady == false)
    {
        Serial.println("[BateriaManager::check] Existe parametro invalido.");
    }
}

/* faz as leitoras dos pinos de ADC*/
void BateriaManager::get()
{
    if ((NumberPinVin == 0) || (NumberPinV5 == 0) || (NumberPinVs3 == 0) || (NumberPinVs2 == 0) || (NumberPinVs1 == 0))
    {
        Serial.println("[BateriaManager::get] Existe pinos não definidos.");
        StateReady = false;
    }
    else
    {
        // Serial.println("NumberPinVs3: " + String(NumberPinVs3));
        // ValueMedVin += analogRead(NumberPinVin);
        // ValueMedV5 += analogRead(NumberPinV5);   // tensão de 5v
        // ValueMedVs3 += analogRead(NumberPinVs3); // tensão das ultimas celulas
        // ValueMedVs2 += analogRead(NumberPinVs2); // tensão das celulas intermediarias
        // ValueMedVs1 += analogRead(NumberPinVs1); // tensão das primeiras celulas

        ValueMedVin += analogReadAdjusted(NumberPinVin);
        ValueMedV5 += analogReadAdjusted(NumberPinV5);   // tensão de 5v
        ValueMedVs3 += analogReadAdjusted(NumberPinVs3); // tensão das ultimas celulas
        ValueMedVs2 += analogReadAdjusted(NumberPinVs2); // tensão das celulas intermediarias
        ValueMedVs1 += analogReadAdjusted(NumberPinVs1); // tensão das primeiras celulas

        // Serial.println("ValueMedVs1 Analog: " + String(ValueMedVs1));
        // Serial.println("ValueMedVs2 Analog: " + String(ValueMedVs2));
        // Serial.println("ValueMedVs3 Analog: " + String(ValueMedVs3));
        // Serial.println("ValueMedV5 Analog: " + String(ValueMedV5));
        // Serial.println("ValueMedVin Analog: " + String(ValueMedVin));
        // Serial.println("ContMedia: " + String(contMedia));
        contMedia += 1;
    }
}

float BateriaManager::perCent(float value, float inferiorLimit, float upperLimit)
{
    value = value - inferiorLimit;

    value = (value * 100) / (upperLimit - inferiorLimit);

    if (value > 100)
    {
        value = 100;
    }
    else if (value < 0)
    {
        value = 0;
    }

    return value;
}

void BateriaManager::showParam()
{
    Serial.println("Pino Vin: " + String(NumberPinVin));
    Serial.println("Pino V5: " + String(NumberPinV5));
    Serial.println("Pino Vs3: " + String(NumberPinVs3));
    Serial.println("Pino Vs2: " + String(NumberPinVs2));
    Serial.println("Pino Vs1: " + String(NumberPinVs1));

    Serial.println("Tensão de Referência: " + String(ValueVref));
    Serial.println("Resolução do ADC: " + String(ValueResolution));
    Serial.println("Valor de R1 em ohms: " + String(ValueR1));
    Serial.println("Valor de R2 em ohms: " + String(ValueR2));

    Serial.println("Valor de Ready: " + String(StateReady));
}

double BateriaManager::analogReadAdjusted(byte pinNumber)
{

    // Specify the adjustment factors.
    const double f1 = 1.7111361460487501e+001;
    const double f2 = 4.2319467860421662e+000;
    const double f3 = -1.9077375643188468e-002;
    const double f4 = 5.4338055402459246e-005;
    const double f5 = -8.7712931081088873e-008;
    const double f6 = 8.7526709101221588e-011;
    const double f7 = -5.6536248553232152e-014;
    const double f8 = 2.4073049082147032e-017;
    const double f9 = -6.7106284580950781e-021;
    const double f10 = 1.1781963823253708e-024;
    const double f11 = -1.1818752813719799e-028;
    const double f12 = 5.1642864552256602e-033;

    // Specify the number of loops for one measurement.
    const int loops = 10;

    // Specify the delay between the loops.
    const int loopDelay = 0;

    // Initialize the used variables.
    int counter = 1;
    int inputValue = 0;
    double totalInputValue = 0;
    double averageInputValue = 0;

    // Loop to get the average of different analog values.
    for (counter = 1; counter <= loops; counter++)
    {

        // Read the analog value.
        inputValue = analogRead(pinNumber);

        // Add the analog value to the total.
        totalInputValue += inputValue;

        // Wait some time after each loop.
        // delay(loopDelay);
    }

    // Calculate the average input value.
    averageInputValue = totalInputValue / loops;

    // Calculate and return the adjusted input value.
    return f1 + f2 * pow(averageInputValue, 1) + f3 * pow(averageInputValue, 2) + f4 * pow(averageInputValue, 3) + f5 * pow(averageInputValue, 4) + f6 * pow(averageInputValue, 5) + f7 * pow(averageInputValue, 6) + f8 * pow(averageInputValue, 7) + f9 * pow(averageInputValue, 8) + f10 * pow(averageInputValue, 9) + f11 * pow(averageInputValue, 10) + f12 * pow(averageInputValue, 11);
}

float BateriaManager::correction(float valor)
{
    valor = (0.0049 * valor) + 0.9275;
    return valor;
}

bool BateriaManager::inDoca()
{
    double vin = getVin();
    if (vin >= 4)
    {
        return true;
    }else
    {
        return false;
    }
}

double BateriaManager::getVin()
{
    double getVin = analogReadAdjusted(NumberPinVin);
    getVin = tensao(getVin, ValueR1, ValueR2);
    return getVin;
}
#endif
