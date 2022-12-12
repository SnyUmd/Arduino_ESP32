#include <Arduino.h>
#include "initialize.h"
#include "defHct.h"

//*************************************
void initPort()
{
    //ポート割り込み処理
    pinMode(PORT_SW, INPUT_PULLUP);

    //LEDポート
    pinMode(PORT_LED_W, OUTPUT);
    pinMode(PORT_LED_F, OUTPUT);
    digitalWrite(PORT_LED_W, LED_OFF);
    digitalWrite(PORT_LED_F, LED_OFF);

    //ステッピングモータ　ポート
    pinMode(PORT_MOTOR1_W, OUTPUT);
    pinMode(PORT_MOTOR2_W, OUTPUT);
    pinMode(PORT_MOTOR3_W, OUTPUT);
    pinMode(PORT_MOTOR4_W, OUTPUT);
    pinMode(PORT_MOTOR1_F, OUTPUT);
    pinMode(PORT_MOTOR2_F, OUTPUT);
    pinMode(PORT_MOTOR3_F, OUTPUT);
    pinMode(PORT_MOTOR4_F, OUTPUT);
    digitalWrite(PORT_MOTOR1_W, 0);
    digitalWrite(PORT_MOTOR2_W, 0);
    digitalWrite(PORT_MOTOR3_W, 0);
    digitalWrite(PORT_MOTOR4_W, 0);
    digitalWrite(PORT_MOTOR1_F, 0);
    digitalWrite(PORT_MOTOR2_F, 0);
    digitalWrite(PORT_MOTOR3_F, 0);
    digitalWrite(PORT_MOTOR4_F, 0);


}