#include <Arduino.h>

void setTimerInterrupt(void(* fn)(void), int count_time, bool regular);
void stopTimerInterrupt();
void IRAM_ATTR onTimer();

// void GetTime(long& valTime);//時間取得（起動後の経過時間）
long GetTime();//時間取得（起動後の経過時間）
bool CheckElapsedTime(long startTime, long targetTime);//時間タイムアウトチェック