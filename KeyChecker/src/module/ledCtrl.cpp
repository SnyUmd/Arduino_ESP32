#include "ledCtrl.h"
#include <Arduino.h>

void ledCtrl::ledFlash(int port, int interval_, int num)
// void LedFlash(int port, int interval_, int num)
{
  for(int i = 0; i < num; i++)
  {
    delay(interval_ * 20);
    digitalWrite(port, 1);
    delay(interval_);
    digitalWrite(port, 0);
  }
}