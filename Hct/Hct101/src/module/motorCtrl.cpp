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

void motorAction(bool bl_left, int loop_num, bool bl_food)
{
    bool blLoop = true;
    int num;
    int cnt = 0;
    int repetitionNum = 0;
    // LC.ledFlash(PORT_LED_G, 10, 5);

    int in1, in2, in3, in4;
    if(bl_food)
    {
        in1 = PORT_MOTOR1_F;
        in2 = PORT_MOTOR2_F;
        in3 = PORT_MOTOR3_F;
        in4 = PORT_MOTOR4_F;
    }
    else
    {
        in1 = PORT_MOTOR1_W;
        in2 = PORT_MOTOR2_W;
        in3 = PORT_MOTOR3_W;
        in4 = PORT_MOTOR4_W;
    }

    for(int i0 = 0; i0 < loop_num; i0++)
    {
        for(int i1 = 0; i1 < 4; i1++)
        {
            if(bl_left) num = 4 - i1 -1;
            else num = i1;
            digitalWrite(in1, aryMotorSts[num][0]);
            delay(1);
            digitalWrite(in2, aryMotorSts[num][1]);
            delay(1);
            digitalWrite(in3, aryMotorSts[num][2]);
            delay(1);
            digitalWrite(in4, aryMotorSts[num][3]);
            delay(1);
        }
    }
    digitalWrite(in1, 0);
    digitalWrite(in2, 0);
    digitalWrite(in3, 0);
    digitalWrite(in4, 0);
}
