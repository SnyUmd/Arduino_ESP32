#define TIMER_NUM_OPEN_W 0
#define TIMER_NUM_CLOSE_W 1
#define TIMER_NUM_OPEN_F 2
#define TIMER_NUM_CLOSE_F 3

// #include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <string>
#include <WebServer.h>
#include <vector>

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

// hw_timer_t* tOpen_W = NULL;
// hw_timer_t* tNowOpen_W = NULL;
// hw_timer_t* tClose_W = NULL;

// hw_timer_t* tOpen_F = NULL;
// hw_timer_t* tNowOpen_F = NULL;
// hw_timer_t* tClose_F = NULL;

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
  bool flgNow;
  bool adjustment;
  bool adjustmentLeft;
  bool ring;
  bool ringing;
  int portLED;
  int interval;
  int length;
  int nowLength;
  int setTime;
  int timerNumOpen;
  int timerNumClose;
  String melody;
  hw_timer_t* tOpen;
  hw_timer_t* tClose;
};

deviceStatus deviceSts_W = 
  { false, false, false, false, false, false, false, false, 
    PORT_LED_G ,0, 
    DEFAULT_LENGTH, 
    DEFAULT_LENGTH, 0, 
    TIMER_NUM_OPEN_W, 
    TIMER_NUM_CLOSE_W,
    "0do8-0re8-0mi8-0fa8-0so8-0ra8-0si8-1do8", NULL, NULL};
deviceStatus deviceSts_F = 
  { false, false, false, false, false, false, false, false, 
    PORT_LED_R ,0, 
    DEFAULT_LENGTH, 
    DEFAULT_LENGTH, 0, 
    TIMER_NUM_OPEN_F, 
    TIMER_NUM_CLOSE_F,
    "2do8-2re8-2mi8-2fa8-2so8-2ra8-2si8-3do8", NULL, NULL};

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


