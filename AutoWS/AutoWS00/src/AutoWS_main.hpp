// 参考：https://kohacraft.com/archives/202006201124.html
#include <EEPROM.h>

#define LED_INTERNAL 2
#define SW_INTERNAL 0

#define PWM_FRQ 50
#define PWM_BIT_NUM 8
#define PWM_CH 1

EEPROMClass eep = EEPROM;

const char *ssid = "um50-fx";
const char *password = "n17e92@53S19n";

char *device_name = "WaterCtrl_01";

int eepSize = 255;

