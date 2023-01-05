#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include "prototypes.h"

#define BLYNK_PRINT Serial
#define BAUD_RATE 115200
#define DATA_STATE_ADDRESS "/dataState.txt"

// ========= VERSION =========
#define VERSION "1v0-"             // versão 1.0
#define SERIE "1"                  // serie 2
#define LOTE "A"                   // lote "A"
#define DEVICE_NAME "Robo_1v0" // nome completo do dispositivo

// ========= BLYNK =========
#define SIZE_TOKEN 32  // tamanho do token para verificação
#define SIZE_SERVER 17 // tamanho do token para verificação
#define SIZE_PORT 5    // tamanho do token para verificação

// ========= TIMES =========
#define TIMEOUT_PORTAL 80                            // time in s
#define TIME_INTERRUPT_US 10000                      // tempo em us
#define TIME_INTERRUPT_MS (TIME_INTERRUPT_US / 1000) // tempo em ms

#define TIME_ISR_RESET 250                                                    // tempo em milisegundos, deve ser 1000 para que TIMER_AP seja inserido em segundos
#define TIMER_AP 150                                                          // tempo  1s = 2   150 = 75s
#define TIME_UPDATE_RTC 3600                                                  // time in secunds tempo de atualização do rtc
#define RELOAD_TIME_UPDATE_RTC ((TIME_UPDATE_RTC / TIME_INTERRUPT_MS) * 1000) // tempo considerando o valor atual de interrupção

#define BLYNK_LED_NOT_NET 50    // time in ms
#define BLYNK_LED_NET 1000      // time in ms
#define BLYNK_LED_ERRO 400      // time in ms
#define BLYNK_LED_NOT_ERRO 1000 // time in ms

#define LED_NOT_NET (BLYNK_LED_NOT_NET / TIME_INTERRUPT_MS)
#define LED_NET (BLYNK_LED_NET / TIME_INTERRUPT_MS)
#define LED_NOT_ERRO (BLYNK_LED_NOT_ERRO / TIME_INTERRUPT_MS)
#define LED_ERRO (BLYNK_LED_ERRO / TIME_INTERRUPT_MS)

// ========= ADC PARAM =========
#define LIMIT_INF 3.3
#define LIMIT_SUP 4.2
#define VALUE_R1 100000.0
#define VALUE_R2 10000.0
#define VALUE_VREF 3.30
#define VALUE_RESOL_ADC 4096.0
#define CHECK_BAT 10 // time in s
#define RELOAD_CHECK_BAT ((CHECK_BAT / TIME_INTERRUPT_MS) * 1000)

// ========= ADC PINS =========
#define PIN_VIN 36
#define PIN_5V 34
#define PIN_S3 35
#define PIN_S2 32
#define PIN_S1 39

// connect
#define SUCCESS 1
#define ERRO 0
#define DEBUG 0
#define TRY_AGAIN_CONNECT 4
#define TIME_CHECK_CONNECTION 30 // SEGUNDOS
#define TIMEOUT_CONNECT 200      //

#define LED_BORD 2
#define ATTEMPTS 3
#define SAMPLES 10

//=========== blynk virtual pins ========================
#define MOV_CAN_PAN V1
#define MOV_CAN_TILT V2
#define MOV_ROBO_RIGHT V3
#define MOV_ROBO_LEFT V4
#define TERMINAL_BLYNK V10
#define RESET_WIFI_V11 V11

//=========== value max end min data tratament =========
#define PAN_VALUE_MIN 0
#define PAN_VALUE_MAX 180
#define TILT_VALUE_MIN 0
#define TILT_VALUE_MAX 180
#define RIGHT_VALUE_MIN 0
#define RIGHT_VALUE_MAX 10000
#define LEFT_VALUE_MIN 0
#define LEFT_VALUE_MAX 10000

//=========== pins ESP32 =================
#define LED_PIN 2 // TXD

// ========= SERVOS =========
#define TIME_PER_ANGLE 10 //in ms
// #define OUTPIN_SERVO_PAN 33
// #define OUTPIN_SERVO_TILT 32
#define OUTPIN_SERVO_PAN 33
#define OUTPIN_SERVO_TILT 25

// Driver motor de passo
#define OUTPIN_A4988_STEP 17
#define OUTPIN_A4988_DIR 16
#define OUTPIN_A4988_EN 23   // habilitado em low
#define OUTPIN_A4988_SLEEP 5 // habilitado em low

#define INPIN_IR_LINHA 26
#define INPIN_IR_COURSE_01 14
#define INPIN_IR_COURSE_02 27

#define OUTPIN_LED_ERRO 12
#define OUTPIN_LED_NET 2

#define OUTPIN_SW_ESPCAM 13
#define OUTPIN_SW_RELE 19

#define INPIN_OPEN_AP 15
#define INPIN_IO0 0
#define OUTPIN_KEY_IP5306 18
#define IP5306_LIGAR_SAIDA 2200 // tempo em ms
#define IP5306_LIGAR_LEDS 500   // tempo em ms

#define I2C_PIN_SDA 21
#define I2C_PIN_SCL 22
#define RX_PIN 34
#define TX_PIN 35

