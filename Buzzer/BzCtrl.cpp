#include "BzCtrl.h"
#include "LedCtrl.h"

int iDuty = PWM_RESOLUTION / 2;
//extern float flSount[][12];

float beat0 = 1000;
float beat1 = 2000;


//*******************************************************
float flSount[][12] =
    {
        {130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220, 233.082, 246.942},
        {261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883},
        {523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880, 932.328, 987.767},
        {1046.502, 1108.731, 1174.659, 1244.508, 1318.51, 1396.913, 1479.978, 1567.982, 1661.219, 1760, 1864.655, 1975.533},
        {2093.005, 2217.461, 2349.318, 2489.016, 2637.02, 2793.826, 2959.955, 3135.963, 3322.438, 3520, 3729.31, 3951.066},
        {4186.009, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        /*　0ド , 1ド# , 2レ, 3レ#, 4ミ , 5ファ , 6ファ# , 7ソ , 8ソ# , 9ラ , 10ラ# , 11シ　*/
};

// //*******************************************************
// enum enmMelodyNote
// {
//     all = 0,
//     harf,
//     quarter,
//     eighth,
//     sixteenth
// };

// float flTime[] = {beat, beat / 2, beat / 4, beat / 8, beat / 16};

//*******************************************************

enum enmSound
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
float aryScore0[][4] = 
{//{記号種, 記号, 音, ペース}
//記号
//0：休符//1：音符//2：伸ばし
        {1, getScale0(4), flSount[2][mMI], 100},
        {2, getScale0(8), 0, 100},
        {1, getScale0(16), flSount[2][mMI], 100},
        {0, getScale0(16), 0, 100},
        {1, getScale0(8), flSount[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSount[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mDO], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSount[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mFA], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSount[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mFA], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mSO], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mRA], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSount[3][mDO], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mRA], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mSO], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSount[2][mFA], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSount[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(4), flSount[2][mRE], 100},
        {2, getScale0(8), 0, 100},
        {1, getScale0(16), flSount[2][mRE], 100},
        {0, getScale0(16), 0, 100},
        {1, getScale0(8), flSount[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(4), flSount[2][mMI], 110},
        {2, getScale0(8), 0, 110},
        {1, getScale0(16), flSount[2][mMI], 125},
        {0, getScale0(16), 0, 125},
        {1, getScale0(8), flSount[2][mMI], 125},
        {0, getScale0(8), 0, 150},
        {1, getScale0(8), flSount[2][mMI], 150},
        {0, getScale0(8), 0, 150},
        //---------------------------------------
        {1, getScale0(8), flSount[2][mDO], 180},
        {0, getScale0(8), 0, 180},
        {1, getScale0(4), flSount[2][mMI], 180},
        {1, getScale0(2), flSount[2][mRE], 200},
        {2, getScale0(1), 0, 200},
        //---------------------------------------
        {0, getScale0(1), 0, 200}
};

