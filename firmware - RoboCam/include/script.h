#ifndef SCRIPT_H_
#define SCRIPT_H_

#ifdef ESP32
#include <SPIFFS.h>
#else
#include <FS.h> //this needs to be first, or it all crashes and burns...
#endif
#include "script.h"
#include "interpreter.h"
#include "prototypes.h"
// #include "interface.h"
extern struct menuhtml menu;
extern interpreter myInterpreter;

class Script
{
private:
/* data */
#define _SCRIPT_NAME_INT "S"
#define _SERV_PAN_ANG "P"
#define _SERV_TILT_ANG "T"
#define _VELOCIDADE_MpS "V"
#define _ESTACIONAR_SEG "E"
#define _MOVER_STEP "M"
#define _DIR_STEP "D"
#define _FIM_LINE "\r\n"
public:
    String name;
    String code;
    String codeFeedback;
    String listName;
    bool stateNewScript;
    bool stateFBcode;
    bool stateErrorScript;
    void check();
    uint8_t checkCommand(String command);
    uint8_t checkArg(String arg);
    String read(String name);

    String list();
    void clearList();

    uint8_t save();
    Script(/* args */);
    ~Script();
};

Script::Script(/* args */)
{
}

Script::~Script()
{
}

uint8_t Script::checkCommand(String command)
{

    uint8_t ret = 0;
    if (command.startsWith(_SCRIPT_NAME_INT))
    {
    }
    else if (command.startsWith(_SERV_PAN_ANG))
    {
    }
    else if (command.startsWith(_SERV_TILT_ANG))
    {
    }
    else if (command.startsWith(_VELOCIDADE_MpS))
    {
    }
    else if (command.startsWith(_ESTACIONAR_SEG))
    {
    }
    else
    {
        /* caso não exita o comando */
        ret = 1;
    }

    return ret;
}

uint8_t Script::checkArg(String arg)
{
    uint8_t ret = 0;
    int argInt = arg.toInt();

    if (argInt == 0)
    {
        ret = 1;
    }

    return ret;
}
void Script::check()
{
    uint8_t ret = 0;
    if (stateNewScript)
    {
        String error1 = " <-- error: the script must be started with the S command";
        String error2 = " <-- ERROR: invalid command";
        String error3 = " <-- ERROR: invalid argument";

        int length = code.length();
        // int start = code.indexOf(_SCRIPT_NAME_INT);
        int end = code.indexOf(_FIM_LINE);

        String line;
        String command;
        String arg;
        int linesCont = 0;
        int errorCont = 0;
        String lineError;

        codeFeedback = "";
        // if ((start >= 0) && (end >= 0) && (start < end))
        // {
        Serial.println("==== Start Script check ==== ");

        for (size_t i = 0; i < length; i++)
        {
            end = code.indexOf(_FIM_LINE);          // armazendo a posição do fim de linha do comando atual
            line = code.substring(0, end);          // pega linha atual comando atual
            command = line.substring(0, 0);         // pega o comando da linha atual
            arg = code.substring(1, end);           // pego o argumento da linha atual
            code = code.substring(end + 2, length); // remove a linha atual

            /* removendo strings de erro*/
            if (stateErrorScript)
            {
                // Serial.println("error: ");
                // Serial.println(line);
                String auxError1 = error1;
                String auxError2 = error2;
                String auxError3 = error3;

                auxError1.toUpperCase();
                auxError2.toUpperCase();
                auxError3.toUpperCase();

                if (line.indexOf(auxError1) > 0)
                {
                    // Serial.println("error 1: ");
                    line = line.substring(0, line.indexOf(auxError1));
                    // Serial.println(line);
                }
                else if (line.indexOf(auxError2) > 0)
                {
                    // Serial.println("error 2: ");
                    line = line.substring(0, line.indexOf(auxError2));
                    // Serial.println(line);
                }
                else if (line.indexOf(auxError3) > 0)
                {
                    // Serial.println("error 3: ");
                    line = line.substring(0, line.indexOf(auxError3));
                    // Serial.println(line);
                }
            }

            ret = myInterpreter.commandLine(line, true);

            // ret = checkCommand(line);
            // if (ret == 0)
            // {
            //     ret = checkArg(arg);
            //     if (ret)
            //     {
            //         // Serial.println("line: " + line + " Argumento invalido: " + arg);
            //         errorCont += 1;
            //         lineError = error3;
            //     }
            // }
            // else
            // {
            //     // Serial.println("Comando invalido: " + line);
            //     errorCont += 1;
            //     if (i == 0)
            //     {
            //         lineError = error1;
            //     }
            //     else
            //     {
            //         lineError = error2;
            //     }
            // }

            if (ret == 1)
            {
                lineError = error2;
                errorCont += 1;
            }
            else if (ret == 2)
            {
                lineError = error3;
                errorCont += 1;
            }

            // /* caso não inicie com S*/
            // if ((i == 0) && line.indexOf(_SCRIPT_NAME_INT))
            // {
            //     lineError = error1;
            //     ret = 1;
            //     errorCont += 1;
            // }

            if (ret == 0)
            {
                // Serial.println(String(i) + ": " + line + " Command:" + command + " Arg:" + arg);
                codeFeedback += line + "\r\n";
            }
            else
            {
                codeFeedback += line + lineError + "\r\n";
                lineError = "";
            }

            /* scape */
            if (code.length() < 4)
            {
                // Serial.println("last command");
                Serial.println("==== End Script check ==== ");
                break;
            }
        }

        Serial.println("code Feedback:");
        Serial.println(codeFeedback);
        stateFBcode = true;
        if (errorCont)
        {
            stateErrorScript = true;
            menu.botton3 = "";
            menu.botton4 = "\"btn3\"";
        }
        else
        {
            stateErrorScript = false;
            menu.botton3 = "href=\"/savescript\"";
            menu.botton4 = "\"btn success\"";
        }
        // }

        stateNewScript = false;
    }
}

