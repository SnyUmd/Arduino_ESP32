#include <WiFi.h>
// #include <Arduino.h>

void wifiInit(WiFiClass& wifi, HardwareSerial& sr, char* ssid, char* pass, char* host_name);

struct tm getTimeInf();
void arrangeTime(char* s, tm time_inf);