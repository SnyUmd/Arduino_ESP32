#include <Arduino.h>

void setTimerInterrupt(void(* fn)(void), int count_time= 1000000, bool regular = true);
void stopTimerInterrupt();

// void GetTime(long& valTime);//時間取得（起動後の経過時間）
long GetTime();//時間取得（起動後の経過時間）
bool CheckElapsedTime(long startTime, long targetTime);//時間タイムアウトチェック