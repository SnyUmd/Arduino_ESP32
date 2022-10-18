#include <WiFi.h>
#include "def.h"
#include "Timer.h"
#include "wifiCtrl.h"

// hw_timer_t * timer = NULL;
struct tm timeInf;
char s[20];//文字格納用

HardwareSerial sr = Serial;
// HardwareSerial sr = Serial1;

//*************************************
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
    digitalWrite(PORT_LED_G, !digitalRead(PORT_LED_G));
    timeInf = getTimeInf();
    arrangeTime(s, timeInf);
    sr.println(s);
    delay(1000);
}

//*************************************
void init()
{
    sr.begin(115200);
    
    //ポート割り込み処理
    pinMode(PORT_SW, INPUT_PULLUP);
    attachInterrupt(PORT_SW, funcInterrupt, FALLING);

    //LEDポート
    pinMode(PORT_LED_G, OUTPUT);
    pinMode(PORT_LED_R, OUTPUT);
    digitalWrite(PORT_LED_G, 0);
    digitalWrite(PORT_LED_R, 0);

    //ステッピングモータ　ポート
    pinMode(PORT_MOTOR0, OUTPUT);
    pinMode(PORT_MOTOR1, OUTPUT);
    pinMode(PORT_MOTOR2, OUTPUT);
    pinMode(PORT_MOTOR3, OUTPUT);
    digitalWrite(PORT_MOTOR0, 0);
    digitalWrite(PORT_MOTOR1, 0);
    digitalWrite(PORT_MOTOR2, 0);
    digitalWrite(PORT_MOTOR3, 0);
}

//*************************************


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

    int portLogic = !digitalRead(PORT_MOTOR0);
    digitalWrite(PORT_MOTOR0, portLogic);
    digitalWrite(PORT_MOTOR1, portLogic);
    digitalWrite(PORT_MOTOR2, portLogic);
    digitalWrite(PORT_MOTOR3, portLogic);

    digitalWrite(PORT_LED_G, portLogic);
    digitalWrite(PORT_LED_R, portLogic);

    sr.println(GetTime());
}