#include <Arduino.h>
// #include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <iostream>
#include <vector>
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
void IRAM_ATTR closeMotorW();
void IRAM_ATTR closeMotorF();
void IRAM_ATTR intervalStop();

void modeSetting();
void IRAM_ATTR onTimer();
void regularAction(deviceStatus& device, int& led_cnt, bool bl_food);

int getNextTime(deviceStatus device);
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
    
    xTaskCreatePinnedToCore(taskMotor_W, "taskMotor_W", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(taskMotor_F, "taskMotor_F", 4096, NULL, 1, NULL, 0);

    // xTaskCreatePinnedToCore(
    //        タスク名,
    //        "タスク名",
    //        スタックメモリサイズ,
    //        NULL,
    //        タスク優先順位,
    //        タスクハンドルポインタ,
    //        Core ID
    // );
}

//*************************************
void loop()
{
    server.handleClient();
}


//*************************************
void taskMotor_W(void* arg)
{
    int cntLED = 0;
    while(1){
        cntLED++;
        regularAction(deviceSts_W, cntLED, false);
    }
}

//*************************************
void taskMotor_F(void* arg)
{
    int cntLED = 0;
    while(1)
    {
        cntLED++;
        regularAction(deviceSts_F, cntLED, true);
    }
}

void regularAction(deviceStatus& device, int& led_cnt, bool bl_food)
{
    
    int len;
    bool ringed = false;
    bool canRing = true;
    deviceStatus *p_device;
    if(bl_food) p_device = &deviceSts_F;
    else p_device = &deviceSts_W;

    if(device.oppenning && !device.opened)
    {
        if(bl_food && deviceSts_W.ringing) canRing = false;
        else if(deviceSts_F.ringing) canRing = false;
        else canRing = true;
        if (canRing && device.ring && !device.ringing)
        {
            device.ringing = true;
            p_device = &device;
            setHctMelody(sr, device.melody.c_str());
            device.ringing = false;
            ringed = true;
            p_device = &device;
        }

        digitalWrite(device.portLED, LED_ON);
        device.oppenning = false;
        device.opened = true;
        motorAction(MOTOR_OPEN, MOTOR_RANGE, bl_food);
        if(device.flgNow) len = device.nowLength;
        else len = device.length;
        device.flgNow = false;

        if(!ringed)
        {
            canRing = true;
            if(bl_food && deviceSts_W.ringing) canRing = false;
            else if(deviceSts_F.ringing) canRing = false;
            else canRing = true;
            if (canRing && device.ring && !device.ringing)
            {
                device.ringing = true;
                p_device = &device;
                setHctMelody(sr, device.melody.c_str());
                device.ringing = false;
                ringed = true;
                p_device = &device;
            }
        }
        
        if(!bl_food)
            setTimerInterrupt(device.tClose, device.timerNumClose, closeMotorW, len * 1000000, false);
        else
            setTimerInterrupt(device.tClose, device.timerNumClose, closeMotorF, len * 1000000, false);
    }
    if(device.closing && device.opened)
    {
        motorAction(MOTOR_CLOSE, MOTOR_RANGE, bl_food);
        digitalWrite(device.portLED, LED_OFF);
        device.closing = false;
        device.opened = false;
    }
    if(device.adjustment && !device.opened)
    {
        motorAction(device.adjustmentLeft, MOTOR_ADJUST_RANGE, bl_food);
        device.adjustment = false;
    }
    led_cnt++;
    if(led_cnt > 1000)
    {
        led_cnt = 0;
        if(device.interval > 0 && !device.opened){
            digitalWrite(device.portLED, !digitalRead(device.portLED));
        }
    }
    delay(1);

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
    deviceStatus device;
    deviceStatus *p_device;
    auto *func = &openMotorF;

    int portLED;

    String returnMessage = "";
    // String paramSts = server.arg("sts");
    String paramMelody = server.arg("melody");
    String paramLength = server.arg("length");
    String paramInterval = server.arg("interval");
    String paramTarget = server.arg("target");
    String paramDirection = server.arg("direction");
    String paramRing = server.arg("ring");

    switch(contentNum)
    {
        case enmNow://-------------------------------------------------------
            if(paramTarget == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
                sr.println("area = w");
                func = &openMotorW;
            }
            else if(paramTarget == "f")
            {
                sr.println("area = f");
                device = deviceSts_F;
                p_device = &deviceSts_F;
                func = &openMotorF;
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }
            
            if( device.opened == false && 
                device.oppenning == false && 
                device.closing == false)
            {
                bzReceivedRing();
                if(paramLength != "") device.nowLength = atoi(paramLength.c_str());
                else device.nowLength = DEFAULT_LENGTH;
                device.flgNow = true;
                device.oppenning = true;
                *p_device = device;
                returnMessage = "successed";
            }
            else{
                bzErrorSound();
                returnMessage = "error";
            }
            break;
        case enmSet://-------------------------------------------------------
            if(paramTarget == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
                func = &openMotorW;

                portLED = PORT_LED_G;
                // bzReceivedRing();
            }
            else if(paramTarget == "f")
            {
                device = deviceSts_F;
                p_device = &deviceSts_F;
                func = &openMotorF;

                portLED = PORT_LED_R;
                // bzReceivedRing();
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }

            if(paramMelody != "" || paramRing == "true" || paramRing == "false")
            {
                if(paramMelody != ""){
                    device.melody = paramMelody;
                    sr.println(paramMelody);
                    setHctMelody(sr, device.melody.c_str());
                }
                if(paramRing == "true" || paramRing == "false")
                {
                    if(paramRing == "true")
                        device.ring = true;
                    else
                        device.ring = false;
                }
                bzReceivedRing();
                *p_device = device;
                returnMessage = "successed";
            }
            else if(paramLength != "") {
                device.length = atoi(paramLength.c_str());
                *p_device = device;
                bzReceivedRing();
                returnMessage = "successed";
            }
            else if(paramInterval == "0")
            {
                sr.println("interval stop");
                device.interval =  0;
                // stopTimerInterrupt(p_t);
                // timerEnd(p_t);
                setTimerInterrupt(device.tOpen, device.timerNumOpen, intervalStop, 0, false);
                digitalWrite(device.portLED, LED_OFF);
                *p_device = device;
                bzReceivedRing();
                returnMessage = "successed";
            }
            else if(paramInterval != "") {
                device.interval = atoi(paramInterval.c_str());
                if(device.interval < 10)device.interval = 10;
                setTimerInterrupt(device.tOpen, device.timerNumOpen, func, device.interval * 1000000, true);
                device.setTime = GetTime();
                digitalWrite(device.portLED, LED_ON);
                *p_device = device;
                bzReceivedRing();
                returnMessage = "successed";
            }
            // else if(paramRing == "true" || paramRing == "false")
            // {
            //     if(paramRing == "true")
            //         device.ring = true;
            //     else
            //         device.ring = false;
            //     *p_device = device;
            //     bzReceivedRing();
            //     returnMessage = "successed";
            // }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }
            break;

        case enmAdjust://--------------------------------------------------
            bool blF = false;
            bool blLeft = false;
            if(paramTarget == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
            }
            else if(paramTarget == "f")
            {
                device = deviceSts_F;
                p_device = &deviceSts_F;
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }

            if( device.opened == false && 
                device.oppenning == false && 
                device.closing == false)
            {
                bzReceivedRing();
                if(paramDirection == "l") device.adjustmentLeft = true;
                else device.adjustmentLeft = false;
                device.adjustment = true;
                *p_device = device;
                returnMessage = "successed";
            }
            else
            {
                bzErrorSound();
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
    String paramItem = server.arg("item");
    String paramTarget = server.arg("target");

    deviceStatus device;
    deviceStatus *p_device;
    // if(paramSts == paramWord_get[enm_time])
    // {
    //     bzReceivedRing();
    //     struct tm nowTime = getTimeInf();
    //     char s[20] = {};
    //     arrangeTime(s, nowTime);
    //     returnMessage = s;
    //     // server.send(200, "text/plain", s);
    // }
    if(paramItem == paramWord_get[enm_temperture])
    {
        bzReceivedRing();
        returnMessage = getTemp().c_str();
    }
    else if(paramItem == paramWord_get[enm_humidity])
    {
        bzReceivedRing();
        returnMessage = getHumd().c_str();
    }
    else if(paramItem == "dq")
    {
        Bz_DragonQuest_Preface();
    }
    else
    {
        if(paramTarget == "w")
        { 
            device = deviceSts_W;
            p_device = &deviceSts_W;
        }
        else if(paramTarget == "f")
        {
            device = deviceSts_F;
            p_device = &deviceSts_F;
        }
        else
        {
            bzErrorSound();
            returnMessage = "error";
            server.send(200, "text/plain", returnMessage);
            return;
        }
        if(paramItem == paramWord_get[enm_interval])
        {
            bzReceivedRing();
            returnMessage = device.interval;
        }
        else if(paramItem == paramWord_get[enm_ring])
        {
            bzReceivedRing();
            if(device.ring) returnMessage = "true";
            else returnMessage = "false";
        }
        else if(paramItem == paramWord_get[enm_melody])
        {
            bzReceivedRing();
            returnMessage = device.melody;
            setHctMelody(sr, device.melody.c_str());
        }
        else if(paramItem == paramWord_get[enm_next])
        {
            // sr.println(getNextTime(device));
            bzReceivedRing();
            returnMessage = getNextTime(device);
        }
        else if(paramItem == "settime")
        {
            bzReceivedRing();
            returnMessage = device.setTime;
        }
        else if(paramItem == paramWord_get[enm_all])
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
    }
    server.send(200, "text/plain", returnMessage);
}

//*************************************
void IRAM_ATTR openMotorW()
{
    deviceSts_W.setTime = GetTime();
    deviceSts_W.oppenning = true;
} 

//*************************************
void IRAM_ATTR openMotorF()
{
    deviceSts_F.setTime = GetTime();
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
void IRAM_ATTR intervalStop(){ } 

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
int getNextTime(deviceStatus device)
{
    if(device.interval == 0)
    {
        return 0;
    }
    else
    {
        long nowT = GetTime();
        long setT = device.setTime;
        sr.print("set time：");
        sr.println(setT);
        sr.print("now time：");
        sr.println(nowT);
        sr.print("result：");
        sr.println(device.interval - ((nowT - setT)/1000));
        
        if(nowT < setT) nowT + 4294967295;
        return device.interval - ((nowT - setT)/1000);
    }
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
    setTimerInterrupt(tSettingOff, 5, &onTimer, 1000000, false);
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