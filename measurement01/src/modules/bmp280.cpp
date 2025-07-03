#include "bmp280.hpp"

bool InitBmp280(Adafruit_BMP280& bmp_, TwoWire& wire_, HardwareSerial& sr_)
{
    wire_.begin();

    if (!bmp_.begin(BPM280_ADDRESS)) {  // BMP280を初期化する
        sr_.println("BMP280 init fail");
        return false;
    }

    return true;
}

float readPressBmp280(Adafruit_BMP280& bmp_)
{
    return bmp_.readPressure();
}

float readTempBmp280(Adafruit_BMP280& bmp_)
{
    return bmp_.readTemperature();
}