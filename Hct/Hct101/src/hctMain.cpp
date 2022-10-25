#include <Arduino.h>
#include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <iostream>
#include "defHct.h"
#include "common.h"
#include "module/Timer.h"
#include "module/wifiCtrl.h"
#include "module/ledCtrl.h"
#include "module/rx8035.h"
#include "module/BzCtrl.h"
#include "module/htu21d.h"
#include "module/i2cCtrl.h"

using namespace std;

ledCtrl LC;

void init();
void IRAM_ATTR onTimer();
void funcInterrupt();
void motorAction(int setNum);
void action();
void setHttpAction();
string getTemp();
string getHumd();
void outputTime();

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
    wifiInit(WiFi, sr, SSID, PASS, HOST_NAME, true);
    // timeInf = getTimeInf();
    // arrangeTime(s, timeInf);
    // sr.println(s);
    setHttpAction();
    sr.println("-----loop Start-----");
    BzGoUp(10, 10);
}

//*************************************
void loop()
{
    server.handleClient();

    //RTC時刻取得-------------------
    // byte val[8] = {};
    // int loopNum = sizeof(val)/sizeof(val[0]);
    // getTimeRX8035(val, wr);
    // for(int i = 1; i <= loopNum; i++) sr.print(val[loopNum - i]);
    // sr.println("");
    //------------------------------
    
    // timeInfRTC.tm_sec = val;
    // sr.println(timeInfRTC.tm_sec);

    //温度取得------------------------------
    // byte val[3] = {};
    // sr.println(readTempHTD21D(val, wr, sr));
    // sr.println(readHumdHTD21D(val, wr, sr));
    // for(int i = 1; i <= loopNum; i++) sr.print(val[loopNum - i]);
    // sr.println("");
    //------------------------------

    // motorAction(2);
    // delay(1000);
    
    action();
    delay(1000);
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
    setTimerInterrupt(&onTimer, 200000, false);//300ms間割り込み停止
    //---------------------------------------
    sr.println(WiFi.localIP());
}

//*************************************
void setLED(int portNum)
{
    int ledStatus = 1;
    String returnMessage = "";

    String paramSts = server.arg("sts");

    if(paramSts == paramLED[led_on]){
        receivedRing();
        ledStatus = 0;
        returnMessage = "LED red on";
    }
    else if(paramSts == paramLED[led_off]){
        receivedRing();
        returnMessage = "LED red off";
    }
    else{
        errorSound();
        returnMessage = "LED red parameter error --- '?sts=on' or '?sts=off'";
    }

    digitalWrite(portNum, ledStatus);
    server.send(200, "text/plain", returnMessage);
}

//*************************************
void setMotor()
{
    String paramSts = server.arg("sts");
    String returnMEssage = "";

    if(paramSts == "start"){
        receivedRing();
        httpSts[enmMotor].sts = true;
        returnMEssage = "Motor on";
    }
    else if(paramSts == "stop"){
        receivedRing();
        httpSts[enmMotor].sts = false;
        returnMEssage = "Motor off";
    }
    else{
        errorSound();
        httpSts[enmMotor].sts = false;
        returnMEssage = "Motor parameter error --- '?sts=start' or '?sts=stop'";
    }

    server.send(200, "text/plain", returnMEssage);
}

//*************************************
void setBuzzer()
{
    String paramSts = server.arg("sts");
    String returnMEssage = "";

    if(paramSts == "start"){
        receivedRing();
        httpSts[enmBuzzer].sts = true;
        returnMEssage = "Sound buzzer";
    }
    else if(paramSts == "stop"){
        receivedRing();
        httpSts[enmBuzzer].sts = false;
        returnMEssage = "Stop Buzzer";
    }
    else{
        errorSound();
        httpSts[enmBuzzer].sts = false;
        returnMEssage = "Buzzer parameter error --- '?sts=start' or '?sts=stop'";
    }

    server.send(200, "text/plain", returnMEssage);
}

