#include <Arduino.h>
#include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "defHct.h"
#include "common.h"
#include "module/Timer.h"
#include "module/wifiCtrl.h"
#include "module/ledCtrl.h"
#include "module/rx8035.h"
#include "module/BzCtrl.h"

ledCtrl LC;

void init();
void IRAM_ATTR onTimer();
void funcInterrupt();
void motorAction(int setNum);
void action();

//*************************************
void init()
{
    sr.begin(115200);
    Wire.begin();
    
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
    sr.println(GetTime());
    attachInterrupt(PORT_SW, funcInterrupt, FALLING);
}

//*************************************
//ポート割り込み処理
//*************************************
void funcInterrupt()
{
    //チャタリング対策------------------------
    attachInterrupt(PORT_SW, NULL, FALLING);
    setTimerInterrupt(&onTimer, 300000, false);//300ms間割り込み停止
    //---------------------------------------

    // int portLogic = !digitalRead(PORT_MOTOR0);
    // digitalWrite(PORT_MOTOR0, portLogic);
    // digitalWrite(PORT_MOTOR1, portLogic);
    // digitalWrite(PORT_MOTOR2, portLogic);
    // digitalWrite(PORT_MOTOR3, portLogic);

    // digitalWrite(PORT_LED_G, portLogic);
    // digitalWrite(PORT_LED_R, portLogic);

    // sr.println(GetTime());
    byte val;
    // Wire.beginTransmission(ADR_RTC);
    // Wire.read(0);
    // error = Wire.endTransmission();
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
    //赤LED　フラッシュ
    server.on(httpSts[enmLedFlashR].uri, HTTP_ANY, [](){
        // if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
        //     target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
        // }
        receivedRing();
        httpSts[enmLedFlashR].sts = true;
        server.send(200, "text/plain", httpSts[enmLedFlashR].returnMess);
    });

    //緑LED　フラッシュ
    server.on(httpSts[enmLedFlashG].uri, HTTP_ANY, [](){
        receivedRing();
        httpSts[enmLedFlashG].sts = true;
        server.send(200, "text/plain", httpSts[enmLedFlashG].returnMess);
    });

    //時刻取得
    server.on(httpSts[enmGetTime].uri, HTTP_ANY, [](){
        receivedRing();
        httpSts[enmGetTime].sts = true;
        server.send(200, "text/plain", httpSts[enmGetTime].returnMess);
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

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        server.send(404, "text/plain", "Not Found."); // 404を返す
    });

    server.begin();
}

//*************************************
void action()
{
    if(httpSts[enmLedFlashR].sts)
        LC.ledFlash(PORT_LED_R, 5, 1);

    if(httpSts[enmLedFlashG].sts)
        LC.ledFlash(PORT_LED_G, 5, 1);

    if(httpSts[enmGetTime].sts);

    if(httpSts[enmMotorStart].sts) motorAction(1);
    if(httpSts[enmBuzzerRing].sts) bz(1);
}

//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
void setup()
{
    sr.println("");
    init();
    // sr.println("");
    delay(1000);
    wifiInit(WiFi, sr, SSID, PASS, HOST_NAME);
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
    // byte val[8] = {};
    // int loopNum = sizeof(val)/sizeof(val[0]);

    // readI2C(val, Wire, ADR_RTC, 0, 7);
    // for(int i = 1; i <= loopNum; i++) sr.print(val[loopNum - i]);
    // sr.println("");
    
    // timeInfRTC.tm_sec = val;
    // sr.println(timeInfRTC.tm_sec);
    
    // motorAction(2);
    // delay(1000);
    action();
}



//*************************************