/**
 * @brief
 * @return ret
 * @retval 0 = success
 * @retval 1 = error saving
 */
uint8_t Script::save()
{
    uint8_t ret;
    Serial.println("\n[Script::save]\n");

    // codeFeedback.replace(" ", "");
    // String name = "/";
    // name += codeFeedback.substring(0, codeFeedback.indexOf(_FIM_LINE));
    // name += ".txt";
    Serial.println("Nome do Script: " + name);
    // codeFeedback = codeFeedback.substring(codeFeedback.indexOf(_FIM_LINE)-2,codeFeedback.length());
    Serial.println("Script: \n" + codeFeedback);

    /*verificação 1*/
    if (name.indexOf("/") == -1)
    {
        name = "/" + name;
    }

    /*verificação 2*/
    if (name.indexOf(".txt") == -1)
    {
        name += ".txt";
    }

    int scriptsListCont = 0;
    String aux;
    if (SPIFFS.begin(true))
    {
        //    scriptsListCont += 1;
        // aux = String(scriptsListCont);
        // aux += "-";
        // aux += name;

        // aux = name;

        // if (SPIFFS.exists(name))
        // {
        //     Serial.println("JA EXISTE SCRIPT COM NOME: " + name);
        // }
        // else
        // {

        /* sobrescrevo o arquivo caso exista ou crio um novo arquivo*/
        File configFile = SPIFFS.open(name, "w");
        configFile.print(codeFeedback);
        configFile.close();
        Serial.println("SCRIPT " + name + " SALVO COM SUCESSO!");

        if (SPIFFS.exists(ADDR_SCRIPTS_LIST))
        {
            /* leio a lista */
            configFile = SPIFFS.open(ADDR_SCRIPTS_LIST, "r");
            aux = configFile.readString();

            /*adiciono a lista se o nome do script não exisitir*/
            if (aux.indexOf(name) < 0)
            {
                aux += name;
                aux += "\r\n";
            }

            /* sobrescrevo a lista com o conteudo de aux */
            configFile = SPIFFS.open(ADDR_SCRIPTS_LIST, "w");
            configFile.print(aux);
            configFile.close();
        }
        else
        {
            configFile = SPIFFS.open(ADDR_SCRIPTS_LIST, "w");
            aux += name;
            aux += "\r\n";
            configFile.print(aux);
            configFile.close();
        }
        ret = 0; // success
        // }

        // read(name);
    }
    else
    {
        ret = 1; // error saving
    }

    return ret;
}

String Script::read(String name)
{
    Serial.println("\n[Script::read]\n");

    String script = "";
    if (SPIFFS.exists(name))
    {
        File configFile = SPIFFS.open(name, "r");
        if (configFile)
        {
            script = configFile.readString();
            Serial.println("Script Lido: \n" + script);
            listName = script;
        }
        else
        {
            Serial.println("error configFile ");
        }

        // configFile = SPIFFS.open(ADDR_SCRIPTS_LIST, "r");
        // if (configFile)
        // {
        //     script = configFile.readString();
        //     Serial.println("Script List: \n" + script);
        // }
        // else
        // {
        //     Serial.println("error configFile ");
        // }

        configFile.close();
    }
    else
    {
        Serial.println("Error INVALID NAME SCRIPT: " + name);
    }

    return script;
}

String Script::list()
{
    read(ADDR_SCRIPTS_LIST);
    return listName;
}

void Script::clearList()
{
    File configFile = SPIFFS.open(ADDR_SCRIPTS_LIST, "w");
    configFile.close();
}

Script myScript;
#endif