//*************************************
void outputTime()
{
    receivedRing();
    struct tm nowTime = getTimeInf();
    char s[20] = {};
    arrangeTime(s, nowTime);
    // server.send(200, "text/plain", httpSts[enmGetTime].returnMess);
    server.send(200, "text/plain", s);
}


//*************************************
void motorAction(int setNum)
{
    const int loop = 4;
    bool blLoop = true;
    int num;
    int cnt = 0;
    int repetitionNum = 0;
    // LC.ledFlash(PORT_LED_G, 10, 5);

    while(blLoop){
        for(int i = 0; i < loop; i++)
        {
            if(cnt < 50) num = i;
            else num = loop - i - 1;

            digitalWrite(PORT_MOTOR0, aryMotorSts[num][0]);
            delay(1);
            digitalWrite(PORT_MOTOR1, aryMotorSts[num][1]);
            delay(1);
            digitalWrite(PORT_MOTOR2, aryMotorSts[num][2]);
            delay(1);
            digitalWrite(PORT_MOTOR3, aryMotorSts[num][3]);
            delay(1);
        }
        cnt++;
        if(cnt > 100) 
        {
            cnt = 0;
            repetitionNum++;
        }
        if(repetitionNum >= setNum) blLoop = false;
    }
}

//*************************************
void setHttpAction()
{
    //赤LED
    server.on(httpSts[enmLedR].uri, HTTP_ANY, [](){setLED(PORT_LED_R);});

    //緑LED　ON
    server.on(httpSts[enmLedG].uri, HTTP_ANY, [](){setLED(PORT_LED_G);});

    //時刻取得
    // server.on(httpSts[enmGetTime].uri, HTTP_ANY, [](){
    // server.on(httpSts[enmGetTime].uri, HTTP_ANY, outputTime);
    //     receivedRing();
    //     struct tm nowTime = getTimeInf();
    //     char s[20] = {};
    //     arrangeTime(s, nowTime);
    //     // server.send(200, "text/plain", httpSts[enmGetTime].returnMess);
    //     server.send(200, "text/plain", s);
    // });

    //モーター
    server.on(httpSts[enmMotor].uri, HTTP_ANY, setMotor);

    //ブザー駆動開始
    server.on(httpSts[enmBuzzer].uri, HTTP_ANY, setBuzzer);
    // server.on(httpSts[enmBuzzer].uri, HTTP_ANY, [](){setBuzzer
    //     receivedRing();
    //     httpSts[enmBuzzer].sts = true;
    //     server.send(200, "text/plain", httpSts[enmBuzzer].returnMess);
    // });

    // //ブザー駆動停止
    // server.on(httpSts[enmBuzzer].uri, HTTP_ANY, [](){
    //     receivedRing();
    //     httpSts[enmBuzzer].sts = false;
    //     server.send(200, "text/plain", httpSts[enmBuzzer].returnMess);
    // });

    //温度取得
    server.on(httpSts[enmGet].uri, HTTP_ANY, [](){
        receivedRing();
        string sTemp = getTemp();
        server.send(200, "text/plain", sTemp.c_str());
    });

    //湿度取得
    server.on(httpSts[enmGet].uri, HTTP_ANY, [](){
        receivedRing();
        string sHumd = getHumd();
        server.send(200, "text/plain", sHumd.c_str());
    });

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        errorSound();
        server.send(404, "text/plain", "Not Found."); // 404を返す
    });

    server.begin();
}

//*************************************
void action()
{
    if(httpSts[enmMotor].sts) motorAction(1);
    if(httpSts[enmBuzzer].sts) bz(1);
    
}

//*************************************
// float getTemp()
// {
//     return readTempHTD21D(wr);
// }

string getTemp()
{
    float fTemp = readTempHTD21D(wr);
    return to_string(fTemp).substr(0, 5);
}

string getHumd()
{
    float fHumd = readHumdHTD21D(wr);
    return to_string(fHumd).substr(0, 5);
}