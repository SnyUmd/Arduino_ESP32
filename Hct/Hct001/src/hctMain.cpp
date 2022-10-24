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

//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
void setup()
{
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
    //赤LED　ON
    server.on(httpSts[enmLedOnR].uri, HTTP_ANY, [](){
        // if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
        //     target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
        // }
        receivedRing();
        digitalWrite(PORT_LED_R, 0);
        server.send(200, "text/plain", httpSts[enmLedOnR].returnMess);
    });

    //赤LED　OFF
    server.on(httpSts[enmLedOffR].uri, HTTP_ANY, [](){
        receivedRing();
        digitalWrite(PORT_LED_R, 1);
        server.send(200, "text/plain", httpSts[enmLedOffR].returnMess);
    });

    //緑LED　ON
    server.on(httpSts[enmLedOnG].uri, HTTP_ANY, [](){
        receivedRing();
        digitalWrite(PORT_LED_G, 0);
        server.send(200, "text/plain", httpSts[enmLedOnG].returnMess);
    });

    //緑LED　OFF
    server.on(httpSts[enmLedOffG].uri, HTTP_ANY, [](){
        receivedRing();
        digitalWrite(PORT_LED_G, 1);
        server.send(200, "text/plain", httpSts[enmLedOffG].returnMess);
    });

    //時刻取得
    server.on(httpSts[enmGetTime].uri, HTTP_ANY, [](){
        receivedRing();
        struct tm nowTime = getTimeInf();
        char s[20] = {};
        arrangeTime(s, nowTime);
        // server.send(200, "text/plain", httpSts[enmGetTime].returnMess);
        server.send(200, "text/plain", s);
    });

    //モーター駆動開始
    server.on(httpSts[enmMotorStart].uri, HTTP_ANY, [](){
        receivedRing();
        httpSts[enmMotorStart].sts = true;
        server.send(200, "text/plain", httpSts[enmMotorStart].returnMess);
    });

    //モーター駆動停止
    server.on(httpSts[enmMotorStop].uri, HTTP_ANY, [](){
        receivedRing();
        httpSts[enmMotorStart].sts = false;
        server.send(200, "text/plain", httpSts[enmMotorStop].returnMess);
    });

    //ブザー駆動開始
    server.on(httpSts[enmBuzzerRing].uri, HTTP_ANY, [](){
        receivedRing();
        httpSts[enmBuzzerRing].sts = true;
        server.send(200, "text/plain", httpSts[enmBuzzerRing].returnMess);
    });

    //ブザー駆動停止
    server.on(httpSts[enmBuzzerStop].uri, HTTP_ANY, [](){
        receivedRing();
        httpSts[enmBuzzerRing].sts = false;
        server.send(200, "text/plain", httpSts[enmBuzzerStop].returnMess);
    });

    //温度取得
    server.on(httpSts[enmGetTemperture].uri, HTTP_ANY, [](){
        receivedRing();
        string sTemp = getTemp();
        server.send(200, "text/plain", sTemp.c_str());
    });

    //湿度取得
    server.on(httpSts[enmGetHumidity].uri, HTTP_ANY, [](){
        receivedRing();
        string sHumd = getHumd();
        server.send(200, "text/plain", sHumd.c_str());
    });

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        server.send(404, "text/plain", "Not Found."); // 404を返す
    });

    server.begin();
}

//*************************************
void action()
{
    if(httpSts[enmMotorStart].sts) motorAction(1);
    if(httpSts[enmBuzzerRing].sts) bz(1);
    
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