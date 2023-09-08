#include <Arduino.h>

/// @brief 割り込みセット
/// @param port_num 割り込みポート番号
/// @param fn 割り込み実行関数
/// @param mode FALLING 又は FISING
void initInterrupt(uint8_t  port_num, void(*fn)(void), int mode)
{
    attachInterrupt(port_num, fn, mode);
}


///割り込みにセットする関数の例
// void ex_interrupt_fn()
// {
//     attachInterrupt(port_num, NULL, mode);//チャタリング防止の為、割り込み停止
//     //割り込み処理--------------------------
//     // ------------------------------------
//     //割り込みセット
//     attachInterrupt(port_num, ex_interrupt_fn, mode);//割り込みセット
// }