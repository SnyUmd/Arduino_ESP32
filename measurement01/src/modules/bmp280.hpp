// #define BPM280_ADDRESS 0x76//0x76 or 0x77
#define BPM280_ADDRESS 0x77//0x76 or 0x77

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

bool InitBmp280(Adafruit_BMP280& bmp_, TwoWire& wire_, HardwareSerial& sr_);
float readPressBmp280(Adafruit_BMP280& bmp_);
float readTempBmp280(Adafruit_BMP280& bmp_);
