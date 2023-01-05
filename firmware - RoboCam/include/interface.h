#ifndef INTERFACE_H_
#define INTERFACE_H_

// #include "script.h"
#include "prototypes.h"
#include "interpreter.h"
extern Script myScript;
extern WiFiUDP ntpUDP;
extern NTPClient ntp;

extern interpreter myInterpreter;
bool bRet = false;
String sRet;
String ssidAux;
String auxRede;
bool bRedeResult = false;
void handle_htmlStart();
void handle_homePage();
void handle_scriptPage();
void handle_interface();
void handle_agendScriptPage();
void handle_helpCommandPage();
void handle_scriptManPage();
void handle_notFoundPage();
void handle_controlPage();
void handle_scriptNew();
void handle_scriptOpen();
void handle_scriptRun();
void handle_scriptRun1();
void handle_wifi();

String handgle_decode(String msg);

void handleNotFound()
{
    // String message = "Server is running!\n\n";
    // message += "URI: ";
    // message += server.uri();
    // message += "\nMethod: ";
    // message += (server.method() == HTTP_GET) ? "GET" : "POST";
    // message += "\nArguments: ";
    // message += server.args();
    // message += "\n";
    // server.send(200, "text / plain", message);
}

void handle_interface()
{
    // Variable to store the HTTP request
    String header;

    // Current time
    unsigned long currentTime = millis();
    // Previous time
    unsigned long previousTime = 0;
    // Define timeout time in milliseconds (example: 2000ms = 2s)
    const long timeoutTime = 1000;

    // WiFiClient client = server.available(); // Listen for incoming clients
    client = server.available(); // Listen for incoming clients

    // WiFiClient client = server.client();
    if (client)
    { // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("[interface] New Client"); // print a message out in the serial port
        String currentLine = "";                  // make a String to hold incoming data from the client

        while (client.connected() && ((currentTime - previousTime) <= timeoutTime))
        { // loop while the client's connected
            currentTime = millis();
            // Serial.println("New Client -> while.");
            // print a message out in the serial port

            if (client.available())
            {
                // print a message out in the serial port
                // if there's bytes to read from the client,
                // char c = client.read(); // read a byte, then
                // Serial.write(c);        // print it out the serial monitor
                // header += c;

                Serial.println(".");
                header = client.readString();
                // header = client.readStringUntil('HTTP');

                // getReceived = header.substring(header.indexOf("GET /"),header.indexOf("HTTP"));
                Serial.println(" === Client Received === \n" + header);

                // // if (c == '\n')

                /**/
                // if (!client.available())
                // { // if the byte is a newline character
                //     // if the current line is blank, you got two newline characters in a row.
                //     // that's the end of the client HTTP request, so send a response:

                //     Serial.println("..");
                //     if (currentLine.length() == 0)
                //     {
                /**/

                Serial.println("...");
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();

                // turns the GPIOs on and off
                if (header.indexOf("GET /outMovRight/on") >= 0)
                {
                    MyMoves.movRoboRight = true;
                }
                else if (header.indexOf("GET /outMovLeft/on") >= 0)
                {
                    MyMoves.movRoboLeft = true;
                }
                else if (header.indexOf("GET /outMovPan/right") >= 0)
                {
                    MyMoves.movCanPan = true;
                    MyMoves.valueCanPan = MyMoves.currentAnglePan;
                    if (MyMoves.valueCanPan <= 178)
                    {
                        MyMoves.valueCanPan += 2;
                    }
                }
                else if (header.indexOf("GET /outMovPan/left") >= 0)
                {
                    MyMoves.movCanPan = true;
                    MyMoves.valueCanPan = MyMoves.currentAnglePan;
                    if (MyMoves.valueCanPan >= 2)
                    {
                        MyMoves.valueCanPan -= 2;
                    }
                }
                else if (header.indexOf("GET /outMovTilt/up") >= 0)
                {
                    MyMoves.movCanTilt = true;
                    MyMoves.valueCanTilt = MyMoves.currentAngleTilt;
                    if (MyMoves.valueCanTilt <= 178)
                    {
                        MyMoves.valueCanTilt += 2;
                    }
                }
                else if (header.indexOf("GET /outMovTilt/down") >= 0)
                {
                    MyMoves.movCanTilt = true;
                    MyMoves.valueCanTilt = MyMoves.currentAngleTilt;
                    if (MyMoves.valueCanTilt >= 2)
                    {
                        MyMoves.valueCanTilt -= 2;
                    }
                }
                else if (header.indexOf("GET /send") >= 0)
                {
                    Serial.println("Script Recebido!!!");
                    myScript.code = header;

                    // int start = myScript.code.indexOf("FSCRIPT=");
                    int start = myScript.code.indexOf("&fscript=");
                    myScript.name = myScript.code.substring(myScript.code.indexOf("?sname=") + 7, start);

                    int end = myScript.code.indexOf("HTTP");
                    int end1 = myScript.code.indexOf("%0D%0A HTTP");
                    myScript.code.toUpperCase();
                    if ((end >= 0) && (end1 >= 0) && (end1 < end))
                    {
                        end = end1;
                        // Serial.println("termina com: %0D%0A HTTP");
                    }
                    else
                    {
                        end = end;
                        // Serial.println("termina com: HTTP");
                    }

                    if ((start >= 0) && (end >= 0) && (end > start))
                    {
                        myScript.stateNewScript = true;

                        Serial.println("start: " + String(start));
                        Serial.println("end: " + String(end));
                        // script = script.substring((script.indexOf("FSCRIPT=S") + 8), script.indexOf("HTTP"));
                        myScript.code = myScript.code.substring((start + 9), end);

                        myScript.code += "\r\n"; // corrigindo possiveis problemas de identificação de fim de script

                        /* corriguindo problemas de codificação */
                        // myScript.code.replace("0D", "\r");
                        // myScript.code.replace("0A", "\n");
                        // myScript.code.replace("%", "");
                        // myScript.code.replace("+", "");
                        // myScript.code.replace("3C", "<");
                        // myScript.code.replace("3A", ":");
                        myScript.code = handgle_decode(myScript.code);
                        myScript.name = handgle_decode(myScript.name);
                        Serial.println("Name: " + myScript.name);
                        Serial.println("Script:\n" + myScript.code);
                        myScript.check();
                    }
                    else
                    {
                        Serial.println("Script INVALIDO!");
                        myScript.code = "";
                    }
                }
                else if (header.indexOf("GET /savescript") >= 0)
                {
                    bRet = true;
                    Serial.println("Salvar Script:" + myScript.name);
                    Serial.println(myScript.codeFeedback);

                    uint8_t ret = myScript.save();
                    if (ret == 0)
                    {
                        sRet = "Script salvo com sucesso";
                        myScript.name = "";
                        myScript.codeFeedback = "";
                    }
                    else if (ret == 1)
                    {
                        sRet = "Erro ao salvar script";
                    }
                    else
                    {
                        sRet = "unknown error";
                    }

                    // myInterpreter.commandList(myScript.codeFeedback);
                }
                else if (header.indexOf("GET /openScript") >= 0)
                {
                    myScript.name = header.substring(header.indexOf("GET /openScript") + 15, header.indexOf(".txt") + 4);
                    Serial.println("Open:" + myScript.name);
                    // Serial.println(myScript.codeFeedback);
                    myScript.codeFeedback = myScript.read(myScript.name);
                    menu.page = _PAGE_SCRIPT_NEW;
                    // myScript.save();
                }
                else if (header.indexOf("GET /runScript") >= 0)
                {
                    myScript.name = header.substring(header.indexOf("GET /runScript") + 14, header.indexOf(".txt") + 4);
                    Serial.println("Open:" + myScript.name);
                    // Serial.println(myScript.codeFeedback);
                    myScript.codeFeedback = myScript.read(myScript.name);
                    menu.page = _PAGE_SCRIPT_RUN_1;
                    // myScript.save();
                }
                else if (header.indexOf("GET /runscript1") >= 0)
                {
                    // myScript.name = header.substring(header.indexOf("GET /runScript") + 14, header.indexOf(".txt") + 4);
                    // Serial.println("Open:" + myScript.name);
                    // // Serial.println(myScript.codeFeedback);
                    // myScript.codeFeedback = myScript.read(myScript.name);
                    myInterpreter.commandList(myScript.codeFeedback);
                    menu.page = _PAGE_SCRIPT_RUN_1;
                    // myScript.save();
                }

                else if (header.indexOf("GET /comand?fname=") >= 0)
                {
                    bRet = true;
                    String line = header.substring(header.indexOf("GET /comand?fname=") + 18, header.indexOf("HTTP/"));
                    line = handgle_decode(line);
                    Serial.println("line Receivend: " + line);
                    int ret = myInterpreter.commandLine(line, false);
                    if (ret == 0)
                    {
                        sRet = "Success";
                    }
                    else if (ret == 1)
                    {
                        sRet = "Invalid Command";
                    }
                    else if (ret == 2)
                    {
                        sRet = "Invalid Argument";
                    }
                    else
                    {
                        sRet = "unknown error";
                    }
                }
                if (header.indexOf("GET /ssidwifi") >= 0)
                {
                    ssidAux = header.substring(header.indexOf("GET /ssidwifi") + 13, header.indexOf(" HTTP/"));
                }
                else if (header.indexOf("GET /confWifi") >= 0)
                {
                    bRedeResult = true;
                    String ssid = header.substring(header.indexOf("?fssid=") + 7, header.indexOf("&fpass="));
                    String pass = header.substring(header.indexOf("&fpass=") + 7, header.indexOf(" HTTP/"));
                    Serial.println("SSID Receivend: " + ssid + " length:" + String(ssid.length()));
                    Serial.println("PASS Receivend: " + pass + " length:" + String(pass.length()));

                    char ssid_rede[ssid.length() + 1];
                    char pass_rede[pass.length() + 1];

                    ssid.toCharArray(ssid_rede, ssid.length() + 1);
                    pass.toCharArray(pass_rede, pass.length() + 1);
                    Serial.println("SSID 2 Receivend: " + String(ssid_rede));
                    Serial.println("PASS 2 Receivend: " + String(pass_rede));

                    connect_saveParamWiFi(ssid_rede, pass_rede);
                    connect_wifi();
                    // WiFi.begin(ssid_rede, pass_rede);
                    // Serial.println("Connecting to WiFi..");
                    // uint8_t attempts = 0;
                    // while (attempts < 100)
                    // {
                    //     delay(500);

                    //     if (WiFi.status() != WL_CONNECTED)
                    //     {
                    //         attempts += 1;
                    //         WiFi.begin(ssid_rede, pass_rede);
                    //     }
                    //     else
                    //     {
                    //         break;
                    //     }
                    // }

                    if (WiFi.status() == WL_CONNECTED)
                    {
                        Serial.println("Connecting to WiFi..sucess!");
                    }
                    else
                    {
                        Serial.println("Connecting to WiFi..error!");
                    }

                    auxRede = WiFi_State();
                    // char pass_rede [] = pass;
                }

                /* page select */
                if (header.indexOf("GET /home") >= 0)
                {
                    menu.page = _PAGE_HOME;
                    menu.botton1 = "\"btn2\"";        // home
                    menu.botton2 = "\"btn success\""; // script
                    menu.botton5 = menu.botton2;      // help
                    menu.botton6 = menu.botton2;      // agendamento
                    menu.botton7 = menu.botton2;      // control
                    menu.botton8 = menu.botton2;      // wifi
                }
                else if (header.indexOf("GET /script") >= 0)
                {

                    if (header.indexOf("GET /script/new") >= 0)
                    {
                        menu.page = _PAGE_SCRIPT_NEW;
                    }
                    else if (header.indexOf("GET /script/open") >= 0)
                    {
                        menu.page = _PAGE_SCRIPT_OPEN;
                    }
                    else if (header.indexOf("GET /script/run") >= 0)
                    {
                        menu.page = _PAGE_SCRIPT_RUN;
                    }
                    else
                    {
                        menu.page = _PAGE_SCRIPT;
                    }

                    menu.botton1 = "\"btn success\""; // home
                    menu.botton2 = "\"btn2\"";        // script
                    menu.botton5 = menu.botton1;      // help
                    menu.botton6 = menu.botton1;      // agendamento
                    menu.botton7 = menu.botton1;      // control
                    menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /script/open") >= 0)
                {
                    menu.page = _PAGE_SCRIPT_OPEN;
                    menu.botton1 = "\"btn success\""; // home
                    menu.botton2 = "\"btn2\"";        // script
                    menu.botton5 = menu.botton1;      // help
                    menu.botton6 = menu.botton1;      // agendamento
                    menu.botton7 = menu.botton1;      // control
                    menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /agendamento") >= 0)
                {
                    menu.page = _PAGE_AGEND_SCRIPT;
                    menu.botton1 = "\"btn success\""; // home
                    menu.botton2 = menu.botton1;      // script
                    menu.botton5 = menu.botton1;      // help
                    menu.botton6 = "\"btn2\"";        // agendamento
                    menu.botton7 = menu.botton1;      // control
                    menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /control") >= 0)
                {
                    menu.page = _PAGE_CONTROL;
                    menu.botton1 = "\"btn success\""; // home
                    menu.botton2 = menu.botton1;      // script
                    menu.botton5 = menu.botton1;      // help
                    menu.botton6 = menu.botton1;      // agendamento
                    menu.botton7 = "\"btn2\"";        // control
                    menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /help") >= 0)
                {
                    menu.page = _PAGE_HELP_COMAND;
                    menu.botton1 = "\"btn success\""; // home
                    menu.botton2 = menu.botton1;      // script
                    menu.botton5 = "\"btn2\"";        // help
                    menu.botton6 = menu.botton1;      // agendamento
                    menu.botton7 = menu.botton1;      // control
                    menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /wifi") >= 0)
                {
                    menu.page = _PAGE_WIFI;
                    menu.botton1 = "\"btn success\""; // home
                    menu.botton2 = menu.botton1;      // script
                    menu.botton5 = menu.botton1;      // help
                    menu.botton6 = menu.botton1;      // agendamento
                    menu.botton7 = menu.botton1;      // control
                    menu.botton8 = "\"btn2\"";        // wifi
                }

                // Web Page Heading
                handle_htmlStart();

                if (menu.page == _PAGE_HOME)
                {
                    handle_homePage();
                }

                else if (menu.page == _PAGE_SCRIPT)
                {
                    handle_scriptPage();
                }
                else if (menu.page == _PAGE_SCRIPT_NEW)
                {
                    handle_scriptNew();
                }
                else if (menu.page == _PAGE_SCRIPT_OPEN)
                {
                    handle_scriptOpen();
                }
                else if (menu.page == _PAGE_SCRIPT_RUN)
                {
                    handle_scriptRun();
                }
                else if (menu.page == _PAGE_SCRIPT_RUN_1)
                {
                    handle_scriptRun1();
                }

                else if (menu.page == _PAGE_MANG_SCRIPT)
                {
                    handle_scriptManPage();
                }

                else if (menu.page == _PAGE_AGEND_SCRIPT)
                {
                    handle_agendScriptPage();
                }

                else if (menu.page == _PAGE_HELP_COMAND)
                {
                    handle_helpCommandPage();
                }

                else if (menu.page == _PAGE_CONTROL)
                {
                    handle_controlPage();
                }
                else if (menu.page == _PAGE_WIFI)
                {
                    handle_wifi();
                }
                else
                {
                    handle_notFoundPage();
                }

                // }

                // Break out of the while loop
                break;

                /**/
                //     }
                //     else
                //     { // if you got a newline, then clear currentLine
                //         currentLine = "";
                //     }
                // }
                /**/

                // else if (c != '\r')
                // {                     // if you got anything else but a carriage return character,
                //     currentLine += c; // add it to the end of the currentLine
                // }
            }
        } // end while

        // Clear the header variable
        header = "";
        // Close the connection
        client.flush();
        client.stop();
        Serial.println("[interface] Client disconnected");
    } // end if
}

void handle_begin()
{
    menu.page = _PAGE_HOME;
    menu.botton1 = "\"btn2\"";        // home
    menu.botton2 = "\"btn success\""; // script
    menu.botton3 = "";
    menu.botton4 = "\"btn3\"";
    menu.botton5 = menu.botton2;
    menu.botton6 = menu.botton2;
    menu.botton7 = menu.botton2;
    menu.botton8 = menu.botton2;
}

void handle_wifi()
{
    Serial.println("handle_wifi");
    client.println("<h1>Robo-Cam WiFi</h1>");
    client.println("<form action=\"/confWifi\">");
    client.println("<p> Escaneando... </p>");
    int countWifi = WiFi.scanNetworks();
    if (countWifi > 0)
    {
        if (countWifi)
        {
            client.println("<p> Redes WiFi encontradas: </p>");
        }
        else
        {
            client.println("<p> Rede WiFi encontrada: </p>");
        }

        for (size_t i = 0; i < countWifi; i++)
        {
            client.println("<p> " + String(i + 1) + ":");
            // ssidAux = WiFi.SSID(i);
            client.println("<a href=\"/ssidwifi" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</a>");
            client.print(" (");
            client.print(WiFi.RSSI(i));
            client.print(")</p>");
        }
    }
    else
    {
        client.println("<p> Nenhuma rede WiFi encontrada </p>");
    }

    /* box ssid*/
    client.println("<label for=\"fssid\">SSID:</label>");
    client.println("<input type=\"text\" id=\"fssid\" name=\"fssid\" value=" + ssidAux + "><br>");
    /* box pass*/
    client.println("<label for=\"fpass\">PASS:</label>");
    client.println("<input type=\"text\" id=\"fpass\" name=\"fpass\"><br>");

    client.println("<input type=\"submit\" button class=\"btn success\" value=\"Submit\">");

    if (bRedeResult)
    {
        client.println("<p> Resultado da tentativa de conexao:" + auxRede + " </p>");
        bRedeResult = false;
        auxRede = "";
    }

    client.println("</form>");
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}

void handle_htmlStart()
{
    // Display the HTML web page
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");

    // CSS to style the on/off buttons
    // Feel free to change the background-color and font-size attributes to fit your preferences
    client.println("<style>");
    client.println("html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;border-radius: 24px;");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(".button2 {background-color: #555555;}");
    client.println("input[type=text] {width: 50%; padding: 12px 40px; margin: 2px 0; box-sizing: border-box; border: 2px solid #ccc; border-radius: 4px;}");
    client.println("input[type=submit] {  background-color: #04AA6D;border: none;color: white;padding: 14px 40px;text-decoration: none;margin: 4px 2px;cursor: pointer;border-radius: 4px;}");
    client.println("div.fixed {position: fixed; top: 1; left:1;} ");
    client.println(".btn {border: 2px solid black;border-radius: 5px;background-color: white;color: black;padding: 14px 40px;font-size: 16px;cursor: pointer;} .success {border-color: #04AA6D;color: green;} .success:hover {background-color: #04AA6D; color: white;}");
    client.println(".btn2 {border: 2px solid black; border-radius: 5px; color: black;padding: 14px 40px;font-size: 16px;cursor: pointer;border-color: #04AA6D;background-color: #04AA6D;color: white;}");
    client.println(".btn3 {border: 2px solid black; border-radius: 5px; color: black;padding: 14px 40px;font-size: 16px;cursor: default;border-color: #04AA6D;background-color: #04AA6D;color: white; opacity: 0.6;}");
    client.println("</style>");
    client.println("</head>");

    client.println("<body>");
    client.println("<div class=\"fixed\" ><a href=\"/home\"><button class=" + menu.botton1 + "> &nbsp; &nbsp; home &nbsp; &nbsp;</button></a>");
    client.println("<p><a href=\"/script\"><button class=" + menu.botton2 + "> &nbsp; &nbsp; script &nbsp; &nbsp; </button></a></p>");
    client.println("<p><a href=\"/agendamento\"><button class=" + menu.botton6 + ">scheduling</button></a></p>");
    client.println("<p><a href=\"/control\"><button class=" + menu.botton7 + "> &nbsp; controle &nbsp; </button></a></p>");
    client.println("<p><a href=\"/wifi\"><button class=" + menu.botton8 + "> &nbsp; &nbsp; &nbsp; WiFi  &nbsp; &nbsp; </button></a></p>");
    client.println("<p><a href=\"/help\"><button class=" + menu.botton5 + "> &nbsp; &nbsp; &nbsp; help  &nbsp; &nbsp; </button></a></p></div>");
}

void handle_controlPage()
{
    Serial.println("handle_controlPage");

    // client.println("<body>");
    // client.println("<p><a href=\"/control\"></a></p>");
    // client.println("<meta http-equiv=\"refresh\" content=\"10;URL='http://robo/control'\">");
    client.println("<h1>Robo-Cam Moviments</h1>");

    client.println("<h2> ROBO </h2>");
    // client.println("<p><a href=\"/outMovLeft/on\"><button class=\"btn success\">LEFT</button></a>");
    // client.println(" <a href=\"/outMovRight/on\"><button class=\"btn success\">RIGHT</button></a></p>");
    String dir;
    // Serial.println("Dir: "+String(stepper->directionPinHighCountsUp()));
    if (stepper->directionPinHighCountsUp())
    {
        dir = "Left";
    }
    else
    {
        dir = "Right";
    }

    client.println("<p> Direcao:" + dir + "</p>");
    client.println("<p> Aceleracao:" + String(stepper->getAcceleration()) + " step/s^2</p>");
    client.println("<p> Velocidade:" + String(stepper->getSpeedInUs()) + " us/step</p>");
    client.println("<p> Posicao Atual:" + String(stepper->getCurrentPosition()) + "</p>");
    client.println("<p> Pulse Counter Attached:" + String(stepper->getPositionAfterCommandsCompleted()) + "</p>");
    client.println("<p> Configuracao de passo:" + myStepper.getStepConfig() + "</p>");
    client.println("<p> Fim de curso 01: " + String(digitalRead(INPIN_IR_COURSE_01)) + "</p");
    client.println("<p> Fim de curso 02: " + String(digitalRead(INPIN_IR_COURSE_02)) + "</p");

    // client.println("<p> Transicoes de pulsos por volta: " + String(myStepper.getPulsesTransitionAround()) + "</p");
    // client.println("<p> Pulsos Ativos por volta: " + String(myStepper.getPulsesActiveAround()) + "</p");
    // client.println("<p> Pulsos Inativos por volta: " + String(myStepper.getPulsesInactiveAround()) + "</p");
    // client.println("<p> Transicoes de pulsos total: " + String(myStepper.getPulsesTransitionTotaly()) + "</p");
    // client.println("<p> Pulsos Ativos total: " + String(myStepper.getPulsesActiveTotaly()) + "</p");
    // client.println("<p> Pulsos Inativos total: " + String(myStepper.getPulsesInactiveTotaly()) + "</p");

    client.println("<p> Passos total do Workspace: " + String(myStepper.getStepTotalyWorkspace()) + "</p");
    client.println("<p> Posicao do passo atual: " + String(myStepper.getStepCurrentPosition()) + "</p");

    // client.println("<p> Passos por pulso ativo por volta: " + String(myStepper.getStepPerPulseActive()) + "</p");
    // client.println("<p> Passos por pulso inativo por volta: " + String(myStepper.getStepPerPulseInactive()) + "</p");

    // client.println("<p> Posicao futura:" + String(stepper->getPositionAfterCommandsCompleted()) + "</p>");

    client.println("<h2> CAMERA </h2>");
    // client.println("<p><a href=\"/outMovTilt/up\"><button class=\"btn success\"> UP </button></a></p>");
    // client.println("<p><a href=\"/outMovPan/left\"><button class=\"btn success\">LEFT</button></a>");
    // client.println(" <a href=\"/outMovPan/right\"><button class=\"btn success\">RIGHT</button></a></p>");
    // client.println("<p><a href=\"/outMovTilt/down\"><button class=\"btn success\">DOWN</button></a></p>");
    client.println("<p> Velocidade:" + String(TIME_PER_ANGLE) + " ms/angle </p>");
    // client.println("<p> Tilt Angle:" + String(MyMoves.currentAngleTilt) + "</p>");
    // client.println("<p> Pan Angle:" + String(MyMoves.currentAnglePan) + "</p>");
    client.println("<p> Tilt Angle:" + String(MyMoves.valueCanTilt) + "</p>");
    client.println("<p> Pan Angle:" + String(MyMoves.valueCanPan) + "</p>");

    client.println("<form action=\"/comand\">");
    client.println("<label for=\"fname\">Command Line:</label>");
    client.println("<input type=\"text\" id=\"fname\" name=\"fname\">");
    client.println("<input type=\"submit\" value=\"Submit\">");
    client.println("</form>");
    // client.println("<p> ENVIAR SCRIPT </p>");
    // client.println("<div class=\"fixed\" ><a href=\"/script\"><button class=\"btn success\"> script </button></a></div>");
    if (bRet)
    {
        client.println("<p> " + sRet + "</p>");
        bRet = false;
        sRet = "";
    }

    client.println("</body>");

    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}
void handle_scriptPage()
{
    Serial.println("handle_scriptPage");
    // client.println("<body>");
    client.println("<h1> Robo-Cam Script </h1>");
    // client.println("<p> MOVIMENTAR ROBO </p>");
    // client.println("<div class=\"fixed\" ><a href=\"/home\"><button class=\"btn success\"> home </button></a></div>");

    client.println("<p><a href=\"/script/new\"><button class=\"btn success\">Novo Scrip</button></a></p>");
    client.println("<p><a href=\"/script/open\"><button class=\"btn success\">Abrir Script</button></a></p>");
    client.println("<p><a href=\"/script/run\"><button class=\"btn success\">Executar Script</button></a></p>");

    // client.println(" <form action=\"/send\" method=\"get\">");
    // client.println("<label for=\"sname\">Nome do Script:</label>");
    // client.println("<input type=\"text\" id=\"sname\" name=\"sname\" value=" + myScript.name + ">");
    // client.println("<p> <label for=\"fscript\">Script code:</label> </p>");
    // // client.println(" <input type=\"text\" id=\"fscript\" name=\"fscript\"><br><br>");
    // // client.println(" <label for=\"lname\">Last name:</label>");
    // // client.println(" <input type=\"text\" id=\"lname\" name=\"lname\"><br><br>");
    // client.println("<textarea id=\"fscript\" name=\"fscript\" rows=\"20\" cols=\"80\">" + myScript.codeFeedback + "</textarea>");

    // client.println("<p><input type=\"submit\" value=\"COMPILAR\"></p>");
    // client.println(" </form>");

    // client.println("<p><a " + menu.botton3 + "><button class=" + menu.botton4 + "> SALVAR </button></a></p>");
    // // client.println("<p href=\"/saves2\"><input type=\"button\" value=\"COMPILAR\"></p>");

    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}

void handle_scriptOpen()
{
    Serial.println("handle_scriptNew");
    client.println("<h1> Robo-Cam Abrir Script </h1>");
    client.println("</body>");
    client.println("</html>");
    String names = myScript.list();
    String aux;
    client.println("<h2> Scripts </h2>");
    while (names.length() > 9)
    {
        aux = names.substring(0, names.indexOf("\r\n"));
        names = names.substring(names.indexOf("\r\n") + 1, names.length());
        // client.println("<p> " + aux + "</p>");
        client.println("<p><a href=\"/openScript" + aux + "\">" + aux + "</a></p>");
    }

    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}

void handle_scriptRun()
{
    Serial.println("handle_scriptRun");
    client.println("<h1> Robo-Cam Abrir Script </h1>");
    client.println("</body>");
    client.println("</html>");
    String names = myScript.list();
    String aux;
    client.println("<h2> Scripts </h2>");
    while (names.length() > 9)
    {
        aux = names.substring(0, names.indexOf("\r\n"));
        names = names.substring(names.indexOf("\r\n") + 1, names.length());
        // client.println("<p> " + aux + "</p>");
        client.println("<p><a href=\"/runScript" + aux + "\">" + aux + "</a></p>");
    }
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}
void handle_scriptRun1()
{
    Serial.println("handle_scriptRun");
    client.println("<h1> Robo-Cam Abrir Script </h1>");
    client.println("</body>");
    client.println("</html>");

    client.println("<h2> Run Script </h2>");
    // client.println("<input type=\"text\" name=\"" + myScript.name + "\" value=\"");
    // String command = myScript.codeFeedback;
    // String aux;
    // while (command.length() > 4)
    // {
    //     aux = command.substring(0, command.indexOf("\r\n"));
    //     command = command.substring(command.indexOf("\r\n") + 1, command.length());
    //     client.println(aux);
    //     client.println("<br>");
    // }

    // client.println("\" readonly>");
    client.println(" <form action=\"/runscript1\" >");
    client.println("<label for=\"sname\">Nome do Script:</label>");
    client.println("<input type=\"text\" id=\"sname\" readonly=\"readonly\" name=\"sname\" value=" + myScript.name + ">");
    client.println("<p> <label for=\"fscript\">Script code:</label> </p>");
    // client.println(" <input type=\"text\" id=\"fscript\" name=\"fscript\"><br><br>");
    // client.println(" <label for=\"lname\">Last name:</label>");
    // client.println(" <input type=\"text\" id=\"lname\" name=\"lname\"><br><br>");
    client.println("<textarea id=\"fscript\" name=\"fscript\" readonly=\"readonly\" rows=\"20\" cols=\"80\">" + myScript.codeFeedback + "</textarea>");

    client.println("<p><input type=\"submit\" value=\"RUN\"></p>");
    client.println(" </form>");
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}
void handle_scriptNew()
{
    Serial.println("handle_scriptNew");
    // client.println("<body>");
    client.println("<h1> Robo-Cam Novo Script </h1>");
    // client.println("<p> MOVIMENTAR ROBO </p>");
    // client.println("<div class=\"fixed\" ><a href=\"/home\"><button class=\"btn success\"> home </button></a></div>");

    client.println(" <form action=\"/send\" method=\"get\">");
    client.println("<label for=\"sname\">Nome do Script:</label>");
    client.println("<input type=\"text\" id=\"sname\" name=\"sname\" value=" + myScript.name + ">");
    client.println("<p> <label for=\"fscript\">Script code:</label> </p>");
    // client.println(" <input type=\"text\" id=\"fscript\" name=\"fscript\"><br><br>");
    // client.println(" <label for=\"lname\">Last name:</label>");
    // client.println(" <input type=\"text\" id=\"lname\" name=\"lname\"><br><br>");
    client.println("<textarea id=\"fscript\" name=\"fscript\" rows=\"13\" cols=\"80\">" + myScript.codeFeedback + "</textarea>");

    client.println("<p><input type=\"submit\" value=\"COMPILAR\"></p>");
    client.println(" </form>");

    client.println("<p><a " + menu.botton3 + "><button class=" + menu.botton4 + "> SALVAR </button></a></p>");
    // client.println("<p href=\"/saves2\"><input type=\"button\" value=\"COMPILAR\"></p>");
    if (bRet)
    {
        client.println("<p> " + sRet + "</p>");
        bRet = false;
        sRet = "";
    }
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}

void handle_scriptManPage()
{
    Serial.println("handle_scriptManPage");

    client.println("<h1> Robo-Cam Script Manager </h1>");
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}

void handle_agendScriptPage()
{
    Serial.println("handle_agendScriptPage");

    client.println("<h1> Robo-Cam Agendamento de Script </h1>");
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}

void handle_helpCommandPage()
{
    Serial.println("handle_helpCommandPage");

    client.println("<h1> Robo-Cam Commands </h1>");
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}
void handle_notFoundPage()
{
    Serial.println("handle_notFoundPage");

    client.println("<h1> error Page </h1>");
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}

void handle_homePage()
{
    Serial.println("handle_homePage");
    String stateWiFi = WiFi.SSID();
    WiFi.status();
    client.println("<meta http-equiv=\"refresh\" content=\"10\">");
    client.println("<h1> Robo-Cam Control </h1>");
    client.println("<p> Data: " + ntp.getFormattedDate() + " </p>");
    client.println("<p> Hora: " + ntp.getFormattedTime() + " </p>");
    client.println("<h2> Status WiFi </h2>");
    client.println("<p> Station Mode</p>");
    client.println("<p> SSID: " + stateWiFi + "  (");
    client.print(WiFi.RSSI());
    client.print(")</p><p> IP: ");
    client.print(WiFi.localIP());
    client.println("</p>");
    if (WiFi.status() == WL_CONNECTED)
    {
        client.println("<p><a href=\"http://" + String(HOST_NAME) + ADDR_HOME + "\">" + "http://" + String(HOST_NAME) + ADDR_HOME + "</a></p>");
    }
    client.println("<p> Status: " + WiFi_State() + " </p>");

    client.println("<p> AP Mode</p>");
    client.println("<p> IP:");
    client.print(WiFi.softAPIP());
    client.print(" </p>");

    client.println("<h2> Status Bateria </h2>");
    String ligacao;
    if (bateria.StateSerieOn)
    {
        ligacao = "Serie";
    }
    else
    {
        ligacao = "Paralelo";
    }

    if (bateria.StateCharging)
    {
        client.println("<p> Bateria: Carregando...</p>");
    }
    client.println("<p> Ligacao dos Bancos:" + ligacao + "</p>");
    client.println("<p> Vin: " + String(bateria.ValueVin) + " V  </p>");
    client.println("<p> 5V: " + String(bateria.ValueV5) + " V  </p>");
    client.println("<p> Bank1: " + String(bateria.ValueVs1) + " V  " + String(bateria.ValuePercentVs1, 0) + "%</p>");
    client.println("<p> Bank2: " + String(bateria.ValueVs2) + " V  " + String(bateria.ValuePercentVs2, 0) + "%</p>");
    client.println("<p> Bank3: " + String(bateria.ValueVs3) + " V  " + String(bateria.ValuePercentVs3, 0) + "%</p>");
    // client.println("<p> Serie: " + String(bateria.ValueVtotal) + " V  " + String(bateria.ValuePercentTotal) + "%</p>");

    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}

String handgle_decode(String msg)
{
    msg.replace("%0D", "\r");
    msg.replace("%0A", "\n");
    // msg.replace("%", "");
    msg.replace("+", " ");
    msg.replace("%2F", "/");
    msg.replace("%25", "%");
    msg.replace("%2B", "+");
    msg.replace("%2C", ",");
    msg.replace("%3A", ":");
    msg.replace("%3B", ";");
    msg.replace("%3C", "<");
    msg.replace("%3E", ">");

    return msg;
}
#endif