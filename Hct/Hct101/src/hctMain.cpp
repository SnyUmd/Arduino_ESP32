#include <Arduino.h>
// #include <string>
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
void modeSetting();

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
    switch(mode)
    {
        case enmNormal:
            action();
            break;
        case enmSetting:
            bzModeChange(1);
            modeSetting();
            bzModeChange(0);
            break;
    }
    
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
    //---------------------------------------

    sr.println(WiFi.localIP());
    sr.println("Setting mode");
    mode = enmSetting;

    // setTimerInterrupt(&onTimer, 200000, false);//300ms間割り込み停止
    // bzModeChange(1);
    // digitalWrite(PORT_LED_G, LED_ON);
    // digitalWrite(PORT_LED_R, LED_ON);
    // modeSetting();
    // delay(10000);
    // sr.println("Normal mode");
    // modeChange(0);
    // setTimerInterrupt(&onTimer, 5000000, false);//300ms間割り込み停止
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
    setTimerInterrupt(&onTimer, 1000000, false);//300ms間割り込み停止
}


//*************************************
void setDevice(int contentNum, String ledColor = "")
{
    String returnMessage = "";
    String paramSts = server.arg("sts");

    if(paramSts == paramWord_set[enm_on]){
        receivedRing();
        switch(contentNum)
        {
            case enm_led:
                if (ledColor == "g"){
                    returnMessage = "LED green on";
                    digitalWrite(PORT_LED_G, LED_ON);
                }
                else if(ledColor == "r"){
                    returnMessage = "LED red on";
                    digitalWrite(PORT_LED_R, LED_ON);
                }
                else {
                    sr.println("*****setDevice ledColor error*****");
                    returnMessage = "Program Error <setDevice ledColor error>";
                    prgBug();
                }
                break;
            case enm_motor:
                httpSts[enmMotor].sts = true;
                returnMessage = "Motor on";
                break;
            case enm_buzzer:
                httpSts[enmBuzzer].sts = true;
                returnMessage = "Sound buzzer";
                break;
            default:
                break;
        }
    }
    else if(paramSts == paramWord_set[enm_off]){
        receivedRing();
        switch(contentNum)
        {
            case enm_led:
                if (ledColor == "g"){
                    returnMessage = "LED green off";
                    digitalWrite(PORT_LED_G, LED_OFF);
                }
                else if(ledColor == "r"){
                    returnMessage = "LED red off";
                    digitalWrite(PORT_LED_R, LED_OFF);
                }
                else {
                    sr.println("*****setDevice ledColor error*****");
                    returnMessage = errorMessage[enmPrgError];
                    prgBug();
                }
                break;
            case enm_motor:
                httpSts[enmMotor].sts = false;
                returnMessage = "Motor off";
                break;
            case enm_buzzer:
                httpSts[enmBuzzer].sts = false;
                returnMessage = "Stop buzzer";
                break;
            default:
                break;
        }
    }
    else{
        errorSound();
        returnMessage = errorMessage[enmStsError_set];
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
    server.on(httpSts[enmLedR].uri, HTTP_ANY, [](){setDevice(enm_led, "r");});

    //緑LED　ON
    server.on(httpSts[enmLedG].uri, HTTP_ANY, [](){setDevice(enm_led, "g");});

    //モーター
    server.on(httpSts[enmMotor].uri, HTTP_ANY, [](){setDevice(enm_motor);});

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
void action()
{
    if(httpSts[enmMotor].sts) motorAction(1);
    if(httpSts[enmBuzzer].sts) bz(1);
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