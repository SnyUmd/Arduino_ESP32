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

void taskMotor_W(void* arg);
void taskMotor_F(void* arg);

void IRAM_ATTR openMotorW();
void IRAM_ATTR openMotorF();

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
    // xTaskCreatePinnedToCore(taskMotor_W, "taskMotor_W", 4096, NULL, 1, NULL, 0);
    // xTaskCreatePinnedToCore(taskMotor_F, "taskMotor_F", 4096, NULL, 1, NULL, 1);
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

    if(deviceSts_W.oppenning && !deviceSts_W.opened)
    {
        sr.println("Run = w");
        motorAction(MOTOR_OPEN, 50, false);
        deviceSts_W.oppenning = false;
    }

    if(deviceSts_F.oppenning && !deviceSts_F.opened)
    {
        sr.println("Run = f");
        motorAction(MOTOR_OPEN, 50, true);
        deviceSts_F.oppenning = false;
    }

    if(deviceSts_W.closing && deviceSts_W.opened)
    {
        motorAction(MOTOR_CLOSE, 50, false);
        deviceSts_W.closing = false;
    }

    if(deviceSts_F.closing && deviceSts_F.opened)
    {
        motorAction(MOTOR_CLOSE, 50, true);
        deviceSts_F.closing = false;
    }
    // if(setVal.settingReserv && !deviceSts.opened) {
    //     setAfter(setVal.interval, 0);
    //     setVal.settingReserv = false;
    // }
}

//*************************************
void taskMotor_W(void* arg)
{
    while(1)
    {
        sr.println("task motor W");
        delay(500);
    }
}

//*************************************
void taskMotor_F(void* arg)
{
    while(1)
    {
        sr.println("task motor F");
        delay(500);
    }
}


//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************

//*************************************
void setHttpAction()
{
    //バルブオープン
    server.on(httpContents[enmNow], HTTP_ANY, [](){setDevice(enmNow);});
    //アフター設定
    server.on(httpContents[enmSet], HTTP_ANY, [](){setDevice(enmSet);});

    //値取得
    server.on(httpContents[enmGet], HTTP_ANY, [](){outputValue();});

    //位置の調整
    server.on(httpContents[enmAdjust], HTTP_ANY, [](){setDevice(enmAdjust);});

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        bzErrorSound();
        server.send(404, "text/plain", errorMessage[enmNotFound]); // 404を返す
    });

    server.begin();
}

