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

    // digitalWrite(port_open, 0);
    // digitalWrite(port_close, 0);
}

//**********************************************************************
void ClsWaterCtrl::ResetflgSolenoidSignal(int port_open, int port_close, int port_led_open, int port_led_close, int logic_led_off){
    digitalWrite(port_open, 0);
    digitalWrite(port_close, 0);
    digitalWrite(port_led_open, logic_led_off);
    digitalWrite(port_led_close, logic_led_off);
}


//**********************************************************************
//logic_led_off = LED消灯時の論理　0 or 1
void ClsWaterCtrl::SetLED(bool is_open, int port_led_open, int port_led_close, int logic_led_off){
    int port = is_open ? port_led_open: port_led_close;

    digitalWrite(port, !logic_led_off);
    // delay(10);
    // digitalWrite(port, logic_led_off);
}

//**********************************************************************
int ClsWaterCtrl::CheckRunningWater(int port_run_water){
    int cnt = 0;
    bool before = 0;
    bool after = 0;
    bool wait = 1;
    for(int i = 0; i < 500; i++){
        after = digitalRead(port_run_water);
        if(after != before) cnt++;
        before = after;
        // if(wait == 1) wait = 5;
        // else wait = 1;
        delay(wait);
    }
    return cnt;
}
