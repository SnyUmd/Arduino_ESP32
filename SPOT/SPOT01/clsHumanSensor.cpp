#include "clsHumanSensor.h"
#include "def.h"

//********************************************************
bool clsHumanSensor::HumanCheck(int iLoopCnt, int iCycleTime, int iExecuteNum)
{
  int iCnt = 0;
  for(int i = 0; i < iLoopCnt; i++)
  {
    if(digitalRead(HUMAN_SENSOR)) iCnt++;
    delay(iCycleTime);
  }

  if(iCnt >= iExecuteNum)
    return true;
  else
    return false;
}