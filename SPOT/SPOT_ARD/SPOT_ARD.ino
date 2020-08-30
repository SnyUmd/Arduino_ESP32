#define LED_RED 4
#define LED_GRE 16

#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GRE, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GRE, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  LedFlash(1000);
}

void LedFlash(int iPeriod)
{
  digitalWrite(LED_RED, !digitalRead(LED_RED));
  digitalWrite(LED_GRE, !digitalRead(LED_GRE));
  
  delay(iPeriod);
}
