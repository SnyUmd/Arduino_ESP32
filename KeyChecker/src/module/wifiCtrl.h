#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// #include <Arduino.h>

bool wifiInit(WiFiClass& wifi, HardwareSerial& sr, char* ssid, char* pass, char* host_name, bool bl_ota);
//使い方
// if(!wifiInit(WiFi, serial, SSID, PASS, HOST_NAME, false))
// {
//     bzErrorSound();
//     while(1)
//     {
//         digitalWrite(PORT_LED_F, !digitalRead(PORT_LED_F));
//         digitalWrite(PORT_LED_W, !digitalRead(PORT_LED_W));
//         delay(100);
//     }
// }

struct tm getTimeInf();
void arrangeTime(char* s, tm time_inf);