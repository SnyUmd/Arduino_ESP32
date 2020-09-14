
#include "LedCtrl.h"

void LedFlash(int iPeriod, int iLightNum, bool blLighsUp)
{
  int iLoop = iLightNum*2;
  for(int i = 0; i < iLoop; i++)
  {
    digitalWrite(LED_RED, !digitalRead(LED_RED));
    digitalWrite(LED_GRE, !digitalRead(LED_GRE));
    delay(iPeriod);
  }
  if(blLighsUp)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GRE, LOW);
  }
  else
  {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GRE, HIGH);
  }
}