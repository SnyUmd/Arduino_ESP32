// 参考：https://kohacraft.com/archives/202006201124.html
#include <Arduino.h>
#include <string>
#include <iostream>
#include <WiFi.h>
// #include <WebServer.h>
#include <Espalexa.h>
// #include <ESPmDNS.h>//250716_1
#include <EEPROM.h>

#include "modules/def.hpp"
#include "modules/Timer.hpp"
#include "modules/def.hpp"
#include "modules/motorCtrl.hpp"
#include "modules/ESP32_eep.hpp"

HardwareSerial sr(0);//RX:34 TX:35
// HardwareSerial sr(2);//RX:16 TX:17

EEPROMClass eep = EEPROM;

WiFiClass wf;
// WebServer server(PORT_NUM);
// WebServer server(PORT_NUM);

hw_timer_t * timer_close = NULL;//タイマーNo.0
hw_timer_t * timer_resurve1 = NULL;//タイマーNo.1
hw_timer_t * timer__resurve2 = NULL;//タイマーNo.2
hw_timer_t * timer__resurve3 = NULL;//タイマーNo.3
enum EnmTimerNum{
  enmStop = 0,
  enmReserve1,
  enmReserve2,
  enmReserve3,
};

struct device_sts{
  String ssid;
  String pass;
  String device_name;
  int now_power;
  bool eep_flg;
};
device_sts deviceSts;

struct eepAddress{
  int eep_address_ssid;
  int eep_address_pass;
  int eep_address_device_name;
  int eep_address_now_power;
  int eep_address_flg;
  int eep_address_end;
};
eepAddress eepAdd ={
  0x00,
  0x20,
  0x40,
  0x60,
  0x70,
  0x71
  //※EEP領域の確保数注意(EEP_SIZE)
};

struct motorSet{
  bool blMotorAction;
  int loop;
  int is_dark;
};
motorSet MotorSet = {
  false,
  0,
  false
};

long sTime = 0;
long eTime = 0;
int endTime = 10;

const char END_CHAR = '*';

// bool flgCheckTime = false;
bool flgSolenoidSignal = false;

// bool waterSts = false;