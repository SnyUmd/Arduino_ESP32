#include "BzCtrl.h"
#include <iostream>
#include <string>
// #include <vector>
#include <algorithm>
// #include "LedCtrl.h"


int iDuty = PWM_RESOLUTION / 2;
//extern float flSound[][12];

float beat0 = 1000;
float beat1 = 2000;


//*******************************************************
float flSound[][12] =
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

string hctScale[] = 
{
    "do", "dos", "re", "res", "mi", "fa", 
    "fas", "so", "sos", "ra", "ras", "si"
};


//*******************************************************
float aryScore0[][4] = 
{//{記号種, 記号, 音, ペース}
//記号
//0：休符//1：音符//2：伸ばし
        {1, getScale0(4), flSound[2][mMI], 100},
        {2, getScale0(8), 0, 100},
        {1, getScale0(16), flSound[2][mMI], 100},
        {0, getScale0(16), 0, 100},
        {1, getScale0(8), flSound[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSound[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mDO], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSound[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mFA], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSound[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mFA], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mSO], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mRA], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSound[3][mDO], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mRA], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mSO], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(8), flSound[2][mFA], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(8), flSound[2][mMI], 100},
        {0, getScale0(8), 0, 100},
        {1, getScale0(4), flSound[2][mRE], 100},
        {2, getScale0(8), 0, 100},
        {1, getScale0(16), flSound[2][mRE], 100},
        {0, getScale0(16), 0, 100},
        {1, getScale0(8), flSound[2][mRE], 100},
        {0, getScale0(8), 0, 100},
        //---------------------------------------
        {1, getScale0(4), flSound[2][mMI], 110},
        {2, getScale0(8), 0, 110},
        {1, getScale0(16), flSound[2][mMI], 125},
        {0, getScale0(16), 0, 125},
        {1, getScale0(8), flSound[2][mMI], 125},
        {0, getScale0(8), 0, 150},
        {1, getScale0(8), flSound[2][mMI], 150},
        {0, getScale0(8), 0, 150},
        //---------------------------------------
        {1, getScale0(8), flSound[2][mDO], 180},
        {0, getScale0(8), 0, 180},
        {1, getScale0(4), flSound[2][mMI], 180},
        {1, getScale0(2), flSound[2][mRE], 200},
        {2, getScale0(1), 0, 200},
        //---------------------------------------
        {0, getScale0(1), 0, 200}
};

