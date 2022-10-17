#include <Arduino.h>

// void initTimerInterrupt();
void initTimerInterrupt(void(* fn)(void));
// void timerInterruptSet(int count_time, bool regular);
void timerInterruptSet(bool bl_run, int count_time, bool regular);
void IRAM_ATTR onTimer();

// void GetTime(long& valTime);//時間取得（起動後の経過時間）
long GetTime();//時間取得（起動後の経過時間）
bool CheckElapsedTime(long startTime, long targetTime);//時間タイムアウトチェック