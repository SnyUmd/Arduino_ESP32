#include <Arduino.h>
#include <WebServer.h>
#include <Wire.h>

WebServer server(80);
HardwareSerial sr(0);
TwoWire wr = Wire;

//task buzzer
bool flgBz = false;//trueでringBzにセットされた関数を実行
typedef void(*ringing)();//ブザー鳴動関数格納用　関数ポインタの宣言
ringing ringBz;//ブザー鳴動用　関数ポインタオブジェクト

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