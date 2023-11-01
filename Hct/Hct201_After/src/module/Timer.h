#include <Arduino.h>

// void setTimerInterrupt(void(* fn)(void), int count_time= 1000000, bool regular = true);
// void setTimerInterrupt(hw_timer_t*& timer, int timer_num, void(* fn)(void), int count_time= 1000000, bool regular = true);
void setTimerInterrupt(hw_timer_t*& timer, int timer_num, void(* fn)(void));
// void startTimerInterrupt(hw_timer_t*& timer);
void startTimerInterrupt(hw_timer_t*& timer, int count_time, bool regular);
void stopTimerInterrupt(hw_timer_t* timer);

// void GetTime(long& valTime);//時間取得（起動後の経過時間）
long GetTime();//時間取得（起動後の経過時間）
bool CheckElapsedTime(long startTime, long targetTime);//時間タイムアウトチェック