#define LED_BLUE 2
#define BOARD_SW 0

#define LED_ON HIGH
#define LED_OFF LOW

#define SW_ON LOW
#define SW_OFF HIGH

//#include <Arduino.h>
String wrfile = "/led_sts.txt";//読み書きするファイル名を設定

//*******************************************************
void setup()
{
    Serial.begin(115200);
    pinMode(LED_BLUE, OUUT);

    digitalWrite(LED_BLUE, LED_ON);
}

//*******************************************************
void loop() 
{
}
