/*
ハッカソンデバイス
1．電源投入
2．Setup()処理スタート
3．loop()処理がループ
　　■マルチタスク
　　　タスク１：taskMotor_Wがループ
　　　タスク２：taskMotor_Fがループ
*/

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
#include "module/BzCtrl.h"
#include "module/htu21d.h"
#include "module/i2cCtrl.h"
#include "module/motorCtrl.h"

using namespace std;

ledCtrl LC;

void init();//初期動作
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

void IRAM_ATTR onTimer();
void regularAction(deviceStatus& device, int& led_cnt, bool bl_food);
String GetResponsMessage(String target, String action, String item = "", String value = "");
String GetResponsMessage_all(String target, String action, String item = "", String value = "");

int getNextTime(deviceStatus device);
//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
//起動時のセットアップ処理
//電源投入時にここからスタート
void setup()
{
    initPort();

    LC.ledFlash(PORT_LED_F, 5, 3);
    LC.ledFlash(PORT_LED_W, 5, 3);
    digitalWrite(PORT_LED_F, LED_OFF);
    digitalWrite(PORT_LED_W, LED_OFF);

    // 割り込み
    attachInterrupt(PORT_SW, swInterrupt, FALLING);
    sr.begin(115200);
    initI2C(wr);
    InitBz();
    bzPowerOn();

    digitalWrite(PORT_LED_F, LED_ON);
    digitalWrite(PORT_LED_W, LED_ON);

    if(!wifiInit(WiFi, sr, SSID, PASS, HOST_NAME, false))
    {
        bzErrorSound();
        while(1)
        {
            digitalWrite(PORT_LED_F, !digitalRead(PORT_LED_F));
            digitalWrite(PORT_LED_W, !digitalRead(PORT_LED_W));
            delay(100);
        }
    }
    digitalWrite(PORT_LED_F, LED_OFF);
    digitalWrite(PORT_LED_W, LED_OFF);
    delay(500);

    setHttpAction();

    //モータ駆動確認
    digitalWrite(PORT_LED_F, LED_ON);
    motorAction(MOTOR_OPEN, 20, true);
    digitalWrite(PORT_LED_W, LED_ON);
    motorAction(MOTOR_OPEN, 20, false);
    digitalWrite(PORT_LED_F, LED_OFF);
    motorAction(MOTOR_CLOSE, 20, true);
    digitalWrite(PORT_LED_W, LED_OFF);
    motorAction(MOTOR_CLOSE, 20, false);

    BzGoUp(5, 10);
    
    sr.println("-----loop Start-----");

    xTaskCreatePinnedToCore(taskMotor_W, "taskMotor_W", 4096, NULL, 1, NULL, 0);
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

    if(device.oppenning && !device.opened)
    {
        if(bl_food && deviceSts_W.ringing) canRing = false;
        else if(deviceSts_F.ringing) canRing = false;
        else if(setRinging) canRing = false;
        else canRing = true;

        if (canRing && device.ring && !device.ringing)
        {
            device.ringing = true;
            setHctMelody(sr, device.melody.c_str());
            device.ringing = false;
            ringed = true;
        }

        digitalWrite(device.portLED, LED_ON);
        device.oppenning = false;
        device.opened = true;

        if(!bl_food)
            setTimerInterrupt(device.tClose, device.timerNumClose, closeMotorW, /*len*/DEFAULT_LENGTH * 1000000, false);
        else
            setTimerInterrupt(device.tClose, device.timerNumClose, closeMotorF, /*len*/DEFAULT_LENGTH * 1000000, false);
        
        motorAction(MOTOR_OPEN, MOTOR_RANGE, bl_food);
        device.flgNow = false;

        if(!ringed)
        {
            canRing = true;
            if(bl_food && deviceSts_W.ringing) canRing = false;
            else if(deviceSts_F.ringing) canRing = false;
            else if(setRinging) canRing = false;
            else canRing = true;
            if (canRing && device.ring && !device.ringing)
            {
                device.ringing = true;
                setHctMelody(sr, device.melody.c_str());
                device.ringing = false;
                ringed = true;
            }
        }     
    }
    if(device.closing && device.opened)
    {
        device.closing = false;
        device.opened = false;
        motorAction(MOTOR_CLOSE, MOTOR_RANGE, bl_food);
        digitalWrite(device.portLED, LED_OFF);
    }
    if(device.adjustment && !device.opened)
    {
        digitalWrite(device.portLED, LED_ON);
        motorAction(device.adjustmentLeft, device.adjustMotion, bl_food);
        digitalWrite(device.portLED, LED_OFF);
        device.adjustment = false;
    }
    led_cnt++;
    if(led_cnt > 1500)
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
    //セット
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
    String paramMelody = server.arg("melody");
    String paramInterval = server.arg("interval");
    String paramTarget = server.arg("target");
    String paramDirection = server.arg("direction");
    String paramRing = server.arg("ring");
    String paramMotion = server.arg("motion");

    String Position = "water";
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
                Position = "food";
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
                device.flgNow = true;
                device.oppenning = true;
                *p_device = device;
                returnMessage = GetResponsMessage(Position, "now");
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

                portLED = PORT_LED_W;
            }
            else if(paramTarget == "f")
            {
                device = deviceSts_F;
                p_device = &deviceSts_F;
                func = &openMotorF;
                Position = "food";

                portLED = PORT_LED_F;
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

                    if(getNextTime(device) < 10){
                        while(deviceSts_F.ringing || deviceSts_W.ringing);
                        setRinging = true;
                        setHctMelody(sr, device.melody.c_str());
                        setRinging = false;
                    }
                    returnMessage = GetResponsMessage(Position, "set", "melody", device.melody);
                }
                if(paramRing == "true" || paramRing == "false")
                {
                    if(paramRing == "true") device.ring = true;
                    else device.ring = false;
                    returnMessage = GetResponsMessage(Position, "set", "ring", paramRing);
                }
                bzReceivedRing();
                *p_device = device;
            }
            else if(paramInterval == "0")
            {
                sr.println("interval stop");
                device.interval =  0;
                setTimerInterrupt(device.tOpen, device.timerNumOpen, intervalStop, 0, false);
                digitalWrite(device.portLED, LED_OFF);
                *p_device = device;
                bzReceivedRing();
                returnMessage = GetResponsMessage(Position, "set", "interval", to_string(device.interval).c_str());
            }
            else if(paramInterval != "") {
                device.interval = atoi(paramInterval.c_str());
                if(device.interval < 10)device.interval = 10;
                setTimerInterrupt(device.tOpen, device.timerNumOpen, func, device.interval * 1000000, true);
                device.setTime = GetTime();
                digitalWrite(device.portLED, LED_ON);
                *p_device = device;
                bzReceivedRing();
                returnMessage = GetResponsMessage(Position, "set", "interval", to_string(device.interval).c_str());
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

                if(paramMotion != "") sscanf(paramMotion.c_str(), "%d", &device.adjustMotion);

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

    String Position = "water";

    if(paramItem == paramWord_get[enm_time])
    {
        bzReceivedRing();
        struct tm nowTime = getTimeInf();
        char s[20] = {};
        arrangeTime(s, nowTime);
        returnMessage = s;
    }
    else if(paramItem == paramWord_get[enm_temperture])
    {
        bzReceivedRing();
        returnMessage = getTemp().c_str();
    }
    else if(paramItem == paramWord_get[enm_humidity])
    {
        bzReceivedRing();
        returnMessage = getHumd().c_str();
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
            Position = "food";
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
            returnMessage = GetResponsMessage(Position, "output", paramItem, to_string(device.interval).c_str());
        }
        else if(paramItem == paramWord_get[enm_ring])
        {
            bzReceivedRing();
            if(device.ring) returnMessage = GetResponsMessage(Position, "output", paramItem, "true");
            else returnMessage = GetResponsMessage(Position, "output", paramItem, "false");
        }
        else if(paramItem == paramWord_get[enm_melody])
        {
            bzReceivedRing();
            returnMessage = GetResponsMessage(Position, "output", paramItem, device.melody);
            setHctMelody(sr, device.melody.c_str());
            
        }
        else if(paramItem == paramWord_get[enm_next])
        {
            bzReceivedRing();
            returnMessage = GetResponsMessage(Position, "output", paramItem, to_string(getNextTime(device)).c_str());
        }
        else if(paramItem == "settime")
        {
            bzReceivedRing();
            returnMessage = device.setTime;
            returnMessage = GetResponsMessage(Position, "output", paramItem, to_string(device.setTime).c_str());
        }
        else if(paramItem == paramWord_get[enm_all])
        {
            bzReceivedRing();
            String ringVal = "";
            if(device.ring) ringVal = "true";
            else ringVal = "false";
            returnMessage += "[\r";
            returnMessage += "" + GetResponsMessage_all(Position, "output", "interval", to_string(device.interval).c_str()) + ",\r";
            returnMessage += "" + GetResponsMessage_all(Position, "output", "ring", ringVal) + ",\r";
            returnMessage += "" + GetResponsMessage_all(Position, "output", "melody", device.melody) + ",\r";
            returnMessage += "" + GetResponsMessage_all(Position, "output", "next", to_string(getNextTime(device)).c_str()) + "\r";
            returnMessage += "]";
        }
        else{
            bzErrorSound();
            returnMessage = errorMessage[enmStsError_get];
        }
    }
    server.send(200, "text/plain", returnMessage);
}

