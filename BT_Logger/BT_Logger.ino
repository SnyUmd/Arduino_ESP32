//#define SENSOR_SET
#define BLUETOOTH

//#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Esp.h>
//#include "cls_I2C.h"
//#include "Adafruit_ADS1015.h"
//#include "cls_Selial.h" 



//----------------------------------
#ifdef BLUETOOTH
#include <BluetoothSerial.h>
//#include <BLEDevice.h>
//#include <WiFi.h>
#include "Arduino_Ini_Setup.h"
#endif
//----------------------------------



//-------------------------------------------
//◆I2C-----
#define ADDRESS_1115 0x48  //ADS1115のアドレス
#define USE_ANALOG_0
#define USE_ANALOG_1
#define USE_ANALOG_2
#define USE_ANALOG_3


//◆シリアル-----
#define SERIAL_BAUDRATE 115200  //ボーレート
//◆I2C
#define SCL_SPEED 400000

#define SENSOR_0_NO 32
#define SENSOR_1_NO 33
#define SENSOR_2_NO 34
#define SENSOR_3_NO 35

#define BLUE_LED 2
#define SW0 0
#define DEB_IO 4
#define TIME_IO 12



//----------------------------------
#ifdef BLUETOOTH
BluetoothSerial Serial_BT;
HardwareSerial Serial00 = Serial;
#else
HardwareSerial Serial00 = Serial;
#endif
//----------------------------------


//サンプリング周期を調整するための待ち時間
#define WAIT_TIME 100;
int iWaitTime = 66;


//-------------------------------------------
Adafruit_ADS1015 ads1015; //ADS1015クラス宣言
Adafruit_ADS1115 ads1115(ADDRESS_1115); //ADS1115クラス宣言
//cls_Selial clsSerial;
//-------------------------------------------

String sOutData = "";
//int16_t adc0, adc1, adc2, adc3;
//double d_adc0, d_adc1, d_adc2, d_adc3;
int i_adc0, i_adc1, i_adc2, i_adc3;
int iCntSW = 0;
int iCntMeasurement = 0;

int iMilSec = 0, iSec = 0, iMin = 0;
long lHour = 0;

bool blSW = false;
bool blSensor0 = false, blSensor1 = false, blSensor2 = false, blSensor3 = false;

//測定開始状態
bool flgMeasurement = false;

uint16_t config_A0;

hw_timer_t* timer = NULL;

//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************

//************************************************************************************************
//センサーが接続されているかをチェック
//************************************************************************************************
void mSensorSet()
{
  if (!digitalRead(SENSOR_0_NO) == LOW)
    blSensor0 = true;
  if (!digitalRead(SENSOR_1_NO) == LOW)
    blSensor1 = true;
  if (!digitalRead(SENSOR_2_NO) == LOW)
    blSensor2 = true;
  if (!digitalRead(SENSOR_3_NO) == LOW)
    blSensor3 = true;
}
//************************************************************************************************
//ポートのセット
//************************************************************************************************
void mSetDigitalIO()
{
  //青LED制御ポートをセット
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, 0);

  //タクトスイッチポートをセット
  pinMode(SW0, INPUT);

  //デバッグ用ポート
  pinMode(DEB_IO, OUTPUT);
  digitalWrite(DEB_IO, 0);

  //タイマー用ポート
  pinMode(TIME_IO, OUTPUT);
  digitalWrite(TIME_IO, 0);

  //センサ接続状態
  pinMode(SENSOR_0_NO, INPUT_PULLDOWN);
  pinMode(SENSOR_1_NO, INPUT_PULLDOWN);
  pinMode(SENSOR_2_NO, INPUT_PULLDOWN);
  pinMode(SENSOR_3_NO, INPUT_PULLDOWN);
}

