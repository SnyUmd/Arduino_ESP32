#define LED_INTERNAL 2
#define SW_INTERNAL 0

#define LED_ON 1
#define LED_OFF 0

//**********************************************************************
#include <ArduinoJson.h>
#include <WiFi.h>
#include "fauxmo/fauxmoESP.h"

//**********************************************************************
void initPort();
void WifiSetup();
void SetupFauxmo();
void LedFlash(int flash_num, int interval);

//**********************************************************************
fauxmoESP fauxmo;

// const char *ssid = "um50-fx_2s";
// const char *password = "n17e92@53S19n";

const char *ssid = "um50-fx";
const char *password = "n17e92@53S19n";

const char *id_device = "um";