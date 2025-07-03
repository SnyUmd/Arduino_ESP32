#define TIMER_NUM_OPEN_W 0
#define TIMER_NUM_CLOSE_W 1
#define TIMER_NUM_OPEN_F 2
#define TIMER_NUM_CLOSE_F 3

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <string>
#include <WebServer.h>
#include <HTTPClient.h>
#include <vector>
#include <EEPROM.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <iostream>

#include "modules/initialize_stc00.hpp"
#include "modules/Timer.hpp"
#include "modules/wifiCtrl.hpp"
#include "modules/ledCtrl.hpp"
#include "modules/BzCtrl.hpp"
#include "modules/ESP32_eep.hpp"
#include "modules/htu21d.hpp"
#include "modules/bmp280.hpp"
#include "modules/scd4x.hpp"
#include "modules/def_stc00.hpp"
#include "classes/Bh1750fviCtrl.hpp"
#include "classes/I2cCtrl.hpp"

//debug---------
int gTest = 0;
//^--------------


Bh1750fviCtrl clsBh1750;
ledCtrl LC;
I2cCtrl i2cCtrl;
Adafruit_BMP280 bmp;
BluetoothSerial srBT;

//Lighting timer-------------------------
hw_timer_t* tLightOff = NULL;
int tLightOff_Num = 0;
int lightOffTime = 10 * 1000000;
// int lightOffTime = 5000;

// long lightingStartTime;
// ^-------------------------------

//LED flash timer------------------------
hw_timer_t* tLedFlash = NULL;
int tLedFlash_Num = 1;
int ledFlashStepTime = 100000;
//^--------------------------------------

String prgStartTime;

// HardwareSerial sr(0);//RX:34 TX:35
HardwareSerial sr(2);//RX:16 TX:17
// WebServer server(80);
WebServer server(SERVER_PORT_NUM);
WiFiClass wf;
// BluetoothSerial Serial_BT;
TwoWire wr = Wire;

String target = ""; // この変数をPOSTメソッドで書き換える

struct tm timeInf;
struct tm timeInfRTC;

int accessNum = 0;
int modeBh1750 = 2; //照度センサのモード選択
                    //0 : low resolution 4lx
                    //1 : hight resolution 1lx
                    //2 : hight resolution2 0.5lx


char s[20];//文字格納用

hw_timer_t* tTest = NULL;
bool blModeSetting = false;

//----------------------------
EEPROMClass eep = EEPROM;
int eepSize = 512;
String eep_read = "";
char eep_read_buf[50];
// String host_name = "petoasis";
// String s_hName = "s-tech";
//^----------------------------

struct wifi_sts
{
  String ssid;
  String pass;
  String host_name;
  String portNum;
  int eep_address_ssid;
  int eep_address_pass;
  int eep_address_host_name;
  int eep_address_port_num;
  int eep_address_sequence_num;
  int eep_address_measurement_num;
  int eep_address_start_time;
  int stop;
};

wifi_sts wifiSts = 
{
  SSID_0,
  PASS_0,
  HOST_NAME,
  "8000",
  0x00,
  0x20,
  0x40,
  0x60,
  0x80,
  0x84,
  0x88,
  0x118,
  //※EEP領域の確保数注意(eepSize)
};

//デバイスの存在==============================================
enum enmDeviceNum{
  enmTempHumd_Htu21,
  enmPresure_Bmp280,
  enmIllumi_Bh1750fvi,
  enmCo2_Scd4x,
  enmDeviceMax//※末尾固定　デバイスの数を取得する為
};

struct IsDevice{
  int address;
  bool isCommunication;
};

IsDevice isDevice[enmDeviceMax] ={
  {0x40, false},
  {0x77, false},
  {0x23, false},
  {0x62, false}
};
//^============================================================

//↓↓↓==============================================
static char outputtext[100]="";
StaticJsonDocument<100> jsonSensor;

String sensorItem[] = 
{
  "name",
  "time",
  "buttery",
  "temp",
  "humd",
  "press",
  "co2",
  "illum",
};

enum entSensorItem
{
  enmDeviceName,
  enmTime,
  enmButtery,
  enmTemp,
  enmHumd,
  enmPress,
  enmCo2,
  enmIllumi,
};

//↑↑↑==============================================

//↓↓↓==============================================
String httpContents[] =
{
  "/read",
  "/set",
  "/wifi"
};

enum enmHttpContents
{
    enmRead,
    enmSet,
    enmWifi
};
//↑↑↑==============================================


//↓↓↓==============================================
const String BTcommand[] = 
{
  "r",
  "w",
  "s"
};
enum enmBTcommand
{
  enm_read,
  enm_write,
  enm_start
};


//関数----------------------------------
void setHttpAction(int port_num);
void httpAction(int contentNum);

float getTemp_htu21d();
float getHumd_htu21d();
String getTimeWifi();
void readEEP();
void wifiStatusReset();
void wifiStatusIntake();
void modeSetting();
String serialRead(BluetoothSerial& srBT, char c_end, long timeout);
void ledOkOrNg(bool is_ok);
void setJsonSensor();
void scd4x_Mesurement();
void accessCount();
void set_sensorInstallationStatus(TwoWire& wire_);

//タイマー割込み処理
void IRAM_ATTR lightOff();
void setLight();
void ResetLightTime();

void IRAM_ATTR ledFlash();


// void init();
// string getTemp();
// string getHumd();

