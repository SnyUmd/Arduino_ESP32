// #define LED_RED 4
// #define LED_GRE 16
// #define SPOT_LIGHT 2
// #define HUMAN_SENSOR 15
// #define SWITCH0 0

#include <Arduino.h>
//#include <Esp32.h>
#include <time.h>
#include "def.h"
#include "Init.h"
#include "SpotTimer.h"
#include "clsHumanSensor.h"
#include "PowerCtrl.h"
#include "TimeCtrl.h"


//クラス**********************
clsHumanSensor clsHS;
clsPowerCtrl clsPC;


//********************************************************
void setup() 
{

  //マルチタスク設定
  //xTaskCreatePinnedToCore(subloop, "subloop", 4096, NULL, 1, NULL, 1);
  //Initialize_Port();

  //ディープスリープからの立ち上がりなのかを判定
  if(!esp_sleep_get_wakeup_cause())
    blLightningFlg = false;
  else
  {
    LedFlash(100, 1, false);
    blLightningFlg = true;
    iWakeupCnt++;
  }
  digitalWrite(LED_RED, LOW);
  
}

//********************************************************
void loop() 
{
  //スリープからの復帰動作
  if(!blLightningFlg)
  {
    LedFlash(1000, 30, false);
    //continue;
  }
  //復帰時
  else
  {
    //復帰からの最初の人チェック
    if(clsHS.HumanCheck(100, 10, 50))
    {
      digitalWrite(LED_GRE, LOW);
      digitalWrite(SPOT_LIGHT, HIGH);
      delay(iLightingTime);

      //二回目以降のチェックは少し緩めに
      if(!clsHS.HumanCheck(100, 10, 40)) blLightningFlg = false;
    }
    else
    {
      digitalWrite(SPOT_LIGHT, LOW);
      clsPC.DeepSleep_WakeupPort(GPIO_NUM_15, true);
    }
  }

  clsPC.DeepSleep_WakeupPort(GPIO_NUM_15, true);

}

//****************************************************************************************************************
//****************************************************************************************************************
//****************************************************************************************************************

void LedFlash(int iPeriod, int iLightNum, bool blLighsUp)
{
  int iLoop = iLightNum*2;
  for(int i = 0; i < iLoop; i++)
  {
    digitalWrite(LED_RED, !digitalRead(LED_RED));
    digitalWrite(LED_GRE, !digitalRead(LED_GRE));
    delay(iPeriod);
  }
  if(blLighsUp)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GRE, LOW);
  }
  else
  {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GRE, HIGH);
  }
}

//********************************************************
// void Init_Port()
// {
//   //LED
//   pinMode(LED_RED, OUTPUT);
//   pinMode(LED_GRE, OUTPUT);

//   digitalWrite(LED_RED, HIGH);
//   digitalWrite(LED_GRE, HIGH);

//   //スポットライト
//   pinMode(SPOT_LIGHT, OUTPUT);
//   digitalWrite(SPOT_LIGHT, LOW);

//   //人感センサ
//   pinMode(HUMAN_SENSOR, INPUT);
//   //pinMode(15, INPUT);

//   //タクトスイッチ
//   pinMode(SWITCH0, INPUT_PULLUP);

// }

//********************************************************
// void DeepSleep_WakeupTime(int iWakeupTime)
// //iWakeupTime：タイマーの時間はuSオーダー
// {
//   esp_sleep_enable_timer_wakeup(iWakeupTime);
//   esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
//   esp_deep_sleep_start();
// }
