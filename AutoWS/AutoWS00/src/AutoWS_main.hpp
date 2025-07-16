// 参考：https://kohacraft.com/archives/202006201124.html
#include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>

// #include "class/clsWaterCtrl.hpp"
#include "class/clsCommon.hpp"
#include "modules/def.hpp"
#include "modules/Timer.hpp"
#include "modules/wifiCtrl.hpp"

ClsWaterCtrl clsWaterCtrl;
ClsCommon clsCommon;

HardwareSerial sr(0);//RX:34 TX:35
// HardwareSerial sr(2);//RX:16 TX:17

WiFiClass wf;
WebServer server(SERVER_PORT_NUM);

hw_timer_t * timer = NULL;
EEPROMClass eep = EEPROM;

// const char *ssid = "um50-fx";
// const char *password = "n17e92@53S19n";
// const char *host_name = "WaterController00";

struct wifi_sts
{
  String ssid;
  String pass;
  String host_name;
  String portNum;
  int eep_address_ssid;
  int eep_address_pass;
  int eep_address_host_name;
  int eep_address_port_num;
  int eep_address_sequence_num;
  int eep_address_measurement_num;
  int eep_address_start_time;
  int stop;
};

wifi_sts wifiSts =
{
  SSID_0,
  PASS_0,
  HOST_NAME,
  HTTP_PORT_NUM,
  0x00,
  0x20,
  0x40,
  0x60,
  0x80,
  0x84,
  0x88,
  0x118,
  //※EEP領域の確保数注意(eepSize)
};

int eepSize = 255;
int cntWater = 0;

int sTime = 0;
int endTime = 10;
int flgCheckTime = false;

bool waterSts = false;