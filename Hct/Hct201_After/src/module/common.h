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

HardwareSerial sr(0);
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


//----------------------------
EEPROMClass ep = EEPROM;
String eep_read = "";
char eep_read_buf[50];
String s_hName = "s-tech";
//----------------------------

String host_name = "petoasis";

struct wifi_sts
{
  String ssid;
  String pass;
  String host_name;
  int eep_address_ssid;
  int eep_address_pass;
  int eep_address_host_name;
};

wifi_sts wifiSts = 
{
  SSID,
  PASS,
  HOST_NAME,
  0x00,
  0x10,
  0x20,
};

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

struct Responses{
  String Target;
  String Action;
  String Item;
  String Value;
};

Responses ResponsWord = 
{
  "\"target\"", 
  "\"action\"",
  "\"item\"", 
  "\"value\""
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
  bool serverInit;
  int portLED;
  int interval;
  // int length;
  // int nowLength;
  int setTime;
  int timerNumOpen;
  int timerNumClose;
  int adjustMotion;
  String melody;
  hw_timer_t* tOpen;
  hw_timer_t* tClose;
};

deviceStatus deviceSts_W = 
  { false, false, false, false, false, false, false, false, true,
    PORT_LED_W ,0, 
    /*DEFAULT_LENGTH, 
    DEFAULT_LENGTH,*/ 0, 
    TIMER_NUM_OPEN_W, 
    TIMER_NUM_CLOSE_W, 10,
    "4do8-4re8-4mi8-4fa8-4so8-4ra8-4si8-5do8", NULL, NULL};
    // "4do8-4re8-4mi8", NULL, NULL};
deviceStatus deviceSts_F = 
  { false, false, false, false, false, false, false, false, true,
    PORT_LED_F ,0, 
    /*DEFAULT_LENGTH, 
    DEFAULT_LENGTH,*/ 0, 
    TIMER_NUM_OPEN_F, 
    TIMER_NUM_CLOSE_F, 10,
    "2do8-2re8-2mi8-2fa8-2so8-2ra8-2si8-3do8", NULL, NULL};
    // "2do8-2re8-2mi8", NULL, NULL};

bool setRinging = false;
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
  "/wifi"
};

enum enmHttpContents
{
    enmGet,
    enmNow,
    enmSet,
    enmAdjust,
    enmWifi,
};
//↑↑↑==============================================



//↓↓↓==============================================
const String paramWord_get[] = 
{
  "time", 
  "temperture", 
  "humidity",
  "interval",
  "ring",
  "melody",
  "next",
  "all"
};
enum enmContentNum_Get
{
  enm_time,
  enm_temperture,
  enm_humidity,
  enm_interval,
  enm_ring,
  enm_melody,
  enm_next,
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


