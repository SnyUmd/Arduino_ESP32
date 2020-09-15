#include <Arduino.h>
//#include <Esp32.h>
#include <time.h>
#include "def.h"
#include "Init.h"
#include "LedCtrl.h"
#include "Bzctrl.h"

extern float flMelody[][12];

//********************************************************
void setup() 
{
  Initialize_Port();

  LedFlash(50,3,true);

  InitBz();
  //BuzzerRing(100, GetMelody(0,0), true, 100);
  BuzzerRing(100, flMelody[0][0], true, 100);
  BuzzerRing(100, GetMelody(0,2), true, 100);
  BuzzerRing(100, GetMelody(0,4), true, 100);
  BuzzerRing(100, GetMelody(0,5), true, 100);
  BuzzerRing(100, GetMelody(0,7), true, 100);
  BuzzerRing(100, GetMelody(0,9), true, 100);
  BuzzerRing(100, GetMelody(0,11), true, 100);

}

//********************************************************
void loop() 
{
  


}

//****************************************************************************************************************
//****************************************************************************************************************
//****************************************************************************************************************



