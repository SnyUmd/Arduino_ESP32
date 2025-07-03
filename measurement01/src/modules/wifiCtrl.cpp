#define JST 3600* 9
#include "wifiCtrl.hpp"

//↓↓--------固定IPアドレスにする場合はここを操作-------
bool isFixedIP = false;
const IPAddress ip(192, 168, 0, 240);
const IPAddress gateway(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress dns1(192, 168, 0, 1);
//↑↑-------------------------------------------------

// ****************************************************************
// void wifiInit(WiFiClass& wifi, HardwareSerial& sr, String ssid, String pass, String host_name, bool bl_ota)
bool wifiInit(WiFiClass& w_f, HardwareSerial& sr, String ssid, String pass, String host_name, bool bl_ota)
{
    int retryCnt = 0;
    while(1)
    {
        int waitTime = 0;
        if(isFixedIP)
        {
            if (!w_f.config(ip,gateway,subnet,dns1))
            {
                sr.println("Failed to configure!");
                return false;
            }
            else
                sr.println("Failed to configure!");
        }
        w_f.setHostname(host_name.c_str());//ホスト名を設定
        delay(1000);
        w_f.begin(ssid.c_str(), pass.c_str());
        delay(1000);
        if(w_f.status() != WL_CONNECTED) {
            while(1)
            {
                waitTime++;
                if(waitTime > 5) break;
                sr.print('.');
                delay(250);
            }
            retryCnt++;
            if(retryCnt < 5)
            {
                sr.println("retry");
                digitalWrite(PORT_LED, LED_ON);
                delay(25);
                digitalWrite(PORT_LED, LED_OFF);
                continue;
            }
            else
            {
                sr.println("connect error");
                return false;
            }
        }
        else break;
    }


    configTime(JST, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");//NTPの設定
    sr.println("");
    sr.println("Wi-Fi connected");
    sr.print("IP address : ");
    sr.println(w_f.localIP());
    sr.print("SSID : ");
    sr.println(ssid);
    sr.print("Host name : ");
    sr.println(w_f.getHostname());
    return true;
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
