#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// #include <Arduino.h>

// void wifiInit(WiFiClass& wifi, HardwareSerial& sr, char* ssid, char* pass, String host_name, bool bl_ota);
void wifiInit(WiFiClass& wifi, HardwareSerial& sr, String ssid, String pass, String host_name, bool bl_ota);

struct tm getTimeInf();
void arrangeTime(char* s, tm time_inf);