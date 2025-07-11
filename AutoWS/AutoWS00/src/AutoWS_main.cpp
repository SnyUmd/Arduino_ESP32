#include <WiFi.h>
#include <Espalexa.h>

#include "AutoWS_main.hpp"
#include "modules/def.hpp"

Espalexa espalexa;

//**********************************************************************
void initPort()
{
  //LED-----------------------
  pinMode(PORT_LED_INTERNAL, OUTPUT);
  pinMode(PORT_LED_GREEN, OUTPUT);
  pinMode(PORT_LED_ORANGE, OUTPUT);
  digitalWrite(PORT_LED_INTERNAL, LED_OFF);
  digitalWrite(PORT_LED_GREEN, LED_OFF_EX);
  digitalWrite(PORT_LED_ORANGE, LED_OFF_EX);

  //SW
  pinMode(PORT_SW_INTERNAL, INPUT);

  //ソレノイド
  pinMode(PORT_WATER_OPEN, OUTPUT);
  pinMode(PORT_WATER_CLOSE, OUTPUT);
  digitalWrite(PORT_WATER_OPEN, 0);
  digitalWrite(PORT_WATER_CLOSE, 0);

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




/*Alexa操作　無効
  ledcSetup(PWM_CH , PWM_FRQ , PWM_BIT_NUM) ; //チャンネル , 周波数 , bit数(分解能)
  ledcAttachPin(PORT_LED_INTERNAL , PWM_CH) ; //端子番号 , チャンネル
  ledcWrite(PWM_CH, 0); //チャンネル , Duty比
*/
}

//**********************************************************************
void WifiSetup()
{
  WiFi.mode(WIFI_STA);
  Serial.printf("connecting to %s¥n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
}

//**********************************************************************
//our callback functions
void firstLightChanged(uint8_t brightness) {

    //do what you need to do here
    // showLED( brightness );
    Serial.print("Light Change : ");
    Serial.println(brightness);

    ledcWrite(PWM_CH, brightness);
}

//**********************************************************************
void LedFlash(int flash_num, int interval, int led_port){
/* PWM点灯　無効
  digitalWrite(led_port, LOW);
  for(int i = 0; i < flash_num; i++){
    delay(interval / 2);
    ledcWrite(PWM_CH, 128);
    delay(interval / 2);
    ledcWrite(PWM_CH, 0);
  }
*/
}

//**********************************************************************
void LedFlashEX(int flash_num, int interval, int led_port_ex){
  digitalWrite(led_port_ex, LED_OFF_EX);
  for(int i = 0; i < flash_num; i++){
    digitalWrite(led_port_ex, LED_ON_EX);
    delay(interval);
    digitalWrite(led_port_ex, LED_OFF_EX);
    delay(interval);
  }
}



//**********************************************************************
//**********************************************************************
void setup() {
    Serial.begin(115200);
    initPort();
    LedFlash(2, 500, PORT_LED_INTERNAL);

/*Alexa操作　無効
    // WifiSetup();

    // espalexa.addDevice("ESP_LED", firstLightChanged,0); //simplest definition, default state off
    // espalexa.begin();

    // LedFlash(5, 200, LED_INTERNAL);
*/
  LedFlashEX(2, 200, PORT_LED_GREEN);
  delay(200);
  LedFlashEX(2, 200, PORT_LED_ORANGE);
  delay(1000);

}

//**********************************************************************
//**********************************************************************
void loop() {
/*Alexa操作　無効
  //  espalexa.loop();
  //  delay(1);
*/

/*
  if(!digitalRead(PORT_SW_INTERNAL)){
    digitalWrite(PORT_LED_INTERNAL, LED_ON);
    waterSts = !waterSts;
    WaterSet(waterSts);
    delay(250);
    digitalWrite(PORT_LED_INTERNAL, LED_OFF);
  }
*/
  while(true){
    if(!digitalRead(PORT_SW_INTERNAL)){
      delay(250);
      break;
    }
    delay(5);
  }

  while(true){
    waterSts = !waterSts;
    ClsWaterCtrl.WaterSet(waterSts);
    delay(4000);
  }

}