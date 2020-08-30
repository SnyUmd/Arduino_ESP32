#define LED_RED 4
#define LED_GRE 16
#define SPOT_LIGHT 2
#define HUMAN_SENSOR 15
#define SWITCH0 0

#include <Arduino.h>
//#include <Esp32.h>
#include <time.h>

bool blLightningFlg = false;
//********************************************************
void setup() 
{

  //マルチタスク設定
  //xTaskCreatePinnedToCore(subloop, "subloop", 4096, NULL, 1, NULL, 1);
    Init_Port();

  //ディープスリープからの立ち上がりなのかを判定
  if(!esp_sleep_get_wakeup_cause())
  {
    LedFlash(100, 5, false);
    blLightningFlg = false;
  }
  else
  {
    LedFlash(100, 1, false);
    blLightningFlg = true;
  }

}

//********************************************************
void loop() 
{
  if(blLightningFlg)
  {
    digitalWrite(LED_GRE, LOW);
    digitalWrite(SPOT_LIGHT, HIGH);
    delay(10000);
  }

  digitalWrite(LED_GRE, HIGH);
  digitalWrite(SPOT_LIGHT, LOW);
  //blLightningFlg = false;
  deepSleep_Port(GPIO_NUM_15, true);
}

//********************************************************

// void subloop(void* pvParameters)
// {
//   while(1)
//   {

//   }
// }

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
void Init_Port()
{
  //LED
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GRE, OUTPUT);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GRE, HIGH);

  //スポットライト
  pinMode(SPOT_LIGHT, OUTPUT);
  digitalWrite(SPOT_LIGHT, LOW);

  //人感センサ
  pinMode(HUMAN_SENSOR, INPUT);
  //pinMode(15, INPUT);

  //タクトスイッチ
  pinMode(SWITCH0, INPUT_PULLUP);

}

//********************************************************
void deepSleep_Timer(int iWakeupTime)
//iWakeupTime：タイマーの時間はuSオーダー
{
  esp_sleep_enable_timer_wakeup(iWakeupTime);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
  esp_deep_sleep_start();

}

void deepSleep_Port(gpio_num_t portNum, bool blHigh)
//portNum:GPIO_NUM_*
//ex)　deepSleep_Port(GPIO_NUM_0);
{
  int iHighLow = 0;
  if(blHigh)
    iHighLow = 1;
  esp_sleep_enable_ext0_wakeup(portNum, iHighLow);
  esp_deep_sleep_start();
}

//********************************************************
//********************************************************
