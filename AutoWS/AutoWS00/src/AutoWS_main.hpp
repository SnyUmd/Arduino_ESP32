// 参考：https://kohacraft.com/archives/202006201124.html
#include <string>
#include <iostream>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>//250716_1
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
// WebServer server(PORT_NUM);
WebServer server(PORT_NUM);

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

EEPROMClass eep = EEPROM;

struct wifi_sts{
  String ssid;
  String pass;
  String host_name;
  int portNum;
};
wifi_sts wifiSts ={
  SSID_0,
  PASS_0,
  HOST_NAME,
  PORT_NUM
};

struct eepAddress{
  int eep_address_ssid;
  int eep_address_pass;
  int eep_address_host_name;
  int eep_address_port_num;
  int eep_address_sequence_num;
  int eep_address_measurement_num;
  int eep_address_start_time;
  int eep_address_end;
};
eepAddress eepAdd ={
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

bool ary_blDeviceSts[] ={
  false,//open
  false,//close
  false,//watering
  false,//open signal
  false//close signal
};
enum EnmDeviceSts{
  enmDevStsOpen = 0,
  enmDevStsClose,
  enmDevStsWatering,
  enmDevStsSignalOpen,
  enmDevStsSignalClose
};

String ary_messageError[]={
  "Error : The device could not be flushed with water",
  "Error : The device could not stop the water"
};
enum EnmMessageError{
  enmNotOpening = 0,
  enmNotClosing
};


int eepSize = 255;
int cntWater = 0;

long sTime = 0;
long eTime = 0;
int endTime = 10;

// bool flgCheckTime = false;
bool flgSolenoidSignal = false;

// bool waterSts = false;