#include <Arduino.h>
// #include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <iostream>
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
void IRAM_ATTR onTimer();
void funcInterrupt();
void modeSetting();
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

//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
void setup()
{
    powerOn();
    sr.println("");
    init();
    // sr.println("");
    delay(1000);
    wifiInit(WiFi, sr, SSID, PASS, HOST_NAME, false);
    // timeInf = getTimeInf();
    // arrangeTime(s, timeInf);
    // sr.println(s);
    setHttpAction();
    sr.println("-----loop Start-----");
    BzGoUp(10, 10);
    
    // setVal.interval = 20;
    // operationReservation = enmRegularOppenning;
    // setAfter(setVal.interval, setVal.length);
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
                motorAction(false, 100);
                deviceSts.nowOppenning = false;
                setTimerInterrupt(&setClosing, setVal.nowOpenLength * 1000000, false);
                operationReservation = enmNowClosing;
            }
            break;
        case enmNowClosing://2
            if (deviceSts.closingRun){
                motorAction(true, 100);
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
                motorAction(false, 100);
                deviceSts.nowOppenning = false;
                setTimerInterrupt(&setClosing, 2 * 1000000, false);
                operationReservation = enmRegularClosing;
                deviceSts.regularOppenning = false;
            }
            break;
        case enmRegularClosing://4
            if (deviceSts.closingRun){
                motorAction(true, 100);
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
void init()
{
    sr.begin(115200);
    initI2C(wr);
    
    //ポート割り込み処理
    pinMode(PORT_SW, INPUT_PULLUP);
    attachInterrupt(PORT_SW, funcInterrupt, FALLING);

    //LEDポート
    pinMode(PORT_LED_G, OUTPUT);
    pinMode(PORT_LED_R, OUTPUT);
    digitalWrite(PORT_LED_G, 1);
    digitalWrite(PORT_LED_R, 1);

    //ステッピングモータ　ポート
    pinMode(PORT_MOTOR0, OUTPUT);
    pinMode(PORT_MOTOR1, OUTPUT);
    pinMode(PORT_MOTOR2, OUTPUT);
    pinMode(PORT_MOTOR3, OUTPUT);
    digitalWrite(PORT_MOTOR0, 0);
    digitalWrite(PORT_MOTOR1, 0);
    digitalWrite(PORT_MOTOR2, 0);
    digitalWrite(PORT_MOTOR3, 0);

    LC.ledFlash(PORT_LED_R, 10, 5);
    
    InitBz();
    digitalWrite(PORT_LED_R, 1);
}

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

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        errorSound();
        server.send(404, "text/plain", errorMessage[enmNotFound]); // 404を返す
    });

    server.begin();
}

//*************************************
//タイマー割り込み処理
//*************************************
void IRAM_ATTR onTimer()
{
    // sr.println(GetTime());
    attachInterrupt(PORT_SW, funcInterrupt, FALLING);
}

//*************************************
//ポート割り込み処理
//*************************************
void funcInterrupt()
{
    //チャタリング対策------------------------
    attachInterrupt(PORT_SW, NULL, FALLING);
    //---------------------------------------

    sr.println(WiFi.localIP());
    sr.println("Setting mode");
    mode = enmSetting;
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
    String paraminterval = server.arg("interval");
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
            if(paraminterval != "") {
                setVal.interval = atoi(paraminterval.c_str());
                setVal.settingReserv = true;
                receivedRing();
                returnMessage = "successed";
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
    motorAction(true, 100);
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