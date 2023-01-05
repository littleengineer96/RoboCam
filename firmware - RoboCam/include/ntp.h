#ifndef NTP_H_
#define NTP_H_

#include "prototypes.h"
#include "globalvar.h"

#define TIME_CHECK_RTCNET 60 // time in minutes

class RTCNet
{
private:
    unsigned long timeUpdate;
public:
    
    void begin();
    void check();
    
    void setTimeUpdate(unsigned long time) { timeUpdate = time; };
    unsigned long getTimeUpdate() { return timeUpdate; };

    RTCNet(/* args */);
    ~RTCNet();
};

RTCNet::RTCNet(/* args */)
{
}

RTCNet::~RTCNet()
{
}

void RTCNet::begin()
{
    ntp.begin();
    // GMT em segundos
    // +1 = 3600
    // +8 = 28800
    // -1 = -3600
    // -3 = -10800 (BRASIL)
    ntp.setTimeOffset(-10800);
    setTimeUpdate(0); //fazer primeira atualizacao
}
void RTCNet::check()
{
    if (timeUpdate == 0)
    {
        if (ntp.update())
        {
            Serial.println("[NTP] RTC Updated");

            timeUpdate = RELOAD_TIME_UPDATE_RTC;
        }
        else
        {
            if (ntp.forceUpdate())
            {
                timeUpdate = RELOAD_TIME_UPDATE_RTC;
            }
            else
            {
                Serial.println("[NTP] ERROR UPDATE RTC");
                timeUpdate = ((10 / TIME_INTERRUPT_MS) * 1000);
            }
        }
    }
}

void rtc_begin()
{
    ntp.begin();
    // GMT em segundos
    // +1 = 3600
    // +8 = 28800
    // -1 = -3600
    // -3 = -10800 (BRASIL)
    ntp.setTimeOffset(-10800);
}
void rtc_update()
{
    if (timeUpdateRTC == 0)
    {
        if (ntp.update())
        {
            // Serial.print("DATA/HORA: ");
            // Serial.println(ntp.getFormattedDate());

            // Serial.print("HORARIO: ");
            // Serial.println(ntp.getFormattedTime());

            // Serial.print("HORA: ");
            // Serial.println(ntp.getHours());

            // Serial.print("MINUTOS: ");
            // Serial.println(ntp.getMinutes());

            // Serial.print("SEGUNDOS: ");
            // Serial.println(ntp.getSeconds());

            // Serial.print("DIA DA SEMANA (0=domingo): ");
            // Serial.println(ntp.getDay());

            // Serial.print("EPOCH TIME (Segundos desde 01/01/1970): ");
            // Serial.println(ntp.getEpochTime());
            Serial.println("[NTP] RTC Updated");

            timeUpdateRTC = RELOAD_TIME_UPDATE_RTC;
        }
        else
        {
            if (ntp.forceUpdate())
            {
                timeUpdateRTC = RELOAD_TIME_UPDATE_RTC;
            }
            else
            {
                Serial.println("[NTP] ERROR UPDATE RTC");
                timeUpdateRTC = ((10 / TIME_INTERRUPT_MS) * 1000);
            }
        }
    }

    // if (timeUpdateRTC == 3000)
    // {
    //     Serial.print("DATA/HORA: ");
    //     // Serial.println(ntp.);

    //     Serial.print("HORARIO: ");
    //     Serial.println(ntp.getFormattedTime());

    //     Serial.print("HORA: ");
    //     Serial.println(ntp.getHours());

    //     Serial.print("MINUTOS: ");
    //     Serial.println(ntp.getMinutes());

    //     Serial.print("SEGUNDOS: ");
    //     Serial.println(ntp.getSeconds());

    //     Serial.print("DIA DA SEMANA (0=domingo): ");
    //     Serial.println(ntp.getDay());

    //     Serial.print("EPOCH TIME (Segundos desde 01/01/1970): ");
    //     Serial.println(ntp.getEpochTime());
    // }
}

#endif