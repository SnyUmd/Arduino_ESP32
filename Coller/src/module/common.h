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

//↓↓↓==============================================
struct Responses{
  String Target;
  String Action;
  String Item;
  String Value;
};

//Responsesの初期値
Responses ResponsWord = 
{
  "\"target\"", 
  "\"action\"",
  "\"item\"", 
  "\"value\""
};
//↑↑↑==============================================

//モータステータス
struct deviceStatus{
  bool opened;            //オープン中フラグ
  bool oppenning;         //オープン動作中フラグ
  bool closing;           //クローズ動作中フラグ
  bool flgNow;            //即時動作 命令フラグ
  bool adjustment;        //モータを右へ微調整 命令フラグ
  bool adjustmentLeft;    //モータを左へ微調整 命令フラグ
  bool ring;              //メロディ出し 命令フラグ
  bool ringing;           //メロディ鳴動中フラグ
  int portLED;            //LEDポート番号
  int interval;           //定期動作インターバル時間設定値
  int setTime;            //次駆動時間をセットした時の、電源投入から経過した時間
  int timerNumOpen;       //オープンのタイマー割り込み管理番号　MotorW：0　MotorF：1
  int timerNumClose;      //クローズのタイマー割り込み管理番号　MogorW：2　MotorF：3
  int adjustMotion;       //モータ調整開始 命令フラグ
  String melody;          //動作時鳴動メロディ音階　設定値
  hw_timer_t* tOpen;      //hwtimer オブジェクト
  hw_timer_t* tClose;     //hwtimer オブジェクト
};

//モータWステータスの初期値
deviceStatus deviceSts_W = 
  { false, false, false, false, false, false, false, false,
    PORT_LED_W ,0, 0, 
    TIMER_NUM_OPEN_W, 
    TIMER_NUM_CLOSE_W, 10,
    "4do8-4re8-4mi8-4fa8-4so8-4ra8-4si8-5do8", NULL, NULL};
//モータFステータスの初期値
deviceStatus deviceSts_F = 
  { false, false, false, false, false, false, false, false,
    PORT_LED_F ,0, 0, 
    TIMER_NUM_OPEN_F, 
    TIMER_NUM_CLOSE_F, 10,
    "2do8-2re8-2mi8-2fa8-2so8-2ra8-2si8-3do8", NULL, NULL};

//ブザー鳴動準備フラグ　ブザー鳴動が被らないように
bool setRinging = false;


//↓↓↓==============================================
//httpContentsの配列要素指定に使用
enum enmHttpContents
{
    enmGet,
    enmNow,
    enmSet,
    enmAdjust
};

String httpContents[] =
{
  "/get",
  "/now",
  "/set",
  "/adjust"
};
//↑↑↑==============================================



//↓↓↓==============================================
//paramWord_getの配列要素指定に使用
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
//↑↑↑==============================================

//↓↓↓==============================================
//errorMessageの配列要素指定に使用
enum enmErrorMessage
{
  enmNotFound,
  enmPrgError,
  enmStsError_set,
  enmStsError_get
};

const String errorMessage[] = 
{
  "Not found",
  "Program Error <setDevice ledColor error>",
  "Status error --- '?sts=on' or '?sts=off'",
  "Status error --- '?sts=time' or '?sts=temperture' or '?sts=humidity'"
};
//↑↑↑==============================================