//*******************************************************
float aryScore1[][4] = 
{
    {1, getScale1(8), flSound[2][mSO], 100},
    {2, getScale1(16), 0, 100},
    {1, getScale1(16), flSound[2][mSO], 100},
    {1, getScale1(4), flSound[3][mDO], 100},
    {1, getScale1(4), flSound[3][mRE], 100},
    //------------------------
    {1, getScale1(4), flSound[3][mMI], 100},
    {1, getScale1(4), flSound[3][mFA], 100},
    {1, getScale1(8), flSound[3][mSO], 100},
    {2, getScale1(16), 0, 100},//楽譜では無し
    {0, getScale1(16), 0, 100},//楽譜では8分音符
    {1, getScale1(4), flSound[4][mDO], 100},
    ////------------------------
    {2, getScale1(4), 0, 100},
    {1, getScale1(8), flSound[3][mSI], 100},
    {2, getScale1(16), 0, 100},
    {1, getScale1(16), flSound[3][mRA], 100},
    {1, getScale1(4), flSound[3][mRA], 100},
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[3][mSO], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(16), flSound[3][mFAs], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(8), flSound[3][mFAs], 100},//楽譜ではファ
    {1, getScale1(8), flSound[3][mRA], 100},
    {1, getScale1(4), flSound[3][mSO], 100},
    {1, getScale1(4), flSound[3][mMI], 100},
    //------------------------
    {2, getScale1(4), 0, 100},
    {1, getScale1(8), flSound[2][mMI], 100},
    {2, getScale1(16), 0, 100},
    {1, getScale1(16), flSound[2][mMI], 100},
    {1, getScale1(4), flSound[2][mMI], 100},
    {1, getScale1(4), flSound[2][mMI], 100},
    //------------------------
    {1, getScale1(4), flSound[2][mFAs], 100},
    {1, getScale1(4), flSound[2][mSOs], 100},
    {1, getScale1(2), flSound[2][mRA], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(16), flSound[2][mRA], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(16), flSound[2][mSI], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(16), flSound[3][mDO], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(2), flSound[3][mRE], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(16), flSound[2][mRA], 100},
    {0, getScale1(16), 0, 100},
    {1, getScale1(16), flSound[2][mRA], 100},//**
    {0, getScale1(16), 0, 100},
    // {1, getScale1(16), flSound[3][mRE], 100},
    {1, getScale1(16), flSound[3][mDO], 100},
    {0, getScale1(16), 0, 100},
    // {1, getScale1(4), flSound[3][mRE], 100},
    {1, getScale1(4), flSound[3][mDO], 100},
    {1, getScale1(4), flSound[2][mSI], 100},
    //------------------------
    {1, getScale1(4), flSound[2][mRA], 100},
    {1, getScale1(4), flSound[2][mSO], 100},
    {1, getScale1(2), flSound[3][mMI], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[3][mFA], 100},
    {1, getScale1(8), flSound[3][mMI], 100},
    {1, getScale1(8), flSound[3][mRE], 100},
    {1, getScale1(2), flSound[3][mDO], 100},
    //------------------------
    {1, getScale1(4), flSound[2][mRA], 100},
    {1, getScale1(4), flSound[3][mDO], 100},
    {1, getScale1(2), flSound[3][mRE], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[3][mMI], 100},
    {1, getScale1(8), flSound[3][mRE], 100},
    {1, getScale1(8), flSound[3][mDO], 100},
    {1, getScale1(2), flSound[3][mDO], 100},
    //------------------------
    {1, getScale1(4), flSound[2][mSI], 100},
    {1, getScale1(4), flSound[2][mSO], 100},
    {1, getScale1(2), flSound[3][mSO], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[3][mMI], 100},
    {1, getScale1(8), flSound[3][mFA], 100},
    {1, getScale1(8), flSound[3][mSO], 100},
    {1, getScale1(2), flSound[3][mRA], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[2][mRA], 100},
    {1, getScale1(8), flSound[2][mSI], 100},
    {1, getScale1(8), flSound[3][mDO], 100},
    {1, getScale1(2), flSound[3][mFA], 100},
    //------------------------
    {1, getScale1(2), flSound[3][mMI], 100},
    {1, getScale1(2), flSound[3][mDO], 100}
};
//*******************************************************
float aryScore2[][4] = 
{
    /*{2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[3][mFA], 100},
    {1, getScale1(8), flSound[3][mMI], 100},
    {1, getScale1(8), flSound[3][mRE], 100},
    {1, getScale1(2), flSound[3][mDO], 100},
    //------------------------
    {1, getScale1(4), flSound[2][mRA], 100},
    {1, getScale1(4), flSound[3][mDO], 100},
    {1, getScale1(2), flSound[3][mRE], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[3][mMI], 100},
    {1, getScale1(8), flSound[3][mRE], 100},
    {1, getScale1(8), flSound[3][mDO], 100},
    {1, getScale1(2), flSound[3][mDO], 100},
    //------------------------
    {1, getScale1(4), flSound[2][mSI], 100},
    {1, getScale1(4), flSound[2][mSO], 100},
    {1, getScale1(2), flSound[3][mSO], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[3][mMI], 100},
    {1, getScale1(8), flSound[3][mFA], 100},
    {1, getScale1(8), flSound[3][mSO], 100},
    {1, getScale1(2), flSound[3][mRA], 100},
    //------------------------
    {2, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[2][mRA], 100},
    {1, getScale1(8), flSound[2][mSI], 100},
    {1, getScale1(8), flSound[3][mDO], 100},
    {1, getScale1(2), flSound[3][mFA], 100},
    //------------------------
    {1, getScale1(2), flSound[3][mMI], 100},
    {1, getScale1(2), flSound[3][mDO], 100},*/
    //------------------------
    /*{2, getScale1(16), 0, 100},
    {0, getScale1(16), 0, 100},
    {0, getScale1(8), 0, 100},
    {1, getScale1(8), flSound[2][mSO], 100},
    {2, getScale1(16), 0, 100},
    {1, getScale1(16), flSound[2][mSO], 100},
    {1, getScale1(4), flSound[3][mDO], 100},
    {1, getScale1(4), flSound[3][mRE], 100},
    //------------------------
    {1, getScale1(4), flSound[3][mMI], 100},
    {1, getScale1(4), flSound[3][mFA], 100},
    {1, getScale1(8), flSound[3][mSO], 100},
    {0, getScale1(8), 0, 100},
    {1, getScale1(4), flSound[4][mDO], 100}*/
};

