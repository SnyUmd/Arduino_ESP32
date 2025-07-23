#include <ArduinoJson.h>
#define LED_INTERNAL 2
#define SW_INTERNAL 0

#define LED_ON 1
#define LED_OFF 0

void setup() {
  // put your setup code here, to run once:
  initPort();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(SW_INTERNAL) == LOW){
    digitalWrite(LED_INTERNAL, !digitalRead(LED_INTERNAL));
    delay(500);
  }
}

void initPort()
{
    pinMode(LED_INTERNAL, OUTPUT);
    pinMode(SW_INTERNAL, INPUT);

    digitalWrite(LED_INTERNAL, LED_OFF);
}