#include "BzCtrl.h"

void InitBz()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ1000, PWM_BIT_NUM);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
    //ledcWrite(PWM_CHANNEL0, PWM_DUTY);
}

void BuzzerRing0(int ringNum, int iPeriod)
{
    //ledcSetup(PWM_CHANNEL0, PWM_FRQ1000, PWM_BIT_NUM);
    //ledcAttachPin(PWM_PIN, PWM_CHANNEL0);

    int iDuty = PWM_RESOLUTION / 2;
    for(int i = 0; i < ringNum; i++)
    {
        ledcWrite(PWM_CHANNEL0, iDuty);
        delay(iPeriod);
        ledcWrite(PWM_CHANNEL0, 0);
        delay(iPeriod);
    }
}

void BuzzerRing_Demo(int ringNum, int iPeriod)
{
  InitBz();

  BzDo0();
  BuzzerRing0(ringNum, iPeriod);
  BzRe0();
  BuzzerRing0(ringNum, iPeriod);
  BzMi0();
  BuzzerRing0(ringNum, iPeriod);
  BzFa0();
  BuzzerRing0(ringNum, iPeriod);
  BzSo0();
  BuzzerRing0(ringNum, iPeriod);
  BzRa0();
  BuzzerRing0(ringNum, iPeriod);
  BzSi0();
  BuzzerRing0(ringNum, iPeriod);
  BzDo1();
  BuzzerRing0(ringNum, iPeriod);
}

void BuzzerRing_Demo_Revers(int ringNum, int iPeriod)
{
  InitBz();

  BzDo1();
  BuzzerRing0(ringNum, iPeriod);
  BzSi0();
  BuzzerRing0(ringNum, iPeriod);
  BzRa0();
  BuzzerRing0(ringNum, iPeriod);
  BzSo0();
  BuzzerRing0(ringNum, iPeriod);
  BzFa0();
  BuzzerRing0(ringNum, iPeriod);
  BzMi0();
  BuzzerRing0(ringNum, iPeriod);
  BzRe0();
  BuzzerRing0(ringNum, iPeriod);
  BzDo0();
  BuzzerRing0(ringNum, iPeriod);
}

void BuzzerRing_Test()
{
    for(int i = 0; i < 1000; i++)
    {
        digitalWrite(BUZZER_PWM, HIGH);
        delay(1);
        digitalWrite(BUZZER_PWM, LOW);
        delay(1);
    }
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
