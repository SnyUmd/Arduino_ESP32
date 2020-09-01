#include "PowerCtrl.h"


void clsPowerCtrl::DeepSleep_WakeupPort(gpio_num_t portNum, bool blHigh)
//portNum:GPIO_NUM_*
//ex)　deepSleep_Port(GPIO_NUM_0);
{
  int iHighLow = 0;
  if(blHigh)
    iHighLow = 1;
  esp_sleep_enable_ext0_wakeup(portNum, iHighLow);

  digitalWrite(LED_GRE, HIGH);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(SPOT_LIGHT, LOW);
  esp_deep_sleep_start();
}