//*************************************
void setDevice(int contentNum)
{
    bool *p_blOpened;
    bool *p_blOppenning;
    bool *p_blClosing;
    int *p_interval;
    int *p_length;
    int *p_nowLength;
    hw_timer_t *p_t;
    deviceStatus *p_device;
    auto *func = &openMotorF;

    String returnMessage = "";
    String paramSts = server.arg("sts");
    String paramLength = server.arg("length");
    String paramInterval = server.arg("interval");
    String paramArea = server.arg("area");

    switch(contentNum)
    {
        case enmNow://-------------------------------------------------------
            if(paramArea == "w")
            { 
                // p_device = &deviceSts_W;
                sr.println("area = w");
                p_blOpened = &deviceSts_W.opened;
                p_blOppenning = &deviceSts_W.oppenning;
                p_blClosing = &deviceSts_W.closing;
                p_nowLength = &deviceSts_W.nowLength;
                p_t = tNowOpen_W;
                func = &openMotorW;
            }
            else if(paramArea == "f")
            {
                sr.println("area = f");
                // p_device = &deviceSts_F;
                p_blOpened = &deviceSts_F.opened;
                p_blOppenning = &deviceSts_F.oppenning;
                p_blClosing = &deviceSts_F.closing;
                p_nowLength = &deviceSts_F.nowLength;
                p_t = tNowOpen_F;
                func = &openMotorF;
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }
            
            if( *p_blOpened == false && 
                *p_blOppenning == false && 
                *p_blClosing == false)
            {
                bzReceivedRing();
                if(paramLength != "") *p_nowLength = atoi(paramLength.c_str());
                else *p_nowLength = DEFAULT_LENGTH;
                setTimerInterrupt(p_t, *func, 0, false);
                returnMessage = "successed";
            }
            else{
                bzErrorSound();
                returnMessage = "error";
            }
            break;
        case enmSet://-------------------------------------------------------
            if(paramArea == "w")
            { 
                // p_device = &deviceSts_W;
                // p_blOpened = &deviceSts_W.opened;
                // p_blOppenning = &deviceSts_W.oppenning;
                // p_blClosing = &deviceSts_W.closing;
                p_interval = &deviceSts_W.interval;
                p_length = &deviceSts_W.length;
                p_t = tOpen_W;
                func = &openMotorW;
            }
            else if(paramArea == "f")
            {
                // p_device = &deviceSts_F;
                // p_blOpened = &deviceSts_F.opened;
                // p_blOppenning = &deviceSts_F.oppenning;
                // p_blClosing = &deviceSts_F.closing;
                p_interval = &deviceSts_F.interval;
                p_length = &deviceSts_F.length;
                p_t = tOpen_F;
                func = &openMotorF;
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }
            if(paramLength != "") *p_length = atoi(paramLength.c_str());
            if(paramInterval == "0")
            {
                *p_interval =  0;
                stopTimerInterrupt(p_t);
            }
            else if(paramInterval != "") {
                *p_interval =  atoi(paramInterval.c_str());
                setTimerInterrupt(p_t, func, *p_interval, true);
                bzReceivedRing();
                returnMessage = "successed";
            }
            break;

        case enmAdjust://--------------------------------------------------
            bool blF = false;
            if(paramArea == "w")
            { 
                // p_device = &deviceSts_W;
                p_blOpened = &deviceSts_W.opened;
                p_blOppenning = &deviceSts_W.oppenning;
                p_blClosing = &deviceSts_W.closing;
                blF = false;
            }
            else if(paramArea == "f")
            {
                // p_device = &deviceSts_F;
                p_blOpened = &deviceSts_F.opened;
                p_blOppenning = &deviceSts_F.oppenning;
                p_blClosing = &deviceSts_F.closing;
                blF = true;
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }

            if( *p_blOpened == false && 
                *p_blOppenning == false && 
                *p_blClosing == false)
            {
                bzReceivedRing();
                *p_blOpened = true;
                *p_blOppenning = true;
                motorAction(true, 10, blF);
                *p_blOpened = false;
                *p_blOppenning = false;
                returnMessage = "successed";
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
            }
            break;
        // default:
        //     bzErrorSound();
        //     // returnMessage = errorMessage[enmStsError_set];
        //     returnMessage = "error";
        //     break;
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
        bzReceivedRing();
        struct tm nowTime = getTimeInf();
        char s[20] = {};
        arrangeTime(s, nowTime);
        returnMessage = s;
        // server.send(200, "text/plain", s);
    }
    else if(paramSts == paramWord_get[enm_temperture])
    {
        bzReceivedRing();
        returnMessage = getTemp().c_str();
    }
    else if(paramSts == paramWord_get[enm_humidity])
    {
        bzReceivedRing();
        returnMessage = getHumd().c_str();
    }

    else if(paramSts == paramWord_get[enm_all])
    {
        bzReceivedRing();
        struct tm nowTime = getTimeInf();
        arrangeTime(s, nowTime);
        returnMessage = s;
        returnMessage += "\r";
        returnMessage += getTemp().c_str();
        returnMessage += "\r";
        returnMessage += getHumd().c_str();
    }
    else{
        bzErrorSound();
        returnMessage = errorMessage[enmStsError_get];
    }
    server.send(200, "text/plain", returnMessage);
}

//*************************************
void IRAM_ATTR openMotorW()
{
    sr.println("intterrupt = w");
    deviceSts_W.oppenning = true;
} 

//*************************************
void IRAM_ATTR openMotorF()
{
    sr.println("intterrupt = f");
    deviceSts_F.oppenning = true;
} 

//*************************************
void IRAM_ATTR closeMotorW()
{
    deviceSts_W.closing = true;
} 
//*************************************
void IRAM_ATTR closeMotorF()
{
    deviceSts_F.closing = true;
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

// //*************************************
// void closeValve_Now()
// {
//     blClosing = false;
//     motorAction(true, 100, false);
//     blOpened = false;
//     if(!deviceSts.nowRun)setVal.settingReserv = true;
//     else deviceSts.nowRun = false;
// }

//*************************************
int getNextTime(bool blFood)
{
    int *setT;
    if(!blFood)
    {
        setT = &deviceSts_W.setTime;
    }
    else
    {
        setT = &deviceSts_F.setTime;
    }
    long nowT = GetTime();
    if(nowT < *setT) nowT + 4294967295;
    return *setT - ((nowT - *setT)/1000);
}

//*************************************
// void IRAM_ATTR setClosing()
// {
//     deviceSts.closingRun = true;

// }
//*************************************
// void IRAM_ATTR setClosing_Regular()
// {
//     deviceSts.closingRun = true;
// }

//*************************************
// void IRAM_ATTR afterOpenValve()
// {
//     openTime = setVal.length * 1000000;
//     deviceSts.regularOppenning = true;
// }

//*************************************
//SWポート割り込み処理
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
    setTimerInterrupt(tSettingOff, &onTimer, 1000000, false);
}

//*************************************
//現在時刻取得
void outputTime()
{
    bzReceivedRing();
    struct tm nowTime = getTimeInf();
    char s[20] = {};
    arrangeTime(s, nowTime);
    server.send(200, "text/plain", s);
}