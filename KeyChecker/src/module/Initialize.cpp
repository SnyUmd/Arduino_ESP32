#include <Arduino.h>
#include "defKeyChecker.h"

void initPort()
{
    pinMode(LED_INTERNAL, OUTPUT);
    pinMode(SW_INTERNAL, INPUT_PULLUP);

    digitalWrite(LED_INTERNAL, LED_OFF);
}

void initFunction()
{
    
}