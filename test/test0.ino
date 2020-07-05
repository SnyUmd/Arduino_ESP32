#define LED_BLUE 2
#define BOARD_SW 0
#define EX_SW1 23

#define LED_ON HIGH
#define LED_OFF LOW

#define SW_ON LOW
#define SW_OFF HIGH

//#include <Wire.h>
#include <Arduino.h>

bool blLED = false;

//*******************************************************
void setup() {
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BOARD_SW, INPUT_PULLUP);
  pinMode(EX_SW1, INPUT_PULLUP);

  xTaskCreatePinnedToCore(loop2, "loopTask", 4096, NULL, 1, NULL, 1);

  digitalWrite(LED_BLUE, LED_ON);
  delay(1000);
}



//*******************************************************
void loop() {
  //if(digitalRead(BOARD_SW) == SW_ON)
  if(blLED)
    digitalWrite(LED_BLUE, LED_ON);
  else
    digitalWrite(LED_BLUE, LED_OFF);
}

//*******************************************************
void loop2(void *pvParameters)
{
  while(1)
  {
    if(blLED == false && digitalRead(BOARD_SW) == SW_ON)
    {
      blLED = true;
      delay(1000);
    }
    if(blLED == true && digitalRead(BOARD_SW) == SW_ON)
    {
      blLED = false;
      delay(1000);
    }
  }
}