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
//音階
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

//*******************************************************
//音階指定時の配列要素に使用
enum enmSound
{
    mDO = 0,    //ド
    mDOs,       //ド#
    mRE,        //レ
    mREs,       //レ#
    mMI,        //ミ
    mFA,        //ファ
    mFAs,       //ファ#
    mSO,        //ソ
    mSOs,       //ソ#
    mRA,        //ラ
    mRAs,       //ラ#
    mSI         //シ
};

//ハッカソン　文字列の音階判定用
string hctScale[] = 
{
    "do", "dos", "re", "res", "mi", "fa", 
    "fas", "so", "sos", "ra", "ras", "si"
};


//*******************************************************
//ドラクエ序章1/3
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
//ドラクエ序章2/3
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
//ドラクエ序章1/3
float aryScore2[][4] = 
{
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
    {1, getScale1(2), flSound[3][mDO], 100},
    //------------------------
    {2, getScale1(16), 0, 100},
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
    {1, getScale1(4), flSound[4][mDO], 100}
};

//*******************************************************
//ブザーを使用する為の初期設定
void InitBz()
{
    ledcSetup(PWM_CHANNEL0, PWM_FRQ1000, PWM_BIT_NUM);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
}

//*******************************************************
//音階を指定してブザー鳴動
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
//音の長さを取得
float getrScale(float iNote)
{
    return 1000/iNote;
}
//*******************************************************
//音階の長さ取得　1000/n
float getScale0(float iNote)
{
    return beat0/iNote;
}
//*******************************************************
//音階の長さ取得　2000/n
float getScale1(float iNote)
{
    return beat1/iNote;
}

//*******************************************************
//ブザー鳴動(PWM信号出力)
void BzRing(float frq, float ringTime)
{
    ledcSetup(PWM_CHANNEL0, frq, PWM_BIT_NUM);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL0);
    ledcWrite(PWM_CHANNEL0, iDuty);
    delay(ringTime);
}

//*******************************************************
//ブザー停止
void BzStop(float stopTime)
{
    ledcWrite(PWM_CHANNEL0, 0);
    delay(stopTime);
}

//*******************************************************
//*******************************************************
//*******************************************************
//ドラゴンクエスト序章
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
    
    iRow = sizeof(aryScore2) / sizeof(aryScore2[0]);
    for(int i2 = 0; i2 < iRow; i2++)
        BzNote(aryScore2[i2][0],aryScore2[i2][1],aryScore2[i2][2],aryScore2[i2][3]);

    BzStop(200);
}

//*******************************************************
//ハッカソンイベント　コマンドのテキストから音出し用
//string val = 2do4-2re4-2mi4-2fa4-2so4-2ra4-2si4-3do4;
void setHctMelody(HardwareSerial& sr, string val)
{
    string s = val;
    int splitPoint;
    int numSplit;
    numSplit = count(s.begin(), s.end(), '-');
    float aryResult[numSplit + 1][4];

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

//*******************************************************
// 例)BzGoUp(10, 10);
//低音から高温へ
//Wi-Fi接続完了音
void BzGoUp(float ringTime, float stopTime)
{
    InitBz();

    for (int i = 0; i < 12; i++)
    {
        BzRing(flSound[3][i], ringTime);
        BzStop(stopTime);
    }
}

//*******************************************************
//高温から低音へ
void BzGoDown(float ringTime, float stopTime)
{
    InitBz();

    for (int i = 12; i > 0; i--)
    {
        BzRing(flSound[3][i], ringTime);
        BzStop(stopTime);
    }
}


//*******************************************************
//受信完了音　ピコッ
void bzReceivedRing()
{
    BzNote(1, getrScale(32), flSound[0][mDO], 100);
    BzNote(0, getScale1(32), 0, 100);
    BzNote(1, getrScale(32), flSound[2][mDO], 100);
    BzNote(0, getScale1(2), 0, 100);
}

//*******************************************************
//コマンドエラー音 プップップッ
void bzErrorSound()
{
    BzNote(1, getrScale(16), flSound[0][mDO], 100);
    BzNote(0, getScale1(16), 0, 100);
    BzNote(1, getrScale(16), flSound[0][mDO], 100);
    BzNote(0, getScale1(16), 0, 100);
    BzNote(1, getrScale(16), flSound[0][mDO], 100);
    BzNote(0, getScale1(16), 0, 100);
}

//*******************************************************
//起動時音　ピッピッ
void bzPowerOn()
{
    BzNote(1, getrScale(32), flSound[4][mDO], 100);
    BzNote(0, getScale1(4), 0, 100);
    BzNote(1, getrScale(32), flSound[4][mDO], 100);
    BzNote(0, getScale1(4), 0, 100);
}
