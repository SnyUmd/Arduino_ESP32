#include "BzCtrl.h"
#include "LedCtrl.h"

int iDuty = PWM_RESOLUTION / 2;
//extern float flMScale[][12];

int beat = 1110;


//*******************************************************
float flMScale[][12] =
    {
        {130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220, 233.082, 246.942},
        {261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883},
        {523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880, 932.328, 987.767},
        {1046.502, 1108.731, 1174.659, 1244.508, 1318.51, 1396.913, 1479.978, 1567.982, 1661.219, 1760, 1864.655, 1975.533},
        {2093.005, 2217.461, 2349.318, 2489.016, 2637.02, 2793.826, 2959.955, 3135.963, 3322.438, 3520, 3729.31, 3951.066},
        {4186.009, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        /*　0ド , 1ド# , 2レ, 3レ#, 4ミ , 5ファ , 6ファ# , 7ソ , 8ソ# , 9ラ , 10ラ# , 11シ　*/
};

//*******************************************************
enum enmMelodyNote
{
    all = 0,
    harf,
    quad,
    eighth,
    sixteenth
};

float flTime[] = {beat, beat / 2, beat / 4, beat / 8, beat / 16};
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
int ary_scale[][3];
//{時間, 記号種, 音}
//記号種　
//0：休符
//1：音符
//2：伸ばし
void setScale(int iTime, int type = 0, int scale = 0)
{
    switch (type)
    {
    case 0/* constant-expression */:
        /* code */
        break;
    case 1:

    case 2:

    default:
        break;
    }
}


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
void BzExtend(float extendTime)
{
    delay(extendTime);
}

//*******************************************************
void BzStop(float stopTime)
{
    ledcWrite(PWM_CHANNEL0, 0);
    delay(stopTime);
}

//*******************************************************
void BzRest(float restTime)
{
    ledcWrite(PWM_CHANNEL0, 0);
    delay(restTime);
}





//*******************************************************
//*******************************************************
//*******************************************************
void Bz_DragonQuest_Preface0(int iBarNum)
{
    switch (iBarNum)
    {
    case 0:
        LedRighting();
        BzRing(flMScale[2][mMI], flTime[quad] + flTime[eighth] - 100);
        LedOff();
        delay(100); //LEDの為
        LedRighting();
        BzRing(flMScale[2][mMI], flTime[sixteenth]);
        LedOff();
        BzStop(flTime[sixteenth]);
        LedRighting();
        BzRing(flMScale[2][mMI], flTime[eighth]);
        LedOff();
        BzStop(flTime[eighth]);
        LedRighting();
        BzRing(flMScale[2][mRE], flTime[eighth]);
        LedOff();
        BzStop(flTime[eighth]);
        break;
    }

    //--------------
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mDO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mSO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[3][mDO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mSO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[quad] + flTime[eighth] - 100);
    LedOff();
    delay(100); //LEDの為
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[sixteenth]);
    LedOff();
    BzStop(flTime[sixteenth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[quad] + flTime[eighth] * 1.025 - 100);
    LedOff();
    delay(100); //LEDの為
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[sixteenth] * 1.05);
    LedOff();
    BzStop(flTime[sixteenth] * 1.075);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth] * 1.1);
    LedOff();
    BzStop(flTime[eighth] * 1.2);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth] * 1.4);
    LedOff();
    BzStop(flTime[eighth] * 1.75);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mDO], flTime[eighth] * 1.75);
    LedOff();
    BzStop(flTime[eighth] * 1.75);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[quad] * 2 - 100);
    LedOff();
    delay(100); //LEDの為
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[harf] * 2);
    //--------------
    BzExtend(flTime[all] * 2);
    LedOff();
}

//*******************************************************
void Bz_DragonQuest_Preface()
{
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[quad] + flTime[eighth] - 100);
    LedOff();
    BzExtend(100); //LEDの為
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[sixteenth]);
    LedOff();
    BzStop(flTime[sixteenth]);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mDO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mSO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[3][mDO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mSO], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mFA], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[quad] + flTime[eighth] - 100);
    LedOff();
    BzExtend(100); //LEDの為
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[sixteenth]);
    LedOff();
    BzStop(flTime[sixteenth]);
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[eighth]);
    LedOff();
    BzStop(flTime[eighth]);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[quad] + flTime[eighth] * 1.025 - 100);
    LedOff();
    BzExtend(100); //LEDの為
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[sixteenth] * 1.05);
    LedOff();
    BzStop(flTime[sixteenth] * 1.075);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth] * 1.1);
    LedOff();
    BzStop(flTime[eighth] * 1.2);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[eighth] * 1.4);
    LedOff();
    BzStop(flTime[eighth] * 1.75);
    //--------------
    LedRighting();
    BzRing(flMScale[2][mDO], flTime[eighth] * 1.75);
    LedOff();
    BzStop(flTime[eighth] * 1.75);
    LedRighting();
    BzRing(flMScale[2][mMI], flTime[quad] * 2 - 100);
    LedOff();
    BzExtend(100); //LEDの為
    LedRighting();
    BzRing(flMScale[2][mRE], flTime[harf] * 2);
    //--------------
    BzExtend(flTime[all] * 2);
    LedOff();
}

//*************************************************************************************
//*************************************************************************************
//*************************************************************************************

void BzGoUp(float ringTime, float stopTime)
{
    InitBz();

    for (int i = 0; i < 12; i++)
    {
        BzRing(flMScale[3][i], ringTime);
        BzStop(stopTime);
    }
}

void BzGoDown(float ringTime, float stopTime)
{
    InitBz();

    for (int i = 12; i > 0; i--)
    {
        BzRing(flMScale[3][i], ringTime);
        BzStop(stopTime);
    }
}
