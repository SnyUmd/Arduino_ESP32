#include <Arduino.h>
#include "module/common.h"
#include "module/defKeyChecker.h"
#include "module/Initialize.h"
#include "module/Interrupt.h"

// put function declarations here:
void swInterrupt_fn();





//**********************************************************
//**********************************************************
//**********************************************************
void setup() {
  // put your setup code here, to run once:
  initPort();
  // attachInterrupt(SW_INTERNAL, swInterrupt, FALLING);
  initInterrupt(SW_INTERNAL, swInterrupt_fn, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}


void swInterrupt_fn()
{
  if(digitalRead(SW_INTERNAL) == !SW_OFF)
  {
    // チャタリング防止の為、割り込み停止
    attachInterrupt(SW_INTERNAL, NULL, FALLING);

    //割り込み処理--------------------------
    digitalWrite(LED_INTERNAL, !digitalRead(LED_INTERNAL));
    // ------------------------------------

    //割り込みセット
    attachInterrupt(SW_INTERNAL, swInterrupt_fn, FALLING);
  }
}