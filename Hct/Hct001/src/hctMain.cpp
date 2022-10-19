#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include "def.h"
#include "Timer.h"
#include "wifiCtrl.h"
#include "ledCtrl.h"
#include "rx8035.h"

ledCtrl LC;
HardwareSerial sr = Serial;
// HardwareSerial sr = Serial1;

// hw_timer_t * timer = NULL;
struct tm timeInf;
struct tm timeInfRTC;
char s[20];//文字格納用

void init();
void IRAM_ATTR onTimer();
void funcInterrupt();

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



//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
void setup()
{
    init();
    wifiInit(WiFi, sr, SSID, PASS, HOST_NAME);
    sr.println("-----Start-----");
    timeInf = getTimeInf();
    arrangeTime(s, timeInf);
    sr.println(s);
}

//*************************************
void loop()
{
    
    // byte val[] = readI2C(ADR_RTC, 0, 7);
    byte val[8] = {};
    int loopNum = sizeof(val)/sizeof(val[0]);
    readI2C(val, Wire, ADR_RTC, 0, 7);
    for(int i = 1; i <= loopNum; i++) sr.print(val[loopNum - i]);
    sr.println("");
    // timeInfRTC.tm_sec = val;
    // sr.println(timeInfRTC.tm_sec);
    delay(1000);
}



//*************************************


