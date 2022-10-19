#include "ledCtrl.h"

void ledCtrl::ledFlash(HardwareSerial& sr, int port, int interval_, int num)
// void LedFlash(int port, int interval_, int num)
{
  sr.println("Flash start");
  for(int i = 0; i < num; i++)
  {
    delay(interval_ * 20);
    digitalWrite(port, 0);
    delay(interval_);
    digitalWrite(port, 1);
    sr.println(i);
  }
  sr.println("Flash end");
}