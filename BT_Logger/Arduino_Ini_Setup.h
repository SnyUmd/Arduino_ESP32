// Arduino_Ini_Setup.h

#ifndef _ARDUINO_INI_SETUP_h
#define _ARDUINO_INI_SETUP_h

//#include <Wire.h>
//#include <ESP.h>
//#include <Arduino.h>
#include <BluetoothSerial.h>
//#include <BLEDevice.h>
#include <WiFi.h>
//00***********************************************************
/*
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
*/
//00***********************************************************




bool mArduino_Serial_Set(HardwareSerial& HS_serial);

void mSetPort_Out(int i_port_num, bool bl_high);
void mSetPort_Imput(int i_port_num, bool bl_pull, bool bl_high);
void mErrLoop(int i_port_num, int i_delay_time);
//ESP32********************************************************
bool mESP32_BLSerial_Set(BluetoothSerial& BS_Serial, String strBT_Name, HardwareSerial HS_serial);
int mESP32_BLSerial_Read(BluetoothSerial& BT_Serial);
float mGetTemperature_BuiltIn();
void mPWM_Out_iniSet(int i_ch_no, int i_port, int i_freq, int i_resolution, int i_percent);
void mPWM_Out(int i_ch_no, int i_resolution, int i_percent);

//Task*********************************************************
void mTask_Set(TaskFunction_t task_2nd, const char* name, uint32_t i32t_StackDepth, void* pvParameters, UBaseType_t uxPriority, TaskHandle_t* pvCreatedTask, BaseType_t xCoreID);
void mTask_2nd(void* pvParameters);




//ESP32********************************************************
bool mESP32_WiFi_Set
(
	WiFiClass& wifi_class,
	WiFiServer& wifi_server,
	HardwareSerial& hs_serial,
	char ch_ssid[],
	char ch_pass[],
	int i_signal_portNum,
	bool bl_serial_message
);

String mGetTime_jst();


//BEEP********************************************************
void mBeep(int i_port_no_r, int i_port_no_l, bool bl_r, bool bl_l, int i_beep_time, int i_vol);


#endif




