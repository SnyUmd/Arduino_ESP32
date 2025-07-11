#define TIME_MAX_VALUE 4294967295//PGM開始からの時間カウント最大値

#include "Timer.hpp"
// #include "defHct.h"

//*********************************************************************************
// hw_timer_t * timer = NULL;

//*********************************************************************************
//実行方法　
// void IRAMATTR onTimer{
// }
// setup()
// {
//      setTimerInterrupt(timer, 0, onTimer);
// }
// main()
// {
//      startTimerInterrupt(timer, 1000000, false);
// }

//*********************************************************************************
// void setTimerInterrupt(hw_timer_t*& timer, int timer_num, void(* fn)(void), int count_time, bool regular)
void setTimerInterrupt(hw_timer_t*& timer, int timer_num, void(* fn)(void))
{
    timer = timerBegin(timer_num, 80, true);
    timerAttachInterrupt(timer, fn, true);
    // timerAlarmWrite(timer, count_time, regular);
}

//*********************************************************************************
void startTimerInterrupt(hw_timer_t*& timer, int count_time, bool regular)
{
    timerAlarmWrite(timer, count_time, regular);
    timerRestart(timer);
    timerAlarmEnable(timer);
}

//*********************************************************************************
void stopTimerInterrupt(hw_timer_t*& timer)
{
    timerAlarmDisable(timer);
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
        if((nowTime + (TIME_MAX_VALUE - startTime)) >= targetTime){return true;}
    }
    else
    {
        if(nowTime - startTime >= targetTime){ return true;}
        else {return false;}
    }
    return false;
}
