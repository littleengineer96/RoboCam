#ifndef PCA_H_
#define PCA_H_

#include "prototypes.h"
extern PCF8574 pca8574;
class PCA
{
private:
    /* data */
    uint8_t NumberPinM1_A4988;
    uint8_t NumberPinM2_A4988;
    uint8_t NumberPinM3_A4988;
    uint8_t NumberPinRST_A498;
    uint8_t NumberPinSW_SG90;
    uint8_t NumberPinSW_IR;
    uint8_t NumberPinEXTRA_01;
    uint8_t NumberPinEXTRA_02;
    // uint8_t AddressI2C_PCA = 0x20;
    bool StateReady = false; // flag para indicar que foi iniciado corretamente;
public:
    uint8_t M1_A4988;
    uint8_t M2_A4988;
    uint8_t M3_A4988;
    uint8_t RST_A498;
    uint8_t SW_SG90;
    uint8_t SW_IR;
    uint8_t EXTRA_01;
    uint8_t EXTRA_02;

    void begin();

    void set(uint8_t pin, uint8_t state) { digitalWrite(pca8574, pin, state); };
    void get(uint8_t pin) { pinMode(pca8574, pin, INPUT_PULLUP); };

    void setPin_M1_A4988(uint8_t pin)
    {
        pinMode(pca8574, pin, OUTPUT);
        NumberPinM1_A4988 = pin;
    };
    void setPin_M2_A4988(uint8_t pin)
    {
        pinMode(pca8574, pin, OUTPUT);
        NumberPinM2_A4988 = pin;
    };
    void setPin_M3_A4988(uint8_t pin)
    {
        pinMode(pca8574, pin, OUTPUT);
        NumberPinM3_A4988 = pin;
    };
    void setPin_RST_A4988(uint8_t pin)
    {
        pinMode(pca8574, pin, OUTPUT);
        NumberPinRST_A498 = pin;
    };
    void setPin_SW_SG90(uint8_t pin)
    {
        pinMode(pca8574, pin, OUTPUT);
        NumberPinSW_SG90 = pin;
    };
    void setPin_SW_IR(uint8_t pin)
    {
        pinMode(pca8574, pin, OUTPUT);
        NumberPinSW_IR = pin;
    };
    void setPin_EXTRA_01(uint8_t pin)
    {
        pinMode(pca8574, pin, OUTPUT);
        NumberPinEXTRA_01 = pin;
    };
    void setPin_EXTRA_02(uint8_t pin)
    {
        pinMode(pca8574, pin, OUTPUT);
        NumberPinEXTRA_02 = pin;
    };

    void write_M1_A4988(uint8_t state)
    {
        M1_A4988 = state;
        set(NumberPinM1_A4988, state);
    };
    void write_M2_A4988(uint8_t state)
    {
        M2_A4988 = state;
        set(NumberPinM2_A4988, state);
    };
    void write_M3_A4988(uint8_t state)
    {
        M3_A4988 = state;
        set(NumberPinM3_A4988, state);
    };
    void write_RST_A4988(uint8_t state)
    {
        RST_A498 = state;
        set(NumberPinRST_A498, state);
    };
    void write_SW_SG90(uint8_t state)
    {
        SW_SG90 = state;
        set(NumberPinSW_SG90, state);
    };
    void write_SW_IR(uint8_t state)
    {
        /* pin activ in low*/
        SW_IR = state;
        set(NumberPinSW_IR, state);
    };
    void write_EXTRA_01(uint8_t state)
    {
        EXTRA_01 = state;
        set(NumberPinEXTRA_01, state);
    };
    void write_EXTRA_02(uint8_t state)
    {
        EXTRA_02 = state;
        set(NumberPinEXTRA_02, state);
    };

    PCA(/* args */);
    ~PCA();
};

PCA::PCA(/* args */)
{
}

PCA::~PCA()
{
}

void PCA::begin()
{
    setPin_M1_A4988(PCA_PIN_M1_A4988);
    setPin_M2_A4988(PCA_PIN_M2_A4988);
    setPin_M3_A4988(PCA_PIN_M3_A4988);
    setPin_RST_A4988(PCA_PIN_RST_A4988);
    setPin_SW_SG90(PCA_PIN_SW_SG90);
    setPin_SW_IR(PCA_PIN_SW_IR);
    setPin_EXTRA_01(PCA_PIN_EXTRA_01);
    setPin_EXTRA_02(PCA_PIN_EXTRA_02);

    write_M1_A4988(LOW);
    write_M2_A4988(LOW);
    write_M3_A4988(LOW);
    write_RST_A4988(HIGH);
    write_SW_SG90(LOW);
    write_SW_IR(HIGH);
    write_EXTRA_01(LOW);
    write_EXTRA_02(LOW);

    StateReady = true;

    if (!(PCF8574(PCA_ADDRESS_I2C).read() != -1))
    {
        Serial.print("Not Found PCF8574: addr = 0x");
        Serial.println(PCA_ADDRESS_I2C, HEX);
    }
}

#endif