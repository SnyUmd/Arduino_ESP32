#define TIME_MAX_VALUE 4294967295//PGM開始からの時間カウント最大値

#include "Timer.h"
#include "Def.h"

//*********************************************************************************
hw_timer_t * timer = NULL;
// void initTimerInterrupt()
// {
//     timer = timerBegin(0, 80, true);
//     timerAttachInterrupt(timer, &onTimer0, true);
// }
//-------------------------------------------------
//実行方法　
// void IRAMATTR onTimer{
// }
// initTimerInterrupt(&onTimer);
// 
void initTimerInterrupt(void(* fn)(void))
{
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, fn, true);
}

//*********************************************************************************
void timerInterruptSet(bool bl_run, int count_time = 1000000, bool regular = true)
{
    if(bl_run)
    {
        timerAlarmWrite(timer, count_time, regular);
        timerAlarmEnable(timer);
    }
    else
        timerAlarmDisable(timer);
}

//*********************************************************************************
void IRAM_ATTR onTimer0()
{
    Serial.println(GetTime());
    // timerInterruptSet(false);
}

//*********************************************************************************
//時間取得（起動後の経過時間）
//valTime：時間格納変数
//*********************************************************************************
// long GetTime(long& valTime)
long GetTime()
{
    return millis();
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



