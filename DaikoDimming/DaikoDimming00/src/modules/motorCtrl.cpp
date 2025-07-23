#include <Arduino.h>
#include "def.hpp"
#include "motorCtrl.hpp"
#include "Timer.hpp"

const int aryMotorSts[4][4] = 
{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

//**********************************************************************
void motorAction(bool is_dark, int loop_num)
{
    // bool blLoop = true;
    int num;
    // int cnt = 0;
    // int repetitionNum = 0;
    // LC.ledFlash(PORT_LED_G, 10, 5);

    int in1, in2, in3, in4;

    for(int i0 = 0; i0 < loop_num; i0++)
    {
        for(int i1 = 0; i1 < 4; i1++)
        {
            if(is_dark) num = 4 - i1 -1;
            else num = i1;
            digitalWrite(PORT_MORTER_0, aryMotorSts[num][0]);
            delay(1);
            digitalWrite(PORT_MORTER_1, aryMotorSts[num][1]);
            delay(1);
            digitalWrite(PORT_MORTER_2, aryMotorSts[num][2]);
            delay(1);
            digitalWrite(PORT_MORTER_3, aryMotorSts[num][3]);
            delay(1);
        }
    }
    digitalWrite(PORT_MORTER_0, 0);
    digitalWrite(PORT_MORTER_1, 0);
    digitalWrite(PORT_MORTER_2, 0);
    digitalWrite(PORT_MORTER_3, 0);
}

