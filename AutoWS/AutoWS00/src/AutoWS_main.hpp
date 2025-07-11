// 参考：https://kohacraft.com/archives/202006201124.html
#include <EEPROM.h>

#include "class/clsWaterCtrl.hpp"

ClsWaterCtrl ClsWaterCtrl;

EEPROMClass eep = EEPROM;

const char *ssid = "um50-fx";
const char *password = "n17e92@53S19n";

char *device_name = "WaterCtrl_01";

int eepSize = 255;

bool waterSts = false;
