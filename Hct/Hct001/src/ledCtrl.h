#include <Arduino.h>
class ledCtrl
{
    public:
    void ledFlash(HardwareSerial& sr, int port, int interval_, int num);
};

// void LedFlash(int port, int interval_, int num);
