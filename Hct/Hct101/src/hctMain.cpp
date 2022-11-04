#include <Arduino.h>
// #include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <iostream>
#include "module/initialize.h"
#include "module/defHct.h"
#include "module/common.h"
#include "module/Timer.h"
#include "module/wifiCtrl.h"
#include "module/ledCtrl.h"
#include "module/rx8035.h"
#include "module/BzCtrl.h"
#include "module/htu21d.h"
#include "module/i2cCtrl.h"
#include "module/motorCtrl.h"

using namespace std;

ledCtrl LC;

void init();
void setHttpAction();
void swInterrupt();
void setNowOpen(int open_time);
void setAfter(int next_time, int adjustment_time);
void setDevice(int contentNum);
void outputValue();
void outputTime();
string getTemp();
string getHumd();
void closeValve_Now();
int getNextTime();
void IRAM_ATTR setClosing();
void IRAM_ATTR afterOpenValve();

void modeSetting();
void IRAM_ATTR onTimer();


//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
void setup()
{
    initPort();
    attachInterrupt(PORT_SW, swInterrupt, FALLING);
    sr.begin(115200);
    initI2C(wr);
    InitBz();
    bzPowerOn();
    wifiInit(WiFi, sr, SSID, PASS, HOST_NAME, false);
    setHttpAction();
    sr.println("-----loop Start-----");
    BzGoUp(10, 10);
    
    // setVal.interval = 20;
    // operationReservation = enmRegularOppenning;
    // setAfter(setVal.interval, setVal.length);
    LC.ledFlash(PORT_LED_R, 10, 5);
    digitalWrite(PORT_LED_R, 1);

    motorAction(false, 100, true);
    motorAction(true, 100, true);
}

//*************************************
void loop()
{
    server.handleClient();

    switch(operationReservation)
    {
        case enmNon:
            break;
        case enmNowOppenning://1
            if (!deviceSts.opened){
                deviceSts.opened = true;
                motorAction(false, 100, false);
                deviceSts.nowOppenning = false;
                setTimerInterrupt(&setClosing, setVal.nowOpenLength * 1000000, false);
                operationReservation = enmNowClosing;
            }
            break;
        case enmNowClosing://2
            if (deviceSts.closingRun){
                motorAction(true, 100, false);
                if(setVal.interval != 0) {
                    setVal.nextTime = getNextTime();
                    sr.print("next ");
                    sr.println(setVal.nextTime);
                    if(setVal.nextTime < 3) setVal.nextTime = 3;
                    setAfter(setVal.nextTime, setVal.nowOpenLength + timeAdjuster + 1);
                    operationReservation = enmRegularOppenning;
                }
                else operationReservation = enmNon;
                deviceSts.opened = false;
                deviceSts.closingRun = false;
            }
            break;
        case enmRegularOppenning://3
            if (!deviceSts.opened && deviceSts.regularOppenning){
                deviceSts.opened = true;
                motorAction(false, 100, false);
                deviceSts.nowOppenning = false;
                setTimerInterrupt(&setClosing, 2 * 1000000, false);
                operationReservation = enmRegularClosing;
                deviceSts.regularOppenning = false;
            }
            break;
        case enmRegularClosing://4
            if (deviceSts.closingRun){
                motorAction(true, 100, false);
                deviceSts.opened = false;
                setAfter(setVal.interval, setVal.length + timeAdjuster);
                deviceSts.closingRun = false;
                operationReservation = enmRegularOppenning;
            }
            break;
    }

    if(setVal.settingReserv && !deviceSts.opened) {
        setAfter(setVal.interval, 0);
        setVal.settingReserv = false;
    }

    if(httpSts[enmBuzzer].sts) bz(1);
}

//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************

//*************************************
void setHttpAction()
{
    //バルブオープン
    server.on(httpSts[enmNow].uri, HTTP_ANY, [](){setDevice(enmNow);});
    //アフター設定
    server.on(httpSts[enmSet].uri, HTTP_ANY, [](){setDevice(enmSet);});

    //ブザー
    server.on(httpSts[enmBuzzer].uri, HTTP_ANY, [](){setDevice(enm_buzzer);});

    //値取得
    server.on(httpSts[enmGet].uri, HTTP_ANY, [](){outputValue();});

    //位置の調整
    server.on(httpSts[enmAdjust].uri, HTTP_ANY, [](){setDevice(enmAdjust);});

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        errorSound();
        server.send(404, "text/plain", errorMessage[enmNotFound]); // 404を返す
    });

    server.begin();
}

//*************************************
//ポート割り込み処理
//*************************************
void swInterrupt()
{
    //チャタリング対策------------------------
    attachInterrupt(PORT_SW, NULL, FALLING);
    //---------------------------------------
    motorAction(true, 100, false);
    // sr.println(WiFi.localIP());
    // sr.println("Setting mode");
    // mdeo = enmSetting;
    attachInterrupt(PORT_SW, swInterrupt, FALLING);
}

//*************************************
void setNowOpen(int open_time)
{
    openTime = open_time;
    deviceSts.nowOppenning = true;
}

//*************************************
void setAfter(int next_time, int adjustment_time = 0)
{
    if(setVal.interval == 0){
        setTimerInterrupt(NULL, 1000000000 , true);
        digitalWrite(PORT_LED_G, LED_OFF);
    }
    else
    {
        operationReservation = enmRegularOppenning;
        setTimerInterrupt(&afterOpenValve, (next_time - adjustment_time) * 1000000 , false);
        digitalWrite(PORT_LED_G, LED_ON);
        setVal.setTime = GetTime();
        setVal.settingReserv = false;
    }
}

