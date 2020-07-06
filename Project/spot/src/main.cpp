#define LED_BLUE 2
#define BOARD_SW 0

#define LED_ON HIGH
#define LED_OFF LOW

#define SW_ON LOW
#define SW_OFF HIGH

#include <Arduino.h>

//***************************************************
void ledFlash()
{
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
  delay(250);
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
  delay(250);
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
  delay(250);
  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
}

//***************************************************
//***************************************************
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BOARD_SW, INPUT_PULLUP);

  digitalWrite(2, HIGH);
}

//***************************************************
//***************************************************
void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(BOARD_SW) == SW_ON)
    ledFlash();
}

