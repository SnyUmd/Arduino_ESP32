#include "clsCommon.hpp"
#include "modules/def.hpp"

//**************************************************************************
void ClsCommon::initPort()
{
  //LED-----------------------
  pinMode(PORT_LED_INTERNAL, OUTPUT);
  pinMode(PORT_LED_OPEN, OUTPUT);
  pinMode(PORT_LED_CLOSE, OUTPUT);
  digitalWrite(PORT_LED_INTERNAL, LED_OFF);
  digitalWrite(PORT_LED_OPEN, LED_OFF_EX);
  digitalWrite(PORT_LED_CLOSE, LED_OFF_EX);

  //SW
  pinMode(PORT_SW_INTERNAL, INPUT);

  //ソレノイド
  pinMode(PORT_WATER_OPEN, OUTPUT);
  pinMode(PORT_WATER_CLOSE, OUTPUT);
  digitalWrite(PORT_WATER_OPEN, 0);
  digitalWrite(PORT_WATER_CLOSE, 0);

  //流水状態
  pinMode(PORT_RUN_WATER, INPUT);

  //モーター
  pinMode(PORT_MORTER_0, OUTPUT);
  pinMode(PORT_MORTER_1, OUTPUT);
  pinMode(PORT_MORTER_2, OUTPUT);
  pinMode(PORT_MORTER_3, OUTPUT);
  digitalWrite(PORT_MORTER_0, 0);
  digitalWrite(PORT_MORTER_1, 0);
  digitalWrite(PORT_MORTER_2, 0);
  digitalWrite(PORT_MORTER_3, 0);

  pinMode(34, INPUT);
  pinMode(35, INPUT);

}

//**************************************************************************
void ClsCommon::ledOkOrNg(bool is_ok, int port_led)
{
    int loopNum = 0;
    int waitTime = 0;

    if(is_ok)
    {
        loopNum = 3;
        waitTime = 100;
    }
    else
    {
        loopNum = 10;
        waitTime = 25;
    }

    for(int i = 0; i < loopNum; i++)
    {
        digitalWrite(port_led, !digitalRead(port_led));
        delay(waitTime);
        digitalWrite(port_led, !digitalRead(port_led));
        delay(waitTime);
    }
}

//**************************************************************************
void ClsCommon::LedFlashEX(int flash_num, int interval, int led_port_ex){
  digitalWrite(led_port_ex, LED_OFF_EX);
  for(int i = 0; i < flash_num; i++){
    digitalWrite(led_port_ex, LED_ON_EX);
    delay(interval);
    digitalWrite(led_port_ex, LED_OFF_EX);
    delay(interval);
  }
}


//**************************************************************************
void ClsCommon::setWaterLED(){
    int cnt = 0;
    cnt = clsWaterCtrl.CheckRunningWater();
    Serial.println(cnt);
    if(cnt > 1) digitalWrite(PORT_LED_INTERNAL, LED_ON);
    else digitalWrite(PORT_LED_INTERNAL, LED_OFF);
}

//**************************************************************************
//**************************************************************************
