#include "BzCtrl.h"
#include "LedCtrl.h"

int iDuty = PWM_RESOLUTION / 2;
extern float flMelody[][12];

int beat = 1110;

//*******************************************************
enum enmMelodyNote
{
    all = 0,
    harf,
    quad,
    eighth,
    sixteenth
};

float flTime[] = {beat, beat / 2 ,beat / 4, beat / 8, beat / 16};
//*******************************************************

enum enmMelody
{
    mDO = 0,
    mDOs,
    mRE,
    mREs,
    mMI,
    mFA,
    mFAs,
    mSO,
    mSOs,
    mRA,
    mRAs,
    mSI
};

//*******************************************************
void InitBz()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ1000, PWM_BIT_NUM);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
    //ledcWrite(PWM_CHANNEL0, PWM_DUTY);
}

//*******************************************************
void BuzzerRing_Dot(int ringNum, int iPeriod)
{
    for (int i = 0; i < ringNum; i++)
    {
        ledcWrite(PWM_CHANNEL0, iDuty);
        delay(iPeriod);
        ledcWrite(PWM_CHANNEL0, 0);
        delay(iPeriod);
    }
}

//*******************************************************
void BzRing(float frq, float ringTime)
{
    ledcSetup(PWM_CHANNEL0, frq, PWM_BIT_NUM);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
    ledcWrite(PWM_CHANNEL0, iDuty);
    delay(ringTime);
}

//*******************************************************
void BzStop(float stopTime)
{
    ledcWrite(PWM_CHANNEL0, 0);
    delay(stopTime);
}

//*******************************************************
void Bz_DragonQuest_First()
{
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[quad] + flTime[eighth] - 100);
    LedOff();
    delay(100);
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[sixteenth]);
    LedOff();
    BzStop(flTime[sixteenth]);
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mDO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mSO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mRA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMelody[3][mDO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mRA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mSO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[quad] + flTime[eighth] - 100);
    LedOff();
    delay(100);
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[sixteenth]);
    LedOff();
    BzStop(flTime[sixteenth]);
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[quad] + flTime[eighth] * 1.025 - 100);
    LedOff();
    delay(100);
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[sixteenth] * 1.05);
    LedOff();
    BzStop(flTime[sixteenth] * 1.075);
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[eighth] * 1.1);
    LedOff();
    BzStop(flTime[eighth]* 1.2);
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[eighth] * 1.4);
    LedOff();
    BzStop(flTime[eighth] * 1.75);
    //--------------
    LedRighting();
    BzRing(flMelody[2][mDO], flTime[eighth] * 1.75);
    LedOff();
    BzStop(flTime[eighth] * 1.75);
    LedRighting();
    BzRing(flMelody[2][mMI], flTime[quad] * 2 - 100);
    LedOff();
    delay(100);
    LedRighting();
    BzRing(flMelody[2][mRE], flTime[harf] + flTime[all] * 2);
    LedOff();
}

//*************************************************************************************
//*************************************************************************************
//*************************************************************************************


void BzGoUp(float ringTime, float stopTime)
{
    InitBz();

    for(int i = 0; i < 12; i++)
    {
        BzRing(flMelody[3][i], ringTime);
        BzStop(stopTime);
    }
}

void BzGoDown(float ringTime, float stopTime)
{
    InitBz();

    for(int i = 12; i > 0; i--)
    {
        BzRing(flMelody[3][i], ringTime);
        BzStop(stopTime);
    }
}
