#include <Arduino.h>

#include "clsWaterCtrl.hpp"
#include "modules/def.hpp"

//**********************************************************************
///sts = 0:false, 1:true
void ClsWaterCtrl::WaterSet(bool is_open){
    digitalWrite(PORT_LED_INTERNAL, is_open);

    digitalWrite(PORT_WATER_OPEN, 0);
    digitalWrite(PORT_WATER_CLOSE, 0);
    digitalWrite(PORT_LED_GREEN, LED_OFF_EX);
    digitalWrite(PORT_LED_ORANGE, LED_OFF_EX);

    digitalWrite(PORT_WATER_OPEN, is_open);
    digitalWrite(PORT_WATER_CLOSE, !is_open);
    digitalWrite(PORT_LED_GREEN, !is_open);
    digitalWrite(PORT_LED_ORANGE, is_open);

    delay(10);

    digitalWrite(PORT_WATER_OPEN, 0);
    digitalWrite(PORT_WATER_CLOSE, 0);
    digitalWrite(PORT_LED_GREEN, LED_OFF_EX);
    digitalWrite(PORT_LED_ORANGE, LED_OFF_EX);
}