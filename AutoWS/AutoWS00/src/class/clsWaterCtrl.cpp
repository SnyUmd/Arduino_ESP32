#include <Arduino.h>
#include "clsWaterCtrl.hpp"

// #include "modules/def.hpp"

//**********************************************************************
///sts = 0:false, 1:true
void ClsWaterCtrl::SetWater(bool is_open, int port_open, int port_close){
    digitalWrite(port_open, 0);
    digitalWrite(port_close, 0);

    digitalWrite(port_open, is_open);
    digitalWrite(port_close, !is_open);

    delay(10);

    digitalWrite(port_open, 0);
    digitalWrite(port_close, 0);
}
//**********************************************************************
//logic_led_off = LED消灯時の論理　0 or 1
void ClsWaterCtrl::SetLED(bool is_open, int port_led_open, int port_led_close, int logic_led_off){
    int port = is_open ? port_led_open: port_led_close;

    digitalWrite(port, !logic_led_off);
    delay(10);
    digitalWrite(port, logic_led_off);

}
