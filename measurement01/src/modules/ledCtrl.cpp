#include "ledCtrl.hpp"

void ledCtrl::ledFlash(int port, int interval_, int num)
// void LedFlash(int port, int interval_, int num)
{
  for(int i = 0; i < num; i++)
  {
    delay(interval_ * 20);
    digitalWrite(port, 0);
    delay(interval_);
    digitalWrite(port, 1);
  }
}