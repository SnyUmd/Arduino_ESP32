#include "def.h"
#include "Timer.h"

// hw_timer_t * timer = NULL;

//*************************************
void setup()
{
    init();
    Serial.println("-----Start-----");
}

//*************************************
void loop()
{
    digitalWrite(PORT_LED_G, !digitalRead(PORT_LED_G));
    delay(1000);
}

//*************************************
void init()
{
    Serial.begin(115200);
    
    pinMode(PORT_SW, INPUT_PULLUP);
    attachInterrupt(PORT_SW, funcInterrupt, FALLING);

    pinMode(PORT_LED_G, OUTPUT);
    pinMode(PORT_LED_R, OUTPUT);
    digitalWrite(PORT_LED_G, 0);
    digitalWrite(PORT_LED_R, 0);


    pinMode(PORT_MOTOR0, OUTPUT);
    pinMode(PORT_MOTOR1, OUTPUT);
    pinMode(PORT_MOTOR2, OUTPUT);
    pinMode(PORT_MOTOR3, OUTPUT);

    digitalWrite(PORT_MOTOR0, 0);
    digitalWrite(PORT_MOTOR1, 0);
    digitalWrite(PORT_MOTOR2, 0);
    digitalWrite(PORT_MOTOR3, 0);
}

//*************************************
void IRAM_ATTR onTimer()
{
    Serial.println(GetTime());
    attachInterrupt(PORT_SW, funcInterrupt, FALLING);
}

//*************************************
void funcInterrupt()
{
    attachInterrupt(PORT_SW, NULL, FALLING);
    setTimerInterrupt(&onTimer, 300000, false);

    int portLogic = !digitalRead(PORT_MOTOR0);
    digitalWrite(PORT_MOTOR0, portLogic);
    digitalWrite(PORT_MOTOR1, portLogic);
    digitalWrite(PORT_MOTOR2, portLogic);
    digitalWrite(PORT_MOTOR3, portLogic);

    digitalWrite(PORT_LED_G, portLogic);
    digitalWrite(PORT_LED_R, portLogic);

    Serial.println(GetTime());
}