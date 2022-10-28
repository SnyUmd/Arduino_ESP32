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
bool blOpening = false;
bool blClosing = false;
bool blOpened = false;
int openTime = 1000000;

struct tm timeInf;
struct tm timeInfRTC;

char s[20];//文字格納用

const int aryMotorSts[4][4] = 
{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

struct httpStatus{
  Uri uri;
  bool sts;
};

httpStatus httpSts[]
{
  {"/led/r", false},
  {"/led/g", false},
  {"/motor", false},
  {"/buzzer", false},
  {"/get", false},
  {"/now", false}
};

enum enmHttpState
{
    enmLedR = 0,
    enmLedG,
    enmMotor,
    enmBuzzer,
    enmGet,
    enmNow
};

const String paramWord_set[] = {"on", "off"};
enum enmLedParams{enm_on = 0, enm_off};

const String paramWord_led[] = {"g", "r"};

enum enmContentNum_Set
{
  enm_non,
  enm_led,
  enm_motor,
  enm_buzzer
};

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