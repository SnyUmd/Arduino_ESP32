#define JST 3600* 9
#include "wifiCtrl.h"

// ****************************************************************
void wifiInit(WiFiClass& wifi, HardwareSerial& sr, String ssid, String pass, String host_name, bool bl_ota)
{

    while(1)
    {
        int waitTime = 0;
        wifi.setHostname(host_name.c_str());//ホスト名を設定
        delay(1000);
        wifi.begin(ssid.c_str(), pass.c_str());
        delay(1000);
        while(wifi.status() != WL_CONNECTED) {
            sr.print('.');
            delay(500);
            waitTime++;
            if(waitTime > 5) break;
        }
        if(waitTime <= 5) break;
        else sr.println("retry");
    }


    configTime(JST, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");//NTPの設定
    sr.println("");
    sr.println("Wi-Fi connected");
    sr.println(wifi.localIP());
    sr.print("SSID : ");
    sr.println(ssid);
    sr.print("Host name : ");
    sr.println(host_name);
}

// ****************************************************************
//使い方　//struct tm t = getTimeInf();
struct tm getTimeInf()
{
    struct tm resultTimeInf;
    getLocalTime(&resultTimeInf);
    return resultTimeInf;
}

// ****************************************************************
//sに使う変数：char s[20];
void arrangeTime(char* s, tm time_inf)
{
    sprintf(s, "%04d.%02d.%02d %02d:%02d:%02d", 
                time_inf.tm_year + 1900,
                time_inf.tm_mon + 1,
                time_inf.tm_mday,
                time_inf.tm_hour,
                time_inf.tm_min,
                time_inf.tm_sec);
}
