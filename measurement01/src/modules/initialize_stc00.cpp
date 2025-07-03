#include <Arduino.h>
#include "initialize_stc00.hpp"
#include "def_stc00.hpp"

//*************************************
void initPort()
{
    //ポート割り込み処理
    pinMode(PORT_SW, INPUT_PULLUP);
    pinMode(PORT_LED, OUTPUT);

    digitalWrite(PORT_LED, LED_OFF);


    //debug-----------------------------
    pinMode(PORT_ACCESS, OUTPUT);
    digitalWrite(PORT_ACCESS, HIGH);
    pinMode(PORT_SW_DEBUG, INPUT_PULLUP);
    //^--------------------
}