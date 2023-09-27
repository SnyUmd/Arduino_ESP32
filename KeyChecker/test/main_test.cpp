#include <Arduino.h>
// #include <WiFi.h>
// #include <WebServer.h>
// #include <vector>
// #include <iostream>
// #include <vector>

#include "module/common.h"
#include "module/defKeyChecker.h"
#include "module/Initialize.h"
//#include "module/Interrupt.h"
// #include "module/wifiCtrl.h"
// #include "module/BzCtrl.h"
#include "module/i2cCtrl.h"
// #include "module/apds9930.h"
#include "module/Timer.h"

void IRAM_ATTR LightOff();

hw_timer_t* hw_timer = NULL;
uint8_t timer_num = 0;
uint64_t alarm_len = 1 * 1000000;
bool repetition = false;

void setup() {
    sr.begin(115200);
    sr.println("-----start-----");
    
    initPort();
}

void loop() {
    digitalWrite(P_LED_INTERNAL, LED_ON);
    sr.println("Light ON");

    sr.println(timer_num);
    sr.println(alarm_len);
    sr.println(repetition);
    setTimerInterrupt(hw_timer, timer_num, LightOff, alarm_len, repetition);

    sr.println("Set timer int");
    delay(5000);
}

//**********************************************************
// void IRAM_ATTR LightOff()
void IRAM_ATTR LightOff()
{
    sr.println("get timer int");
    digitalWrite(P_LED_INTERNAL, LED_OFF);
    sr.println("Light OFF");
    sr.println("");

    timerAlarmDisable(hw_timer);
    timerStop(hw_timer);
    timerEnd(hw_timer);
    // timer_delete(timer_num);
    hw_timer = NULL;
//   timerAttachInterruptFlag()
}