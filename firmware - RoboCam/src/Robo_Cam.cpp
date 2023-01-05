/*
   Software desenvolvido para controle de um Robô com uma câmera acoplada
   que move sobre um cabo nas duas direçoes possiveis além de realizar os
   movimentos PAN e TILT em sua câmera acoplada, a comunicação do Robô é
   via WiFi, atraves da plataforma Blynk.
*/

#include "prototypes.h"

// struct Verify wifi;
// struct Verify blynk;
struct ESP_32 MyESP32;
struct RoboCan MyMoves;

const char esp32_rede[20] = "Game_Play_LanH";
const char esp32_pass[20] = "25061997"; // j85295360

char blynkToken[SIZE_BLYNK_TOKEN] = "Your_tokem_blynk";
char blynkServer[SIZE_BLYNK_SERVER] = "blynk-cloud.com";
char blynkPort[SIZE_BLYNK_PORT] = "80";

hw_timer_t *timer = NULL;

unsigned long current_time = 0;
bool state_led = false;

unsigned long timeCheckWifi = 0;
unsigned long timeUpdateRTC = 0;

bool scriptPage = false;

Servo servoPan;
Servo servoTilt;

WidgetTerminal terminal(TERMINAL_BLYNK);

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

WiFiServer server(SERVER_PORT);
// WebServer server(SERVER_PORT);
WiFiClient serverClients[1];
WiFiClient client; // Listen for incoming clients

BateriaManager bateria;

PCF8574 pca8574(PCA_ADDRESS_I2C);
PCA pca;

// Script myScript;
struct menuhtml menu;

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP);

serial mySerial;

// power Power;
interpreter myInterpreter;

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";
stepper_ myStepper;
void setup()
{
  Serial.begin(BAUD_RATE); // configurando comunicação serial
  Serial.println("[setup] Iniciando...");

  io_begin(); // configurando pinos
  pca.begin();

  INTERRUPT_setup(TIME_INTERRUPT_US); // configurando interrupções
  digitalWrite(LED_BORD, true);

  Power.begin();
  Power.DisableAll();

  // step_begin(); // configurando o motor de passo
  myStepper.begin(); // configurando o motor de passo

  // connect_manager(blynkToken, blynkServer, blynkPort);
  // BLYNK_connect(blynkToken, blynkServer, blynkPort);

  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP(SSID_AP, PASS_AP);

  connect_wifi();

  // Programming_OTA();
  OTA.begin(HOST_NAME);

  if (!MDNS.begin(HOST_NAME))
  {
    Serial.print("\nerro ao iniciar mDNS, tentando novamente...");
  }

  handle_begin();

  servo_begin();

  // server.begin();

  rtc_begin();

  Serial.println("[setup] OK");

  terminal.println("Start!");
  terminal.flush();

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Serial.println("http://" + String(HOST_NAME) + ADDR_HOME);
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println(ADDR_HOME);

  /* timeUpdateRTC deve ser zero nesse ponto para fazer a primeira atualização */
  rtc_update();

  timeCheckWifi = RELOAD_WIFI_CHECK;
  // timeUpdateRTC = RELOAD_TIME_UPDATE_RTC;
  // bateria.ValueTimeCheck = RELOAD_CHECK_BAT/10;

  bateria.begin();
  server.begin();
}

void loop()
{
  // Blynk.run();
  if (digitalRead(INPIN_IR_COURSE_01) == 0)
  {
    // stepper->forceStop();
  }

  if (digitalRead(INPIN_IR_COURSE_02) == 0)
  {
    // stepper->forceStop();
  }

  OTA.run(); // programming OTA

  connection_wifi_check(&timeCheckWifi); // verifica o status da conexão wifi

  handle_interface(); // verifica se existe clientes conectados ao servidor

  step_update(); // executa algum movimento caso exista alguma solicitação

  SERVO_update(&MyMoves, &servoPan, &servoTilt); // executa algum movimento caso exista alguma solicitação

  rtc_update(); // atualiza data e hora

  bateria.check(); // verifica a bateria

  mySerial.check(); // verifica se existe dados no buffer serial

  /* deep_sleep mode */
  // esp_sleep_enable_timer_wakeup(10000000);
  // esp_deep_sleep_start();

  /* light sleep mode*/
  // esp_sleep_enable_timer_wakeup(10000000); // 1 second
  // int ret = esp_light_sleep_start();
  // Serial.printf("lp: %d\n", ret);
}

BLYNK_WRITE(RESET_WIFI_V11)
{
  int value = param.asInt(); // Get value as integer
  Serial.println("Reset");

  terminal.println("Reset!");
  terminal.flush();

  if (value)
  {
    WiFiManager wifiConnect;
    wifiConnect.resetSettings();
    WiFi.disconnect();
    // TimeCheck = 1;
    SPIFFS.format();
    ESP.restart();
  }
}
