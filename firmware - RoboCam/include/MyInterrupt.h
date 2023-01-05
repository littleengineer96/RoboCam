#ifndef MYINTERRUPT_H_
#define MYINTERRUPT_H_

#include "prototypes.h"

void INTERRUPT_setup(uint64_t time_in_us);
void INTERRUPT_routine(void);
void INTERRUPT_blynk_led(void);

void INTERRUPT_setup(uint64_t time_in_us)
{
    /*
    https://www.dobitaobyte.com.br/isr-interrupcoes-e-timer-com-esp32/
    */

    // inicialização do timer. Parametros:
    /* 0 - seleção do timer a ser usado, de 0 a 3.
      80 - prescaler. O clock principal do ESP32 é 80MHz. Dividimos por 80 para ter 1us por tick.
    true - true para contador progressivo, false para regressivo
    */
    timer = timerBegin(0, 80, true);

    /*conecta à interrupção do timer
    - timer é a instância do hw_timer
    - &My_Timer endereço da função a ser chamada pelo timer
    - true gera uma interrupção
   */
    timerAttachInterrupt(timer, &INTERRUPT_routine, true);

    /*
     - o timer instanciado no inicio
     - o valor em us para
     - auto-reload. true para repetir o alarme
    */
    timerAlarmWrite(timer, time_in_us, true);

    // ativa o alarme
    timerAlarmEnable(timer);
}

void INTERRUPT_routine(void)
{
    current_time += 1;


    /*apos conectar a rede*/
    if ((current_time % LED_NET == 0) && (timeCheckWifi))
    {
        INTERRUPT_blynk_led();
        // digitalWrite(LED_BORD, state_led);
    }
    /*enquando não se conecta a uma rede*/
    else if ((timeCheckWifi == 0) && (current_time % LED_NOT_NET == 0))
    {
        INTERRUPT_blynk_led();
        // digitalWrite(LED_BORD, state_led);
    }

    if (timeCheckWifi)
    {
        timeCheckWifi -= 1;
    }
    
    if (bateria.ValueTimeCheck)
    {
        bateria.ValueTimeCheck -= 1;
    }

    if (timeUpdateRTC)
    {
        timeUpdateRTC -=1;
    }
    
}
void INTERRUPT_blynk_led(void)
{
    state_led = !state_led;
    digitalWrite(OUTPIN_LED_NET, state_led);
    digitalWrite(OUTPIN_LED_ERRO,state_led);
}

#endif