#include <Arduino.h>
#include <WebServer.h>
#include <Wire.h>
#include "BzCtrl.h"

WebServer server(80);
HardwareSerial sr(0);
TwoWire wr = Wire;

//task buzzer
bool flgBz = false;//trueでringBzにセットされた関数を実行
// typedef void(*ringing)();//ブザー鳴動関数格納用　関数ポインタの宣言

enum enmBz
{
    enm_power_on,
    enm_error,
    enm_stanby_ok,
    enm_int_ring,
    enm_free0,
};
struct structBz
{
    typedef void(*ringing)();//ブザー鳴動関数格納用　関数ポインタの宣言
    ringing ring;
    bool blBz;
};
structBz bzSts[] = 
{
    {bzPowerOn, false},
    {bzErrorSound, false},
    {stanbyOk, false},
    {bzReceivedRing, false},
    {bzDummy, false}
};

enum enmHttpContents
{
    // enmGet,
    // enmNow,
    // enmSet,
    // enmAdjust
};
String httpContents[] =
{
//   "/get",
//   "/now",
//   "/set",
//   "/adjust"
};

