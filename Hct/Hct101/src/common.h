// #include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <string>
#include <WebServer.h>

// #include <iostream>

// int actionState = 0;

HardwareSerial sr(1);
WebServer server(80);
TwoWire wr = Wire;
String target = ""; // この変数をPOSTメソッドで書き換える

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
  {"/get", false}
};

enum enmHttpState
{
    enmLedR = 0,
    enmLedG,
    enmMotor,
    enmBuzzer,
    enmGet
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
  "temperature", 
  "humidity"
};
enum enmContentNum_Get
{
  enm_time,
  enm_temperture,
  enm_humidity
};

const String errorMessage[] = 
{
  "Program Error <setDevice ledColor error>",
  "Status error --- '?sts=on' or '?sts=off'",
  "Status error --- '?sts=time' or '?sts=temparture' or '?sts=humidity'"
};
enum enmErrorMessage
{
  enmPrgError,
  enmStsError_set,
  enmStsError_get
};