//*******************************************************
void InitBz()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ1000, PWM_BIT_NUM);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
    //ledcWrite(PWM_CHANNEL0, PWM_DUTY);
}

//*******************************************************
void BzNote(float type, float iTime, float scale, float iPase)
//type 0:給付,　1：音符,　2：伸ばし
//iTime：時間
//scale：音階
//iPase：長さ微調整
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
//*******************************************************
float getrScale(float iNote)
{
    return 1000/iNote;
}
//*******************************************************
float getScale0(float iNote)
{
    return beat0/iNote;
}
//*******************************************************
float getScale1(float iNote)
{
    return beat1/iNote;
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
void BzTest()
{
    int ch1 = 0;
    int ch2 = 2;
    int ch3 = 4;
    int ch4 = 6;
    int ch5 = 8;
    int ch6 = 10;
    int ch7 = 12;
    int ch8 = 14;

    int bit = 2;
    float rsl = 4;

    ledcSetup(ch1, 1000, bit);
    ledcWrite(ch1, rsl/2);
    ledcAttachPin(25, ch1);
    delay(1000);
    //ledcWrite(ch1, 0);

    ledcSetup(ch2, 2000, bit);
    ledcWrite(ch2, rsl/2);
    ledcAttachPin(26, ch2);
    delay(1000);
    //ledcWrite(ch2, 0);

    ledcSetup(ch3, 3000, bit);
    ledcWrite(ch3, rsl/2);
    ledcAttachPin(27, ch3);
    delay(1000);
    //ledcWrite(ch3, 0);

    ledcSetup(ch4, 4000, bit);
    ledcWrite(ch4, rsl/2);
    ledcAttachPin(14, ch4);
    delay(1000);




    ledcSetup(ch5, 5000, bit);
    ledcWrite(ch5, rsl/2);
    ledcAttachPin(12, ch5);
    delay(1000);

    ledcSetup(ch6, 6000, bit);
    ledcWrite(ch6, rsl/2);
    ledcAttachPin(13, ch6);
    delay(1000);

    ledcSetup(ch7, 7000, bit);
    ledcWrite(ch7, rsl/2);
    ledcAttachPin(33, ch7);
    delay(1000);

    ledcSetup(ch8, 8000, bit);
    ledcWrite(ch8, rsl/2);
    ledcAttachPin(32, ch8);
    delay(1000);

//-----------------

    // ledcSetup(ch1, flSound[2][mDO], PWM_BIT_NUM);
    // ledcWrite(ch1, iDuty);
    // ledcAttachPin(25, ch1);
    // delay(1000);

    // ledcSetup(ch2, flSound[2][mMI], PWM_BIT_NUM);
    // ledcWrite(ch2, iDuty);
    // ledcAttachPin(26, ch2);
    // delay(1000);

    // ledcSetup(ch3, flSound[2][mSO], PWM_BIT_NUM);
    // ledcWrite(ch3, iDuty);
    // ledcAttachPin(27, ch3);
    // delay(1000);

    // ledcSetup(ch4, flSound[3][mDO], PWM_BIT_NUM);
    // ledcWrite(ch4, iDuty);
    // ledcAttachPin(14, ch4);
    // delay(1000);

    ledcWrite(ch8, 0);
    delay(1000);
    ledcWrite(ch7, 0);
    delay(1000);
    ledcWrite(ch6, 0);
    delay(1000);
    ledcWrite(ch5, 0);
    delay(1000);
    ledcWrite(ch4, 0);
    delay(1000);
    ledcWrite(ch3, 0);
    delay(1000);
    ledcWrite(ch2, 0);
    delay(1000);
    ledcWrite(ch1, 0);
    delay(1000);

}

void BzTest2()
{
    float flq = 1;     //周波数
    int bit = 8;            //ビット
    float rsl = 256;         //分解能
    int prt = 25;           //ポート番号
    int pwm_ch = 0;         //PWMチャンネル

    ledcSetup(pwm_ch, flq, bit);
    ledcWrite(pwm_ch, rsl/2);
    ledcAttachPin(prt, pwm_ch);
    delay(1000);
    //ledcWrite(pwm_ch, rsl/4);
    ledcSetup(pwm_ch, 10, bit);
    ledcWrite(pwm_ch, rsl/2);
    //ledcAttachPin(prt, pwm_ch);
    delay(1000);
    //ledcWrite(pwm_ch, rsl/8);
    ledcSetup(pwm_ch, 20, bit);
    ledcWrite(pwm_ch, rsl/2);
    //ledcAttachPin(prt, pwm_ch);

    //ledcWrite(ch2, 0);
}
//*******************************************************
void Bz_DragonQuest_Preface()
{
    int iRow = sizeof(aryScore0) / sizeof(aryScore0[0]);
    int iCol = sizeof(aryScore0[0]) / sizeof(aryScore0[0][0]);

    for(int i0 = 0; i0 < iRow; i0++)
       BzNote(aryScore0[i0][0],aryScore0[i0][1],aryScore0[i0][2],aryScore0[i0][3]);
    
    if (digitalRead(SWITCH0) == 0)
    {
        BzStop(200);
        return;
    }
    iRow = sizeof(aryScore1) / sizeof(aryScore1[0]);
    for(int i1 = 0; i1 < iRow; i1++)
        BzNote(aryScore1[i1][0],aryScore1[i1][1],aryScore1[i1][2],aryScore1[i1][3]);
    
    // iRow = sizeof(aryScore2) / sizeof(aryScore2[0]);
    // for(int i2 = 0; i2 < iRow; i2++)
    //     BzNote(aryScore2[i2][0],aryScore2[i2][1],aryScore2[i2][2],aryScore2[i2][3]);

    BzStop(200);
}

//ハッカソンイベント　音出し用
//string val = 2do4-2re4-2mi4-2fa4-2so4-2ra4-2si4-3do4;
void setHctMelody(HardwareSerial& sr, string val)
{
    string s = val;
    int splitPoint;
    int numSplit;
    numSplit = count(s.begin(), s.end(), '-');
    float aryResult[numSplit + 1][4];
    // float hctMelody[100][4] = {};

    // map<string, int> hctScale = {{"do", 0}, {"re", 1}};
    // map<int, string> hctScale = {{0, "do"}, {1, "re"}};

    sr.print("split : ");
    sr.println(numSplit);
    for(int i = 0; i <= numSplit; i++)
    {
        bool blRest = false;
        int position;
        int floor;
        int symbol;
        string buf;
        string musicalScale;
        splitPoint = s.find("-");//区切り文字の位置を取得
        // splitPoint = s.find(".");//区切り文字の位置を取得
        
        buf = s.substr(0, splitPoint);//文字抽出
        s.erase(0, splitPoint + 1);//文字削除
        sr.println(buf.c_str());
        int restPosition = buf.find("rest");
        sr.print("rest find：");
        sr.println(restPosition);
        if(restPosition >= 0 && restPosition < 10)
        {
            blRest = true;
            aryResult[i][0] = 0;
            floor = 0;
            buf.erase(0, 4);
            aryResult[i][2] = 0;
        }
        else
        {
            aryResult[i][0] = 1;
            floor = atoi(buf.substr(0, 1).c_str());
            buf.erase(0, 1);
            musicalScale = buf.substr(0, 2);
            buf.erase(0, 2);
            const char *chrNext = buf.substr(0, 1).c_str();
            sr.print("chrNext：");
            sr.println(*chrNext);
            if(*chrNext < '0' || *chrNext > '9')
            {
                sr.println("#認識");
                musicalScale += buf.substr(0, 1);
                buf.erase(0, 1);
            }
            for(int ii = 0; ii < 12; ii++)
            {
                if(hctScale[ii] == musicalScale)
                {
                    aryResult[i][2] = flSound[floor][ii];
                    break;
                }
            }
        }
        symbol = atoi(buf.c_str());
        aryResult[i][1] = getScale0(symbol);
        aryResult[i][3] = 100;

        sr.println(aryResult[i][0]);
        sr.println(aryResult[i][1]);
        sr.println(aryResult[i][2]);
        sr.println(aryResult[i][3]);
        sr.println("-----");
    }

    // int iRow = sizeof(aryResult)/sizeof(*aryResult);
    for(int i0 = 0; i0 <= numSplit; i0++)
    {
        if( aryResult[i0][0] == 0 && 
            aryResult[i0][1] == 0 && 
            aryResult[i0][2] == 0 && 
            aryResult[i0][3] == 0) {break;}
       BzNote(aryResult[i0][0],aryResult[i0][1],aryResult[i0][2],aryResult[i0][3]);
    }

    BzStop(200);
}

//*************************************************************************************
//*************************************************************************************
//*************************************************************************************

// 仕様例)BzGoUp(10, 10);
void BzGoUp(float ringTime, float stopTime)
{
    InitBz();

    for (int i = 0; i < 12; i++)
    {
        BzRing(flSound[3][i], ringTime);
        BzStop(stopTime);
    }
}

void BzGoDown(float ringTime, float stopTime)
{
    InitBz();

    for (int i = 12; i > 0; i--)
    {
        BzRing(flSound[3][i], ringTime);
        BzStop(stopTime);
    }
}

void bz(int num)
{
    for(int i = 0; i < num; i++)
    {
        BzNote(1, getrScale(16), flSound[4][mDO], 50);
        BzStop(1);
        // BzNote(0, getScale1(16), 0, 100);
    }
    // BzNote(1, getrScale(8), flSound[2][mDO], 50);
    // BzNote(0, getScale1(8), 0, 50);
    // BzNote(1, getrScale(8), flSound[2][mDO], 50);
    // BzNote(0, getScale1(8), 0, 50);
    // BzNote(1, getrScale(8), flSound[2][mDO], 50);
    // BzNote(0, getScale1(8), 0, 50);
    // BzNote(1, getrScale(8), flSound[2][mDO], 50);
    BzStop(200);
}

void bzReceivedRing()
{
    BzNote(1, getrScale(32), flSound[0][mDO], 100);
    BzNote(0, getScale1(32), 0, 100);
    BzNote(1, getrScale(32), flSound[2][mDO], 100);
    BzNote(0, getScale1(2), 0, 100);
}

void bzErrorSound()
{
    BzNote(1, getrScale(16), flSound[0][mDO], 100);
    BzNote(0, getScale1(16), 0, 100);
    BzNote(1, getrScale(16), flSound[0][mDO], 100);
    BzNote(0, getScale1(16), 0, 100);
    BzNote(1, getrScale(16), flSound[0][mDO], 100);
    BzNote(0, getScale1(16), 0, 100);
    // BzStop(200);
    // BzNote(0, getScale1(16), 0, 100);
}

void bzPowerOn()
{
    BzNote(1, getrScale(32), flSound[4][mDO], 100);
    BzNote(0, getScale1(4), 0, 100);
    BzNote(1, getrScale(32), flSound[4][mDO], 100);
    BzNote(0, getScale1(4), 0, 100);
}


void prgBug()
{
    BzNote(1, getrScale(1), flSound[0][mDO], 100);
    BzStop(1);
}

void bzWiFiError()
{
    BzNote(1, getrScale(8), flSound[0][mDO], 100);
    BzNote(0, getScale1(8), 0, 100);
    BzNote(1, getrScale(8), flSound[0][mDO], 100);
    BzNote(0, getScale1(8), 0, 100);
    BzNote(1, getrScale(8), flSound[0][mDO], 100);
    BzNote(0, getScale1(8), 0, 100);
}

void bzModeChange(int modeNum)
{
    if(modeNum == 0)
    {
        BzGoUp(10, 10);
    }
    else
    {
        BzNote(1, getrScale(32), flSound[4][mDO], 100);
        BzNote(0, getScale1(32), 0, 100);
        BzNote(1, getrScale(32), flSound[4][mDO], 100);
        BzNote(0, getScale1(32), 0, 100);
        BzNote(1, getrScale(32), flSound[4][mDO], 100);
        BzNote(0, getScale1(32), 0, 100);
    }
    // BzNote(1, getrScale(32), flSound[4][mDO], 100);
    // BzNote(0, getScale1(32), 0, 100);
    // BzNote(1, getrScale(32), flSound[4][mDO], 100);
    // BzNote(0, getScale1(32), 0, 100);
    // BzNote(1, getrScale(32), flSound[4][mDO], 100);
    // BzNote(0, getScale1(32), 0, 100);
}



