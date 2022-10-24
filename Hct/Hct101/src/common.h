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
  String returnMess;
  bool sts;
};

httpStatus httpSts[]
{
  {"/led/r", "", false},
  {"/led/g", "", false},
  {"/get_time", "Time", false},
  {"/motor", "Motor start", false},
  {"/buzzer", "Buzzer ring", false},
  {"/get", "Temperture", false}
};

// httpStatus httpSts[]
// {
//   {"/led/on/r", "LED red on", false},
//   {"/led/off/r", "LED red off", false},
//   {"/led/on/g", "LED green on", false},
//   {"/led/off/g", "LED green off", false},
//   {"/get_time", "Time", false},
//   {"/motor/start", "Motor start", false},
//   {"/motor/stop", "Motor stop", false},
//   {"/buzzer/start", "Buzzer ring", false},
//   {"/buzzer/stop", "Buzzer stop", false},
//   {"/temperture", "Temperture", false},
//   {"/humidity", "Humidity", false}
// };

enum enmHttpState
{
    enmLedR = 0,
    enmLedG,
    enmGetTime,
    enmMotor,
    enmBuzzer,
    enmGet
};
// enum enmHttpState
// {
//     enmLedOnR,
//     enmLedOffR,
//     enmLedOnG,
//     enmLedOffG,
//     enmGetTime,
//     enmMotorStart,
//     enmMotorStop,
//     enmBuzzerRing,
//     enmBuzzerStop,
//     enmGetTemperture,
//     enmGetHumidity
// };

const String paramLED[] = {"on", "off"};

enum enmLedParams{led_on = 0, led_off};