//************************************************************************************************
//起動時のLED点灯
//************************************************************************************************
void mStartupLED()
{
  digitalWrite(BLUE_LED, 1);
  delay(100);
  digitalWrite(BLUE_LED, 0);
  delay(100);
  digitalWrite(BLUE_LED, 1);
  delay(100);
  digitalWrite(BLUE_LED, 0);
  delay(100);
  digitalWrite(BLUE_LED, 1);
  delay(100);
  digitalWrite(BLUE_LED, 0);
  delay(100);

}

//************************************************************************************************
//サンプリングの1周期をチェック
//************************************************************************************************
void mCheckMeasurementCounter(int& cnt)
{
  if (cnt >= 10)
  {
    cnt = 0;
    digitalWrite(DEB_IO, !digitalRead(DEB_IO));
  }
}

//************************************************************************************************
//タイマー割り込み処理
//************************************************************************************************
void mTimeCnt()
{
  digitalWrite(TIME_IO, !digitalRead(TIME_IO));
  iMilSec++;
  if (iMilSec >= 1000)
  {
    iMilSec -= 1000;
    iSec++;

    if (iSec >= 60)
    {
      iSec -= 60;
      iMin++;
      if (iMin >= 60)
      {
        iMin = 0;
        lHour++;
      }
    }
  }
}
//************************************************************************************************
//タイマーカウンタのリセット
//************************************************************************************************
void mTimerReset()
{
  iMilSec = 0;
  iSec = 0;
  iMin = 0;
  lHour = 0;

  timerAlarmDisable(timer);
}



//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************


//********************************************************************************************
//セットアップ
//********************************************************************************************

void setup() {

  //シリアルポートのセット--------------
  Serial00.begin(SERIAL_BAUDRATE);
  Serial00.println("-----Serial set done-----");

  //デジタルIOのセット--------------
  mSetDigitalIO();
  Serial00.println("-----DigiralIO set done-----");


#ifdef BLUETOOTH
  //Bluetoothのセット--------------------
  if (!mESP32_BLSerial_Set(Serial_BT, "BT_um", Serial00))
  {
    //digitalWrite(BLUE_LED, 0);
    Serial00.print("-----Bluetooth err-----");
    while (true);
  }
  else
  {
    Serial00.print("-----Bluetooth set-----");
    //digitalWrite(BLUE_LED, 1);
  }
#endif


  //1ミリタイマーのセット■■■■■
  timer = timerBegin(0, 80, true);//(タイマー番号, 分周比, カウントアップ[true]orダウン[false])
  timerAttachInterrupt(timer, mTimeCnt, true);
  timerAlarmWrite(timer, 1000, true);
  //■■■■■■■■■■■■

  mStartupLED();

  //ADS1115のセット--------------
  ads1115.begin();
  //Wire.setClock(100000);
  //ads1115.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V 1 bit= 0.1875   mV
  ads1115.setGain(GAIN_ONE);    // +/- 4.096V 1 bit= 0.125    mV
  //ads1115.setGain(GAIN_TWO);    // +/- 2.048V 1 bit= 0.0625   mV
  //ads1115.setGain(GAIN_FOUR);   // +/- 1.024V 1 bit= 0.03125  mV
  //ads1115.setGain(GAIN_EIGHT);    // +/- 0.512V 1 bit= 0.015625 mV
  //ads1115.setGain(GAIN_SIXTEEN);  // +/- 0.256V 1 bit= 0.0078125mV

  Serial00.println("-----ADS1115 set done-----");


  //Wire.begin(ADS1015_ADDRESS);
  //Wire.setClock(400000);

  //================
  //gpio_set_direction(GPIO_NUM_35, GPIO_MODE_OUTPUT);
  //gpio_set_level(GPIO_NUM_35, 0);

  Serial00.println("-----Standby-----");
}



