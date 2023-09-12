#include <Arduino.h>
#include "defKeyChecker.h"

void initPort()
{
    pinMode(P_LED_INTERNAL, OUTPUT);
    pinMode(P_SW_INTERNAL, INPUT_PULLUP);
    pinMode(P_BUZZER, OUTPUT);
    pinMode(P_APDS9930_INT, INPUT);

    digitalWrite(P_LED_INTERNAL, LED_OFF);
    digitalWrite(P_BUZZER, LOW);
}

void initFunction()
{
    
}