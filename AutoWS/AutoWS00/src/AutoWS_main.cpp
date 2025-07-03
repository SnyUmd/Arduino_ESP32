#include <WiFi.h>
#include <Espalexa.h>

#include "AutoWS_main.hpp"

Espalexa espalexa;

//**********************************************************************
void initPort()
{
  pinMode(LED_INTERNAL, OUTPUT);
  pinMode(SW_INTERNAL, INPUT);

  ledcSetup(PWM_CH , PWM_FRQ , PWM_BIT_NUM) ; //チャンネル , 周波数 , bit数(分解能)
  ledcAttachPin(LED_INTERNAL , PWM_CH) ; //端子番号 , チャンネル
  ledcWrite(PWM_CH, 0); //チャンネル , Duty比
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
void LedFlash(int flash_num, int interval){
  digitalWrite(LED_INTERNAL, LOW);
  for(int i = 0; i < flash_num; i++){
    delay(interval / 2);
    ledcWrite(PWM_CH, 128);
    delay(interval / 2);
    ledcWrite(PWM_CH, 0);
  }
}

//**********************************************************************
//**********************************************************************
void setup() {
    Serial.begin(115200);
    initPort();
    LedFlash(2, 500);
    WifiSetup();

    espalexa.addDevice("ESP_LED", firstLightChanged,0); //simplest definition, default state off
    espalexa.begin();

    LedFlash(5, 200);
}

//**********************************************************************
//**********************************************************************
void loop() {
   espalexa.loop();
   delay(1);
}