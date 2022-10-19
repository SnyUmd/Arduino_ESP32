// #include <Arduino.h>
#include <WiFi.h>
#include <string>
#include <WebServer.h>
// #include <iostream>

// int actionState = 0;

HardwareSerial sr(1);
WebServer server(80);
String target = ""; // この変数をPOSTメソッドで書き換える

struct tm timeInf;
struct tm timeInfRTC;

char s[20];//文字格納用

int aryMotorSts[4][4] = 
{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

struct httpStatus{
  Uri uri;
  String returnMess;
  bool sts;
};

httpStatus httpSts[]
{
  {"/led/flash/r", "LED red flash", false},
  {"/led/flash/g", "LED green flash", false},
  {"/get_time", "Time", false},
  {"/motor/start", "Motor start", false},
  {"/motor/stop", "Motor stop", false},
  {"/buzzer/start", "Buzzer ring", false},
  {"/buzzer/stop", "Buzzer stop", false}
};

enum enmHttpState
{
    enmLedFlashR,
    enmLedFlashG,
    enmGetTime,
    enmMotorStart,
    enmMotorStop,
    enmBuzzerRing,
    enmBuzzerStop
};