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

void regularAction(deviceStatus& device, int& led_cnt, bool bl_food)
{
    
    int len;
    bool ringed = false;

    if(device.oppenning && !device.opened)
    {
        if (device.ring && !device.ringing)
        {
            device.ringing = true;
            setHctMelody(sr, device.melody.c_str());
            device.ringing = false;
            ringed = true;
        }

        digitalWrite(device.portLED, LED_ON);
        device.oppenning = false;
        device.opened = true;
        motorAction(MOTOR_OPEN, 50, bl_food);
        if(device.flgNow) len = device.nowLength;
        else len = device.length;
        device.flgNow = false;

        if (!ringed && device.ring && !device.ringing)
        {
            device.ringing = true;
            setHctMelody(sr, device.melody.c_str());
            device.ringing = false;
            ringed = true;
        }
        if(!bl_food)
            setTimerInterrupt(device.tClose, device.timerNumClose, closeMotorW, len * 1000000, false);
        else
            setTimerInterrupt(device.tClose, device.timerNumClose, closeMotorF, len * 1000000, false);
    }
    if(device.closing && device.opened)
    {
        motorAction(MOTOR_CLOSE, 50, bl_food);
        digitalWrite(device.portLED, LED_OFF);
        device.closing = false;
        device.opened = false;
    }
    if(device.adjustment && !device.opened)
    {
        motorAction(device.adjustmentLeft, 10, bl_food);
        device.adjustment = false;
    }
    led_cnt++;
    // sr.println(led_cnt);
    if(led_cnt > 1000)
    {
        led_cnt = 0;
        if(device.interval > 0 && !device.opened){
            sr.println(digitalRead(device.portLED));
            digitalWrite(device.portLED, !digitalRead(device.portLED));
        }
    }
    delay(1);

}

//*************************************
void taskMotor_W(void* arg)
{
    // clsAction clsAction_W;
    int cntLED = 0;
    while(1){
        cntLED++;
        regularAction(deviceSts_W, cntLED, false);
        // clsAction_W.regularAction(deviceSts_W, cntLED, false);
        // regularAction(deviceSts_W, cntLED);
        // bool ringed = false;
        // if(deviceSts_W.oppenning && !deviceSts_W.opened)
        // {
        //     if (deviceSts_W.ring && !deviceSts_W.ringing)
        //     {
        //         deviceSts_W.ringing = true;
        //         setHctMelody(sr, deviceSts_W.melody.c_str());
        //         deviceSts_W.ringing = false;
        //         ringed = true;
        //     }

        //     deviceSts_W.oppenning = false;
        //     deviceSts_W.opened = true;
        //     motorAction(MOTOR_OPEN, 50, false);
        //     if(deviceSts_W.flgNow) len = deviceSts_W.nowLength;
        //     else len = deviceSts_W.length;
        //     deviceSts_W.flgNow = false;

        //     if (!ringed && deviceSts_W.ring && !deviceSts_W.ringing)
        //     {
        //         deviceSts_W.ringing = true;
        //         setHctMelody(sr, deviceSts_W.melody.c_str());
        //         deviceSts_W.ringing = false;
        //         ringed = true;
        //     }
        //     setTimerInterrupt(deviceSts_W.tClose, deviceSts_W.timerNumClose, closeMotorW, len * 1000000, false);
        // }
        // if(deviceSts_W.closing && deviceSts_W.opened)
        // {
        //     deviceSts_W.closing = false;
        //     deviceSts_W.opened = false;
        //     motorAction(MOTOR_CLOSE, 50, false);
        // }
        // if(deviceSts_W.adjustment && !deviceSts_W.opened)
        // {
        //     motorAction(deviceSts_W.adjustmentLeft, 10, false);
        //     deviceSts_W.adjustment = false;
        // }
        // cntLED++;
        // sr.println(cntLED);
        // if(cntLED > 100)
        // {
        //     cntLED = 0;
        //     if(deviceSts_W.interval > 0){
        //         sr.println(digitalRead(PORT_LED_G));
        //         digitalWrite(PORT_LED_R, !digitalRead(PORT_LED_G));
        //     }
        // }
        // delay(1);
    }
}