String GetResponsMessage(String target, String action, String item, String value)
{
    String resultMessage = "";
    resultMessage += "{\r";
    resultMessage += "\t" + ResponsWord.Target + ":\"" + target + "\",\r";
    resultMessage += "\t" + ResponsWord.Action + ":\"" + action + "\",\r";
    resultMessage += "\t" + ResponsWord.Item + ":\"" + item + "\",\r";
    resultMessage += "\t" + ResponsWord.Value + ":\"" + value + "\"\r";
    resultMessage += "}";
    return resultMessage;
}

String GetResponsMessage_all(String target, String action, String item, String value)
{
    String resultMessage = "";
    resultMessage += "\t{\r";
    resultMessage += "\t\t" + ResponsWord.Target + ":\"" + target + "\",\r";
    resultMessage += "\t\t" + ResponsWord.Action + ":\"" + action + "\",\r";
    resultMessage += "\t\t" + ResponsWord.Item + ":\"" + item + "\",\r";
    resultMessage += "\t\t" + ResponsWord.Value + ":\"" + value + "\"\r";
    resultMessage += "\t}";
    return resultMessage;
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
//現在時刻取得
void outputTime()
{
    bzReceivedRing();
    struct tm nowTime = getTimeInf();
    char s[20] = {};
    arrangeTime(s, nowTime);
    server.send(200, "text/plain", s);
}