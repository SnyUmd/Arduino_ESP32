#define LED_BLUE 2
#define BOARD_SW 0
#define EX_SW1 23

#define LED_ON HIGH
#define LED_OFF LOW

#define SW_ON LOW
#define SW_OFF HIGH

//#include <Wire.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <Stream.h>
//#include <EEPROM.h>

bool blLED = false;
String wrfile = "/led_sts.txt";//読み書きするファイル名を設定
String writeStr = "0";	//書き込み文字列を設定

//*******************************************************
void setup()
{
  //SPIFFS.begin(true);	//SPIFFS開始
  WriteLedSts("0");

  pinMode(LED_BLUE, OUTPUT);
  pinMode(BOARD_SW, INPUT_PULLUP);
  pinMode(EX_SW1, INPUT_PULLUP);

  xTaskCreatePinnedToCore(loop2, "loopTask", 4096, NULL, 1, NULL, 1);

  digitalWrite(LED_BLUE, LED_ON);
  delay(1000);

  if(ReadLedSts() == 0)
  {
    blLED = false;
    digitalWrite(LED_BLUE, LED_OFF);
  }
  else
  {
    blLED = true;
    digitalWrite(LED_BLUE, LED_ON);
  }

  ledFlash();

}



//*******************************************************
void loop() {
  //if(digitalRead(BOARD_SW) == SW_ON)
  if(blLED)
    digitalWrite(LED_BLUE, LED_ON);
  else
    digitalWrite(LED_BLUE, LED_OFF);
}

//*******************************************************
void loop2(void *pvParameters)
{
  int ledSts;
  while(1)
  {
    
    if(digitalRead(BOARD_SW) == SW_ON)
    {
      ledSts = ReadLedSts();
      if(ledSts == 0)
      {
        WriteLedSts("1");
        blLED = true;
        delay(500);
      }
      else
      {
        WriteLedSts("0");
        blLED = false;
        delay(500);
      }
      ledFlash();
    }
    
  }
}

//***************************************************
int ReadLedSts()
{
  SPIFFS.begin(true);	//SPIFFS開始
  File fr = SPIFFS.open(wrfile.c_str(), "r");//ファイルを読み込みモードで開く
  //String readStr = fr.readStringUntil('\n');//改行まで１行読み出し
  String readStr = fr.readString();
  fr.close();	//ファイルを閉じる
  SPIFFS.end();
  if(readStr == "0")
    return 0;
  else
    return 1;
}


//***************************************************
void WriteLedSts(String strWrite)
{
  SPIFFS.begin(true);	//SPIFFS開始
  File fw = SPIFFS.open(wrfile.c_str(), "w");//ファイルを書き込みモードで開く
  fw.println( strWrite );	//ファイルに書き込み
  fw.close();	//ファイルを閉じる
  SPIFFS.end();
}


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