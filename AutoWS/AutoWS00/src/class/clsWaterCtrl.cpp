#include <Arduino.h>
#include "modules/def.hpp"
#include "clsWaterCtrl.hpp"
#include "modules/Timer.hpp"
// #include "AutoWS_main.hpp"

//▼def.hppから取得**************************
// #define PORT_WATER_OPEN 12
// #define PORT_WATER_CLOSE 13
// #define PORT_LED_OPEN 33//green
// #define PORT_LED_CLOSE 32//orange
// #define LED_OFF_EX 1
// #define LED_ON_EX 0
// #define SOLENOID_OFF 0
// #define SOLENOID_ON 1
//▲****************************************

//**********************************************************************
void ClsWaterCtrl::ResetSolenoidSignal(){
    digitalWrite(PORT_WATER_OPEN, SOLENOID_OFF);
    digitalWrite(PORT_WATER_CLOSE, SOLENOID_OFF);
    digitalWrite(PORT_LED_OPEN, LED_OFF_EX);
    digitalWrite(PORT_LED_CLOSE, LED_OFF_EX);
}

//**********************************************************************
int ClsWaterCtrl::CheckRunningWater(){
    int cnt = 0;
    bool before = 0;
    bool after = 0;
    bool wait = 1;
    for(int i = 0; i < 500; i++){
        after = digitalRead(PORT_RUN_WATER);
        if(after != before) cnt++;
        before = after;
        // if(wait == 1) wait = 5;
        // else wait = 1;
        delay(wait);
    }
    return cnt;
}

//**********************************************************************
void ClsWaterCtrl::WaterOpen(){
    digitalWrite(PORT_WATER_OPEN, SOLENOID_OFF);
    digitalWrite(PORT_WATER_CLOSE, SOLENOID_OFF);
    digitalWrite(PORT_LED_OPEN, LED_OFF_EX);
    digitalWrite(PORT_LED_CLOSE, LED_OFF_EX);

    waitTime(50);

    digitalWrite(PORT_WATER_OPEN, SOLENOID_ON);
    digitalWrite(PORT_LED_OPEN, LED_ON_EX);

    waitTime(10);

    digitalWrite(PORT_WATER_OPEN, SOLENOID_OFF);
    digitalWrite(PORT_LED_OPEN, LED_OFF_EX);

    waitTime(50);
    digitalWrite(PORT_LED_OPEN, LED_ON_EX);
}

//**********************************************************************
void ClsWaterCtrl::WaterClose(){
    digitalWrite(PORT_WATER_OPEN, SOLENOID_OFF);
    digitalWrite(PORT_WATER_CLOSE, SOLENOID_OFF);
    digitalWrite(PORT_LED_OPEN, LED_OFF_EX);
    digitalWrite(PORT_LED_CLOSE, LED_OFF_EX);

    waitTime(50);

    digitalWrite(PORT_WATER_CLOSE, SOLENOID_ON);
    digitalWrite(PORT_LED_CLOSE, LED_ON_EX);

    waitTime(10);

    digitalWrite(PORT_WATER_CLOSE, SOLENOID_OFF);
    digitalWrite(PORT_LED_CLOSE, LED_OFF_EX);

    waitTime(50);
    digitalWrite(PORT_LED_CLOSE, LED_ON_EX);

}



//**********************************************************************

