#define LED_BLUE 2
#define BOARD_SW 0

#define LED_ON HIGH
#define LED_OFF LOW

#define SW_ON LOW
#define SW_OFF HIGH

#include <Arduino.h>
#include <SPIFFS.h>
#include <Stream.h>
#include <Wire.h>

String wrfile = "/led_sts.txt";//読み書きするファイル名を設定


//***************************************************
void ledFlash()
{
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
  delay(250);
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
  delay(250);
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
  delay(250);
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
}


//***************************************************
int ReadLedSts()
{
  //SPIFFS.begin(true);	//SPIFFS開始
  File fr = SPIFFS.open(wrfile.c_str(), "r");//ファイルを読み込みモードで開く
  //String readStr = fr.readStringUntil('\n');//改行まで１行読み出し
  String readStr = fr.readString();
  fr.close();	//ファイルを閉じる
  //SPIFFS.end();
  delay(100);
  readStr.replace("\r\n", "");
  //Serial.println(readStr);
  if(readStr == "0")
    return 0;
  else
    return 1;
}


//***************************************************
void WriteLedSts(String strWrite)
{
  //SPIFFS.begin(true);	//SPIFFS開始
  File fw = SPIFFS.open(wrfile.c_str(), "w");//ファイルを書き込みモードで開く
  fw.println( strWrite );	//ファイルに書き込み
  fw.close();	//ファイルを閉じる
  delay(100);
  //SPIFFS.end();
}


//***************************************************
//***************************************************
void setup() {
  SPIFFS.begin(true);	//SPIFFS開始
  //WriteLedSts("0");
  Serial.begin(115200);
  Serial.println("Start");

  pinMode(LED_BLUE, OUTPUT);
  pinMode(BOARD_SW, INPUT_PULLUP);

  digitalWrite(2, HIGH);

  ledFlash();
}

//***************************************************
//***************************************************
void loop() {
  // put your main code here, to run repeatedly:
  int i = ReadLedSts();
  Serial.println(i);
  //Serial.println("loop");
  ledFlash();
}

