#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
// #include <vector>
#include <iostream>
#include <vector>


#include "module/common.h"
#include "module/defKeyChecker.h"
#include "module/Initialize.h"
#include "module/Interrupt.h"
#include "module/wifiCtrl.h"
#include "module/BzCtrl.h"
#include "module/i2cCtrl.h"
#include "module/apds9930.h"
#include "module/Timer.h"

// put function declarations here:
//----------------------------
void swInterrupt_fn();
void setHttpAction();
void httpAction(int, String);

void ringSet(int);
void taskBuzzer(void* arg);
void taskLighting(void* arg);
bool checkBz();
void ringSet(int);

bool LightOff();
void start_light_sleep();

//----------------------------
// ledCtrl LC;
hw_timer_t* test_timer_t;


//**********************************************************
//**********************************************************
//**********************************************************
void setup() {
  // put your setup code here, to run once:
    sr.begin(115200);
    sr.println("-----start-----");

    initPort();
    InitBz();
    initI2C(wr);
    ringSet(enm_power_on);
    attachInterrupt(P_SW_INTERNAL, swInterrupt_fn, FALLING);
    sr.println("SW Interrupt set");

    //ブザー鳴動タスクをセット
    xTaskCreatePinnedToCore(taskBuzzer, "taskBuzzer", 4096, NULL, 2, NULL, 1);
    sr.println("Buzzer task set");
    xTaskCreatePinnedToCore(taskLighting, "taskLighting", 4096, NULL, 1, NULL, 1);
    sr.println("Timer task set");

    //Wi-Fi接続
    if(!wifiInit(WiFi, sr, SSID, PASS, HOST_NAME, false))
    {
        //接続失敗でLED点滅
        ringSet(enm_error);
        while(1)
        {
            digitalWrite(P_LED_INTERNAL, !digitalRead(P_LED_INTERNAL));
            delay(200);
        }
    }
    setHttpAction();

    LC.ledFlash(P_LED_INTERNAL, 5, 5);
    digitalWrite(P_LED_INTERNAL, LED_OFF);
    sr.println("setup successed");
    sr.println("");

    ringSet(enm_stanby_ok);//準備完了音を鳴らす
}

//**********************************************************
void loop() {
    server.handleClient();

    // sr.println(readTestApds9930(wr, sr));
    // delay(500);
    // int a = 1;
}

//**********************************************************
//タイマーセットタスク
void taskLighting(void* arg)
{
    long startTime;
    while(1)
    {
        if(flgLighting)
        {
            digitalWrite(P_LED_INTERNAL, LED_ON);
            sr.println("Light ON");
            startTime = GetTime();
            sr.println("set start time");
            while(flgLighting)
            {
                if(CheckElapsedTime(startTime, 10000)) LightOff();
                // delay(1);
            }
        }
        delay(1);
    }
}

//**********************************************************
//ブザー鳴動タスク
void taskBuzzer(void* arg)
{
    while(1)
    {
        // if(flgBz)
        while(checkBz())
        {
        for(int i = 0; i < sizeof(bzSts)/sizeof(structBz); i++)
        {
            if(bzSts[i].blBz) 
            {
                bzSts[i].ring();
                bzSts[i].blBz = false;
            }
        }
        }
        // flgBz = false;
        delay(1);//待たなければリセットしてしまう。
    }
}

//**********************************************************
//鳴らすブザー音をセット
void ringSet(int bz_num)
{
    bzSts[bz_num].blBz = true;
    // flgBz = true;
}

//**********************************************************
bool checkBz()
{
    for(int i = 0; i < sizeof(bzSts)/sizeof(structBz); i++) 
    {
      if(bzSts[i].blBz) return true;
    }
    return false;
}

//**********************************************************
bool setFlgLighting()
{
    sr.println("-----function setFlgLighting----");
    if(!flgLighting)
    {
        flgLighting = true;
        ringSet(enm_interrupt);
        sr.println("flag lighting set");
        return true;
    }
    else
    {
        // sr.println("点灯中");
        return false;
    } 
}

//**********************************************************
// void IRAM_ATTR LightOff()
bool LightOff()
{
    if(flgLighting)
    {
        digitalWrite(P_LED_INTERNAL, LED_OFF);
        sr.println("Light OFF");
        ringSet(enm_of);
        flgLighting = false;
        sr.println("reset flag lighting");
        return true;
    }
    else return false;
}


//**********************************************************
/// @brief SW割り込み処理
void swInterrupt_fn()
{
    if(digitalRead(P_SW_INTERNAL) == !SW_OFF)
    {
        // チャタリング防止の為、割り込み停止
        attachInterrupt(P_SW_INTERNAL, NULL, FALLING);

        //割り込み処理--------------------------
        sr.println("get intterrupt");

        if(!setFlgLighting()) sr.println("点灯中");
        // ------------------------------------

        //割り込みセット
        attachInterrupt(P_SW_INTERNAL, swInterrupt_fn, FALLING);
    }
}

//**********************************************************
void setHttpAction()
{
    //バルブオープン
    server.on("/test", HTTP_ANY, [](){httpAction(0,"sts");});//http://keychecker.airport/test?sts=??
    server.on("/set", HTTP_ANY, [](){httpAction(1,"sts");});//http://keychecker_000.airport/set?sts=??
    //セット
    // server.on(httpContents[enmSet], HTTP_ANY, [](){httpAction(enmSet);});


    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        server.send(404, "text/plain", "not found"); // 404を返す
    });

    server.begin();
}

//**********************************************************
void httpAction(int pattern, String param)
{
    String returnMessage = "";
    String param_val = server.arg(param);
    switch (pattern)
    {
        case 0:
            if(param_val == "ok")
                returnMessage = "OK";
            else if(param_val == "ng")
                returnMessage = "NG";
            else
                returnMessage = "-----";
            break;
        case 1:
            if(param_val == "on")
            {
                if(!setFlgLighting())
                {
                    sr.println("点灯中");
                    returnMessage = "error [While the lights are on]";
                } 
                else returnMessage = "Light turned on successfully";
            }
            else if(param_val == "off")
            {
                if(LightOff())
                    returnMessage = "Light turned off successflly";
                else
                {
                    sr.println("既に消灯中");
                    returnMessage = "error  [While the lights are off]";
                }
            }
            break;
        
        default:
        break;
    }
    server.send(200, "text/plain", returnMessage);

}

//**********************************************************
