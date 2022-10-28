#include <Arduino.h>
#include "defHct.h"
#include "motorCtrl.h"

const int aryMotorSts[4][4] = 
{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

void motorAction(bool bl_left, int loop_num)
{
    bool blLoop = true;
    int num;
    int cnt = 0;
    int repetitionNum = 0;
    // LC.ledFlash(PORT_LED_G, 10, 5);

    for(int i0 = 0; i0 < loop_num; i0++)
    {
        for(int i1 = 0; i1 < 4; i1++)
        {
            if(bl_left) num = i1;
            else num = 4 - i1 -1;
            digitalWrite(PORT_MOTOR0, aryMotorSts[num][0]);
            delay(1);
            digitalWrite(PORT_MOTOR1, aryMotorSts[num][1]);
            delay(1);
            digitalWrite(PORT_MOTOR2, aryMotorSts[num][2]);
            delay(1);
            digitalWrite(PORT_MOTOR3, aryMotorSts[num][3]);
            delay(1);
        }
    }
}

