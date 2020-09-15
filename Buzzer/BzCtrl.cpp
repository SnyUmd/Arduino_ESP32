#include "BzCtrl.h"

void InitBz()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ1000, PWM_BIT_NUM);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
    //ledcWrite(PWM_CHANNEL0, PWM_DUTY);
}

void BuzzerRing_Dot(int ringNum, int iPeriod)
{
    int iDuty = PWM_RESOLUTION / 2;
    for(int i = 0; i < ringNum; i++)
    {
        ledcWrite(PWM_CHANNEL0, iDuty);
        delay(iPeriod);
        ledcWrite(PWM_CHANNEL0, 0);
        delay(iPeriod);
    }
}

void BuzzerRing(int ringTime, float frq, bool blStop, int iStopTime)
{
    int iDuty = PWM_RESOLUTION / 2;
    ledcSetup(PWM_CHANNEL0, frq, PWM_BIT_NUM);
    ledcWrite(PWM_CHANNEL0, iDuty);
    delay(ringTime);
    if(blStop)
    {
        ledcWrite(PWM_CHANNEL0, 0);
        delay(iStopTime);
    }
}

void BuzzerRing_Demo_00(int ringNum, int iPeriod)
{
  
}

void BuzzerRing_Demo(int ringNum, int iPeriod)
{
  InitBz();

  BzDo0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzRe0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzMi0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzFa0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzSo0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzRa0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzSi0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzDo1();
  BuzzerRing_Dot(ringNum, iPeriod);
}

void BuzzerRing_Demo_Revers(int ringNum, int iPeriod)
{
  InitBz();

  BzDo1();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzSi0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzRa0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzSo0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzFa0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzMi0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzRe0();
  BuzzerRing_Dot(ringNum, iPeriod);
  BzDo0();
  BuzzerRing_Dot(ringNum, iPeriod);
}

void BzStop()
{
    ledcWrite(PWM_CHANNEL0, 0);
}

void BzDo0()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ_DO_0, PWM_BIT_NUM);
    //ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
}
void BzRe0()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ_RE_0, PWM_BIT_NUM);
    //ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
}
void BzMi0()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ_MI_0, PWM_BIT_NUM);
    //ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
}
void BzFa0()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ_FA_0, PWM_BIT_NUM);
}
void BzSo0()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ_SO_0, PWM_BIT_NUM);
}
void BzRa0()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ_RA_0, PWM_BIT_NUM);
}
void BzSi0()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ_SI_0, PWM_BIT_NUM);
}
void BzDo1()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ_DO_1, PWM_BIT_NUM);
}