//************************************************************************************************
//メインループ
//************************************************************************************************
void loop() {

  sOutData = "";


  while (!flgMeasurement)
  {
    delay(5);
    //スイッチが押されていたらカウント
    if (digitalRead(SW0) == 0)
      iCntSW++;
    else
      iCntSW = 0;
    //カウンターが長押しされたらスタンバイループを抜けて計測開始
    if (iCntSW >= 50)
    {
      //カウンタをリセット
      iCntSW = 0;
      //測定フラグをセット
      flgMeasurement = true;

      Serial00.println("-----Stardt measurement-----");

      //青LEDを点灯
      digitalWrite(BLUE_LED, 1);
    }
  }

  //測定開始のLED点灯
  digitalWrite(BLUE_LED, 1);

  //タイマースタート
  timerAlarmEnable(timer);
  iCntMeasurement++;

#ifdef SENSOR_SET
  //=====================================================================
  mSensorSet();
#ifdef USE_ANALOG_0
  if (blSensor0)
    i_adc0 = ads1115.readADC_SingleEnded(0);// *(double)0.00025226;
  else
    i_adc0 = 0;
  sOutData += i_adc0;
  sOutData += ",";
#endif

#ifdef USE_ANALOG_1
  if (blSensor1)
    i_adc1 = ads1115.readADC_SingleEnded(1);// *(double)0.00025226;
  else
    i_adc1 = 0;
  sOutData += i_adc1;
  sOutData += ",";
#endif

#ifdef USE_ANALOG_2
  if (blSensor2)
    i_adc2 = ads1115.readADC_SingleEnded(2);// *(double)0.00025226;
  else
    i_adc2 = 0;
  sOutData += i_adc2;
  sOutData += ",";
#endif

#ifdef USE_ANALOG_3
  if (blSensor3)
    i_adc3 = ads1115.readADC_SingleEnded(3);// *(double)0.00025226;
  else
    i_adc3 = 0;
  sOutData += i_adc3;
  sOutData += ",";
  //=====================================================================
#endif

#else
  //=====================================================================
#ifdef USE_ANALOG_0
  i_adc0 = ads1115.readADC_SingleEnded(0);// *(double)0.00025226;
#else
  i_adc0 = 0;
#endif
  sOutData += i_adc0;
  sOutData += ",";

#ifdef USE_ANALOG_1
  i_adc1 = ads1115.readADC_SingleEnded(1);// *(double)0.00025226;
#else
  i_adc1 = 0;
#endif
  sOutData += i_adc1;
  sOutData += ",";

#ifdef USE_ANALOG_2
  i_adc2 = ads1115.readADC_SingleEnded(2);// *(double)0.00025226;
#else
  i_adc2 = 0;
#endif
  sOutData += i_adc2;
  sOutData += ",";

#ifdef USE_ANALOG_3
  i_adc3 = ads1115.readADC_SingleEnded(3);// *(double)0.00025226;
#else
  i_adc3 = 0;
#endif
  sOutData += i_adc3;
  sOutData += ",";
#endif
  //=====================================================================


  //時間情報を追加
  //sOutData += " time:";
  sOutData += lHour;
  sOutData += "'";
  sOutData += iMin;
  sOutData += "'";
  sOutData += iSec;
  sOutData += "'";
  sOutData += iMilSec;

  //シリアル出力
  Serial00.println(sOutData);

  //測定カウンターをチェック　
  mCheckMeasurementCounter(iCntMeasurement);

  //スイッチが押されていたらカウント
  if (digitalRead(SW0) == 0)
    iCntSW++;
  //スイッチが押されていなければ、カウンタをリセット
  else
    iCntSW = 0;
  //SWが長押しされたら計測停止してスタンバイループへ
  if (iCntSW >= 30)
  {
    //カウンタをリセット
    iCntSW = 0;
    iCntMeasurement = 0;
    //タイマーカウンタをリセット
    mTimerReset();
    //測定フラグをリセット
    flgMeasurement = false;
    //青LEDを消灯
    digitalWrite(BLUE_LED, 0);

    Serial00.println("-----End measurement-----");

    while (digitalRead(SW0) == 0);

    Serial00.println("-----Standby-----");

  }

  delay(iWaitTime);

}
