#include <Arduino.h>
#include "init.h"
#include "def.h"
 
float flMelody[][12] =
{
    {261.626,
     277.183,
     293.665,
     311.127,
     329.628,
     349.228,
     369.994,
     391.995,
     415.305,
     440,
     466.164,
     493.883
     }
};

float GetMelody(int row, int col)
{
    return flMelody[row][col];
}

//test = 0;

void Initialize_Port()
{
    

    //LED
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GRE, OUTPUT);

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GRE, HIGH);

    //スポットライト
    //pinMode(SPOT_LIGHT, OUTPUT);
    //digitalWrite(SPOT_LIGHT, LOW);

    //人感センサ
    //pinMode(HUMAN_SENSOR, INPUT);
    //pinMode(15, INPUT);

    //ブザー
    //pinMode(BUZZER_PWM, OUTPUT);
    //digitalWrite(BUZZER_PWM, LOW);

    //タクトスイッチ
    pinMode(SWITCH0, INPUT_PULLUP);
}