float aryScore1[][4] = 
{
    {1, getScale1(8), flSount[2][mSO], 100},
    {2, getScale1(16), 0, 100},
    {1, getScale1(16), flSount[2][mSO], 100},
    {1, getScale1(4), flSount[3][mDO], 100},
    {1, getScale1(4), flSount[3][mRE], 100},
    //------------------------
    {1, getScale1(4), flSount[3][mMI], 100},
    {1, getScale1(4), flSount[3][mFA], 100},
    {1, getScale1(8), flSount[3][mSO], 100},
    {2, getScale1(16), 0, 100},//楽譜では無し
    {0, getScale1(16), 0, 100},//楽譜では8分音符
    {1, getScale1(4), flSount[4][mDO], 100},
    ////------------------------
    {2, getScale1(4), 0, 100},
    {1, getScale1(8), flSount[3][mSI], 100},
    {2, getScale1(16), 0, 100},
    {1, getScale1(16), flSount[3][mRA], 100},
    {1, getScale1(4), flSount[3][mRA], 100},
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSount[3][mSO], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(16), flSount[3][mFAs], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(8), flSount[3][mFAs], 100},//楽譜ではファ
    {1, getScale1(8), flSount[3][mRA], 100},
    {1, getScale1(4), flSount[3][mSO], 100},
    {1, getScale1(4), flSount[3][mMI], 100},
    //------------------------
    {2, getScale1(4), 0, 100},
    {1, getScale1(8), flSount[2][mMI], 100},
    {2, getScale1(16), 0, 100},
    {1, getScale1(16), flSount[2][mMI], 100},
    {1, getScale1(4), flSount[2][mMI], 100},
    {1, getScale1(4), flSount[2][mMI], 100},
    //------------------------
    {1, getScale1(4), flSount[2][mFAs], 100},
    {1, getScale1(4), flSount[2][mSOs], 100},
    {1, getScale1(2), flSount[2][mRA], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(16), flSount[2][mRA], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(16), flSount[2][mSI], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(16), flSount[3][mDO], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(2), flSount[3][mRE], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(16), flSount[2][mRA], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(16), flSount[2][mRA], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(16), flSount[3][mRE], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(4), flSount[3][mRE], 100},
    {1, getScale1(4), flSount[2][mSI], 100},
    //------------------------
    {1, getScale1(4), flSount[2][mRA], 100},
    {1, getScale1(4), flSount[2][mSO], 100},
    {1, getScale1(2), flSount[3][mMI], 100}
};
float aryScore2[][4] = 
{
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSount[3][mFA], 100},
    {1, getScale1(8), flSount[3][mMI], 100},
    {1, getScale1(8), flSount[3][mRE], 100},
    {1, getScale1(2), flSount[3][mDO], 100},
    //------------------------
    {1, getScale1(4), flSount[2][mRA], 100},
    {1, getScale1(4), flSount[3][mDO], 100},
    {1, getScale1(2), flSount[3][mRE], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSount[3][mMI], 100},
    {1, getScale1(8), flSount[3][mRE], 100},
    {1, getScale1(8), flSount[3][mDO], 100},
    {1, getScale1(2), flSount[3][mDO], 100},
    //------------------------
    {1, getScale1(4), flSount[2][mSI], 100},
    {1, getScale1(4), flSount[2][mSO], 100},
    {1, getScale1(2), flSount[3][mSO], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSount[3][mMI], 100},
    {1, getScale1(8), flSount[3][mFA], 100},
    {1, getScale1(8), flSount[3][mSO], 100},
    {1, getScale1(2), flSount[3][mRA], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSount[2][mRA], 100},
    {1, getScale1(8), flSount[2][mSI], 100},
    {1, getScale1(8), flSount[3][mDO], 100},
    {1, getScale1(2), flSount[3][mFA], 100},
    //------------------------
    {1, getScale1(2), flSount[3][mMI], 100},
    {1, getScale1(2), flSount[3][mDO], 100},
    //------------------------
    {2, getScale1(16), 0, 100},
    {0, getScale1(16), 0, 100},
    {0, getScale1(8), 0, 100},
    {1, getScale1(8), flSount[2][mSO], 100},
    {2, getScale1(16), 0, 100},
    {1, getScale1(16), flSount[2][mSO], 100},
    {1, getScale1(4), flSount[3][mDO], 100},
    {1, getScale1(4), flSount[3][mRE], 100},
    //------------------------
    {1, getScale1(4), flSount[3][mMI], 100},
    {1, getScale1(4), flSount[3][mFA], 100},
    {1, getScale1(8), flSount[3][mSO], 100},
    {0, getScale1(8), 0, 100},
    {1, getScale1(4), flSount[4][mDO], 100}
};

void BzNote(float type, float iTime, float scale, float iPase)
{
    float iPaseBuf = iPase / 100;
    switch (int(type))
    {
    case 0:
        ledcWrite(PWM_CHANNEL0, 0);
        delay(iTime * iPaseBuf);
        break;
    case 1:
        ledcSetup(PWM_CHANNEL0, scale, PWM_BIT_NUM);
        ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
        ledcWrite(PWM_CHANNEL0, iDuty);
        delay(iTime * iPaseBuf);
        break;
    case 2:
        delay(iTime * iPaseBuf);
        break;
    }
}

float getScale0(float iNote)
{
    return beat0/iNote;
}
float getScale1(float iNote)
{
    return beat1/iNote;
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
    
}

//*******************************************************
void Bz_DragonQuest_Preface()
{
    int iRow = sizeof(aryScore0) / sizeof(aryScore0[0]);
    int iCol = sizeof(aryScore0[0]) / sizeof(aryScore0[0][0]);

    for(int i0 = 0; i0 < iRow; i0++)
       BzNote(aryScore0[i0][0],aryScore0[i0][1],aryScore0[i0][2],aryScore0[i0][3]);
    
    iRow = sizeof(aryScore1) / sizeof(aryScore1[0]);
    for(int i1 = 0; i1 < iRow; i1++)
      BzNote(aryScore1[i1][0],aryScore1[i1][1],aryScore1[i1][2],aryScore1[i1][3]);
    
    iRow = sizeof(aryScore2) / sizeof(aryScore2[0]);
    for(int i2 = 0; i2 < iRow; i2++)
        BzNote(aryScore2[i2][0],aryScore2[i2][1],aryScore2[i2][2],aryScore2[i2][3]);


}

//*************************************************************************************
//*************************************************************************************
//*************************************************************************************

void BzGoUp(float ringTime, float stopTime)
{
    InitBz();

    for (int i = 0; i < 12; i++)
    {
        BzRing(flSount[3][i], ringTime);
        BzStop(stopTime);
    }
}

void BzGoDown(float ringTime, float stopTime)
{
    InitBz();

    for (int i = 12; i > 0; i--)
    {
        BzRing(flSount[3][i], ringTime);
        BzStop(stopTime);
    }
}
