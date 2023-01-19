#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// #include <Arduino.h>

bool wifiInit(WiFiClass& wifi, HardwareSerial& sr, char* ssid, char* pass, char* host_name, bool bl_ota);

struct tm getTimeInf();
void arrangeTime(char* s, tm time_inf);