//*************************************
void setDevice(int contentNum)
{
    String returnMessage = "";
    String paramSts = server.arg("sts");
    String paramLength = server.arg("length");
    String paramInterval = server.arg("interval");
    String paramArea = server.arg("area");
    switch(contentNum)
    {
        case enmNow://-------------------------------------------------------
            if(!deviceSts.opened)
            {
                receivedRing();
                if(paramLength != "") setVal.nowOpenLength = atoi(paramLength.c_str());
                else setVal.nowOpenLength = 1;
                // setVal.nextTime = getNextTime();
                deviceSts.nowRun = true;
                operationReservation = enmNowOppenning;
                returnMessage = "successed";
            }
            else{
                errorSound();
                returnMessage = "error";
            }
            break;
        case enmSet://-------------------------------------------------------
            if(paramLength != "") setVal.length = atoi(paramLength.c_str());
            if(paramInterval != "") {
                setVal.interval = atoi(paramInterval.c_str());
                setVal.settingReserv = true;
                receivedRing();
                returnMessage = "successed";
            }
            break;
        case enmAdjust:
            if(paramArea && !deviceSts.opened)
            {
                bool blF = false;
                receivedRing();
                deviceSts.opened = true;
                if(paramArea == "food") blF = true;
                motorAction(true, 10, blF);
                deviceSts.opened = false;
                returnMessage = "successed";
            }
            else
            {
                errorSound();
                returnMessage = "error";
            }
            break;
        default:
            if(paramSts == paramWord_set[enm_on]){
                receivedRing();
                switch(contentNum)
                {
                    case enm_buzzer:
                        httpSts[enmBuzzer].sts = true;
                        returnMessage = "Buzzer on";
                        break;
                    default:
                        break;
                }
            }
            else if(paramSts == paramWord_set[enm_off]){
                receivedRing();
                switch(contentNum)
                {
                    case enm_buzzer:
                        httpSts[enmBuzzer].sts = false;
                        returnMessage = "Buzzer off";
                        break;
                    default:
                        break;
                }
            }
            else{
                errorSound();
                // returnMessage = errorMessage[enmStsError_set];
                returnMessage = "error";
            }
            break;
    }
    server.send(200, "text/plain", returnMessage);
}

//*************************************
void outputValue()
{
    String returnMessage = "";
    String paramSts = server.arg("sts");
    
    if(paramSts == paramWord_get[enm_time])
    {
        receivedRing();
        struct tm nowTime = getTimeInf();
        char s[20] = {};
        arrangeTime(s, nowTime);
        returnMessage = s;
        // server.send(200, "text/plain", s);
    }
    else if(paramSts == paramWord_get[enm_temperture])
    {
        receivedRing();
        returnMessage = getTemp().c_str();
    }
    else if(paramSts == paramWord_get[enm_humidity])
    {
        receivedRing();
        returnMessage = getHumd().c_str();
    }

    else if(paramSts == paramWord_get[enm_all])
    {
        receivedRing();
        struct tm nowTime = getTimeInf();
        arrangeTime(s, nowTime);
        returnMessage = s;
        returnMessage += "\r";
        returnMessage += getTemp().c_str();
        returnMessage += "\r";
        returnMessage += getHumd().c_str();
    }
    else{
        errorSound();
        returnMessage = errorMessage[enmStsError_get];
    }
    server.send(200, "text/plain", returnMessage);
}

//*************************************
void outputTime()
{
    receivedRing();
    struct tm nowTime = getTimeInf();
    char s[20] = {};
    arrangeTime(s, nowTime);
    server.send(200, "text/plain", s);
}

//*************************************
string getTemp()
{
    float fTemp = readTempHTD21D(wr);
    sr.println(fTemp);
    return to_string(fTemp).substr(0, 5);
}

//*************************************
string getHumd()
{
    float fHumd = readHumdHTD21D(wr);
    return to_string(fHumd).substr(0, 5);
}

//*************************************
void closeValve_Now()
{
    blClosing = false;
    motorAction(true, 100, false);
    blOpened = false;
    if(!deviceSts.nowRun)setVal.settingReserv = true;
    else deviceSts.nowRun = false;
}

//*************************************
int getNextTime()
{
    long nowT = GetTime();
    if(nowT < setVal.setTime) nowT + 4294967295;
    return setVal.interval - ((nowT - setVal.setTime)/1000);
}

//*************************************
void IRAM_ATTR setClosing()
{
    deviceSts.closingRun = true;

}
//*************************************
// void IRAM_ATTR setClosing_Regular()
// {
//     deviceSts.closingRun = true;
// }

//*************************************
void IRAM_ATTR afterOpenValve()
{
    openTime = setVal.length * 1000000;
    deviceSts.regularOppenning = true;
}





//*************************************
//*************************************
//*************************************
//*************************************

//*************************************
//タイマー割り込み処理
//*************************************
void IRAM_ATTR setSwInterrupt()
{
    // sr.println(GetTime());
    attachInterrupt(PORT_SW, swInterrupt, FALLING);
}

//*************************************
void modeSetting()
{
    int swCnt_L = 0;
    int swCnt_H = 0;
    digitalWrite(PORT_LED_G, LED_ON);
    digitalWrite(PORT_LED_R, LED_ON);

    while(1)
    {
        if(digitalRead(PORT_SW) == LOW)
        {
            swCnt_L++;
            if(swCnt_L > 40){ break; }
        }
        else{ swCnt_L = 0; }
        delay(5);
    }

    digitalWrite(PORT_LED_G, LED_OFF);
    digitalWrite(PORT_LED_R, LED_OFF);
    mode = enmNormal;
    setTimerInterrupt(&onTimer, 1000000, false);
}