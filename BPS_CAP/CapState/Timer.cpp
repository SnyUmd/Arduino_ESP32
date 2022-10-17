#include "Timer.h"
#include "Def.h"
#define TIME_MAX_VALUE 4294967295//PGM開始からの時間カウント最大値

//*********************************************************************************
//時間取得（起動後の経過時間）
//valTime：時間格納変数
//*********************************************************************************
void GetTime(long& valTime)
{
    valTime = millis();
}

//*********************************************************************************
//時間タイムアウトチェック
//startTime：比較対象変数
//targetTime：タイムアウト時間
//*********************************************************************************
bool CheckElapsedTime(long startTime, long targetTime)
{
    long nowTime = millis();
    if(startTime > nowTime)
    {
        if((nowTime + (TIME_MAX_VALUE - startTime)) >= targetTime) return true;
    }
    else
        if(nowTime - startTime >= targetTime) return true;
    else
        return false;
}
