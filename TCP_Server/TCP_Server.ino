#include "Def.h"
#include "LED_Ctrl.h"

LED_Ctrl LC;

void setup()
{
    init();
    LC.LedFlash(PORT_LED, 10, 5);
}

void loop()
{
    LC.LedFlash(PORT_LED, 100, 1);
}


void init()
{
    // LED
    pinMode(PORT_LED, OUTPUT);
    digitalWrite(PORT_LED, LED_OFF);

    // モーター
    pinMode(PORT_MOTER_IN1, OUTPUT);
    pinMode(PORT_MOTER_IN2, OUTPUT);
    pinMode(PORT_MOTER_IN3, OUTPUT);
    pinMode(PORT_MOTER_IN4, OUTPUT);
    digitalWrite(PORT_MOTER_IN1, LOW);
    digitalWrite(PORT_MOTER_IN2, LOW);
    digitalWrite(PORT_MOTER_IN3, LOW);
    digitalWrite(PORT_MOTER_IN4, LOW);
}