//*************************************
void taskMotor_F(void* arg)
{
    // clsAction clsAction_F;
    int len;
    int cntLED = 0;
    while(1)
    {
        cntLED++;
        regularAction(deviceSts_F, cntLED, true);
        // clsAction_F.regularAction(deviceSts_F, cntLED, true);
    }
    // while(1)
    // {
    //     // sr.println("task motor F");
    //     // delay(500);
    //     bool ringed = false;
    //     if(deviceSts_F.oppenning && !deviceSts_F.opened)
    //     {
            
    //         if (deviceSts_F.ring && !deviceSts_F.ringing)
    //         {
    //             deviceSts_F.ringing = true;
    //             setHctMelody(sr, deviceSts_F.melody.c_str());
    //             deviceSts_F.ringing = false;
    //             ringed = true;
    //         }

    //         deviceSts_F.oppenning = false;
    //         deviceSts_F.opened = true;
    //         motorAction(MOTOR_OPEN, 50, true);
    //         if(deviceSts_F.flgNow) len = deviceSts_F.nowLength;
    //         else len = deviceSts_F.length;
    //         deviceSts_F.flgNow = false;

    //         if (!ringed && deviceSts_F.ring && !deviceSts_F.ringing)
    //         {
    //             deviceSts_F.ringing = true;
    //             setHctMelody(sr, deviceSts_F.melody.c_str());
    //             deviceSts_F.ringing = false;
    //             ringed = true;
    //         }
    //         setTimerInterrupt(deviceSts_F.tClose, deviceSts_F.timerNumClose, closeMotorF, len * 1000000, false);
    //     }
    //     if(deviceSts_F.closing && deviceSts_F.opened)
    //     {
    //         motorAction(MOTOR_CLOSE, 50, true);
    //         deviceSts_F.closing = false;
    //         deviceSts_F.opened = false;
    //     }
    //     if(deviceSts_F.adjustment && !deviceSts_F.opened)
    //     {
    //         motorAction(deviceSts_F.adjustmentLeft, 10, true);
    //         deviceSts_F.adjustment = false;
    //     }
    //     cntLED++;
    //     // sr.println(cntLED);
    //     if(cntLED > 100)
    //     {
    //         cntLED = 0;
    //         if(deviceSts_F.interval > 0){
    //             sr.println(digitalRead(PORT_LED_R));
    //             digitalWrite(PORT_LED_R, !digitalRead(PORT_LED_R));
    //         }
    //     }
    //     delay(1);
    // }
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
    String paramSts = server.arg("sts");
    String paramMelody = server.arg("melody");
    String paramLength = server.arg("length");
    String paramInterval = server.arg("interval");
    String paramArea = server.arg("area");
    String paramDirection = server.arg("direction");
    String paramRing = server.arg("ring");

    switch(contentNum)
    {
        case enmNow://-------------------------------------------------------
            if(paramArea == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
                sr.println("area = w");
                func = &openMotorW;
            }
            else if(paramArea == "f")
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
            if(paramArea == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
                func = &openMotorW;

                portLED = PORT_LED_G;
                // bzReceivedRing();
            }
            else if(paramArea == "f")
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
            if(paramMelody != "")
            {
                device.melody = paramMelody;
                sr.println(paramMelody);

                // setHctMelody(sr, device.melody.c_str());

                *p_device = device;
                bzReceivedRing();
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
                digitalWrite(portLED, LED_OFF);
                *p_device = device;
                bzReceivedRing();
                returnMessage = "successed";
            }
            else if(paramInterval != "") {
                device.interval = atoi(paramInterval.c_str());
                setTimerInterrupt(device.tOpen, device.timerNumOpen, func, device.interval * 1000000, true);
                device.setTime = GetTime();
                digitalWrite(portLED, LED_ON);
                *p_device = device;
                bzReceivedRing();
                returnMessage = "successed";
            }
            else if(paramRing == "true" || paramRing == "false")
            {
                if(paramRing == "true")
                    device.ring = true;
                else
                    device.ring = false;
                *p_device = device;
                bzReceivedRing();
                returnMessage = "successed";
            }
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
            if(paramArea == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
            }
            else if(paramArea == "f")
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