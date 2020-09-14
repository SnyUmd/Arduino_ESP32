#include <Arduino.h>
#include "init.h"
#include "def.h"

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