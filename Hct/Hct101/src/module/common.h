// #include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <string>
#include <WebServer.h>

int mode = 0;
enum enmMode
{
  enmNormal,
  enmSetting
};

HardwareSerial sr(1);
WebServer server(80);
TwoWire wr = Wire;
String target = ""; // この変数をPOSTメソッドで書き換える
// bool blOpening = false;
bool blClosing = false;
bool blOpened = false;
int openTime = 1000000;
int timeAdjuster = 3;

struct tm timeInf;
struct tm timeInfRTC;

char s[20];//文字格納用

hw_timer_t* tOpen_W = NULL;
hw_timer_t* tNowOpen_W = NULL;
hw_timer_t* tClose_W = NULL;

hw_timer_t* tOpen_F = NULL;
hw_timer_t* tNowOpen_F = NULL;
hw_timer_t* tClose_F = NULL;

hw_timer_t* tSettingOff = NULL;

const int aryMotorSts[4][4] = 
{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

enum enmContentNum_Set
{
  enm_non,
  enm_led,
  enm_motor,
  enm_buzzer
};

//↓↓↓↓==============================================
// struct setValue{
//   int interval;
//   int length;
// };

// setValue setVal_W = {0, 1, 0, 0, 1, false};
// setValue setVal_F = {0, 1, 0, 0, 1, false};
// setValue setVal_W = {0, 1};
// setValue setVal_F = {0, 1};
//↑↑↑==============================================


//↓↓↓==============================================
// struct deviceStatus{
//   bool opened;
//   bool nowRun;
//   bool regularRun;
//   bool nowOppenning;
//   bool regularOppenning;
//   bool nowClosing;
//   bool regularClosing;
//   bool closingRun;
// };

// deviceStatus deviceSts = {false, false, false, false, false, false, false};


struct deviceStatus{
  bool opened;
  bool oppenning;
  bool closing;
  int interval;
  int length;
  int nowLength;
  int setTime;
};

deviceStatus deviceSts_W = {false, false, false, 0, DEFAULT_LENGTH, DEFAULT_LENGTH, 0};
deviceStatus deviceSts_F = {false, false, false, 0, DEFAULT_LENGTH, DEFAULT_LENGTH, 0};

enum enmDevStsAddress
{
  enm_opened = 0,
  enm_oppenning,
  enm_closing,
  enm_interval = 6,
  enm_length = 10

};
//↑↑↑==============================================

int operationReservation = 0;
enum operationType{
  enmNon = 0,
  enmNowOppenning,
  enmNowClosing,
  enmRegularOppenning,
  enmRegularClosing,
};


//↓↓↓==============================================
String httpContents[] =
{
  "/get",
  "/now",
  "/set",
  "/adjust",
};

enum enmHttpContents
{
    enmGet,
    enmNow,
    enmSet,
    enmAdjust
};
//↑↑↑==============================================



//↓↓↓==============================================
const String paramWord_get[] = 
{
  "time", 
  "temperture", 
  "humidity",
  "all"
};
enum enmContentNum_Get
{
  enm_time,
  enm_temperture,
  enm_humidity,
  enm_all
};
//↑↑↑==============================================


//↓↓↓==============================================
const String errorMessage[] = 
{
  "Not found",
  "Program Error <setDevice ledColor error>",
  "Status error --- '?sts=on' or '?sts=off'",
  "Status error --- '?sts=time' or '?sts=temperture' or '?sts=humidity'"
};
enum enmErrorMessage
{
  enmNotFound,
  enmPrgError,
  enmStsError_set,
  enmStsError_get
};
//↑↑↑==============================================


