#include "LED_Ctrl.h"
#include "Def.h"
#include <Arduino.h>

void LED_Ctrl::LedFlash(int port, int interval_, int num)
// void LedFlash(int port, int interval_, int num)
{
  for(int i = 0; i < num; i++)
  {
    delay(interval_ * 20);
    digitalWrite(port, LED_ON);
    delay(interval_);
    digitalWrite(port, LED_OFF);
  }
}