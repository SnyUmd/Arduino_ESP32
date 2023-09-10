#include <Arduino.h>
#include <WebServer.h>

WebServer server(80);
HardwareSerial sr(0);


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