#define PCA_PIN_M1_A4988 0
#define PCA_PIN_M2_A4988 1
#define PCA_PIN_M3_A4988 2
#define PCA_PIN_RST_A4988 3 // habilitado em low
#define PCA_PIN_EXTRA_01 4
#define PCA_PIN_SW_SG90 5
#define PCA_PIN_SW_IR 6
#define PCA_PIN_EXTRA_02 7

#define PCA_ADDRESS_I2C 0x20

// #define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 10 /* Time ESP32 will go to sleep (in seconds) */

//====== SERVER ====
#define SERVER_PORT 80
#define HOST_NAME "robo"
#define ADDR_SCRIPT "/script"
#define ADDR_HOME "/home"
#define ADDR_SCRIPTS_LIST "/scriptsList.txt"

#define ADDR_WIFI_CONF "/configWiFi.json.json"
enum State
{                           //                                                         LED_ON    LED_OFF
    NOT_CONNECTED_ST,       // acabou de iniciar não conectado. LED STATICO   CONST
    CONNECTING_ST,          // conectando...                    LED RAPIDO    160ms    100ms
    CONNECTED_ST,           // conectado ao Wifi!               LED LENTO     3000ms   500ms
    DISCONNECTED_ST,        // disconexão!                     LED MEDIO     500ms    500ms
    BLYNK_NOT_CONNECTED_ST, // não conectado ao blynk
    BLYNK_CONNECTING_ST,    // conectando ao Blynk
    BLYNK_CONNECTED_ST,     // conectado ao Blynk
    BLYNK_DISCONNECTED_ST   // desconectado do Blynk
};

enum StateVerify
{
    NOT_CONNECTED_VERIFY = 10, //
    CONNECTING_VERIFY = 3,     // quantidade de vezes
    CONNECTION_ATTEMPTS = 3,
    CONNECTED_VERIFY = 15000,    // tempo em ms
    DISCONNECTED_VERIFY = 1,     // quantidade de vezes
    BLYNK_CONNECTING_VERIFY = 3, // quantidade de vezes
    BLYNK_CONNECTION_ATTEMPTS = 3,
    BLYNK_CONNECTED_VERIFY = 15000,        // tempo em ms
    OFF_MODE_BLYNK_CONNECTING_VERIFY = 14, // quantidade de vezes que a conexão com o Blynk é tentada em modo offline 14x15s=210segundos
    BLYNK_DISCONNECTED_VERIFY = 2          // quantidade de vezes
};

// struct Verify
// {
//     int ConectionState = -1;           // stato da conexao
//     int ConnectionAttemptsCounter = 0; // contador de tentativas de conexao

//     int ConnectingVerify = 0;     // variavel de controle
//     int NotConnectedVerify = 0;   // variavel de controle
//     int ConnectedVerify = 0;      // variavel de controle
//     int DisconnectedVerify = 0;   // variavel de controle
//     unsigned long TimeVerify = 0; // variavel de controle
// };

struct ESP_32
{
    int CurrentState = 0; // stato da conexao
    int CurrentAttempts = 0;
    bool ConnectedWiFi = false;
    bool ConnectedBlynk = false;
    bool OfflineMode = true;
    bool Carrying = false;
    bool ReturnDoca = false;
    bool TurnOnCAM = false;
    bool Reconnect = false;
    bool CheckConnection = false;
};

struct RoboCan
{
    bool movCanPan = false;
    int valueCanPan = 0;
    int currentAnglePan = 0;
    unsigned long timePerAnglePan = 0;

    bool movCanTilt = false;
    int valueCanTilt = 0;
    int currentAngleTilt = 0;
    unsigned long timePerAngleTilt = 0;

    bool movRoboRight = false;
    int valueRoboRight = 0;

    bool movRoboLeft = false;
    int valueRoboLeft = 0;
};

struct BlynkLed
{
    unsigned long NETledOn;
    unsigned long NETledOff;

    unsigned long ERROledOn;
    unsigned long ERROledOff;
};

// struct pca
// {
// uint8_t M1_A4988;
// uint8_t M2_A4988;
// uint8_t M3_A4988;
// uint8_t RST_A498;
// uint8_t EXTRA_01;
// uint8_t SW_SG90 ;
// uint8_t SW_IR;
// uint8_t EXTRA_02;

// };

struct menuhtml
{
    uint8_t page;
    String botton1; // home page
    String botton2; // script page
    String botton3; // save button href
    String botton4; // save button type
    String botton5; // help page
    String botton6; // agendamento page
    String botton7; // control page
    String botton8; //wifi page
};

enum PageNumber
{
    _PAGE_HOME,
    _PAGE_SCRIPT,
    _PAGE_SCRIPT_NEW,
    _PAGE_SCRIPT_OPEN,
    _PAGE_SCRIPT_RUN,
    _PAGE_SCRIPT_RUN_1,
    _PAGE_MANG_SCRIPT,
    _PAGE_CONTROL,
    _PAGE_AGEND_SCRIPT,
    _PAGE_HELP_COMAND,
    _PAGE_WIFI,
    _PAGE_NOT_FOUND
};

#endif