#include <Arduino.h>
#include <Wire.h>

void initScd4x(TwoWire& wire_);
bool sendCommandWcd4x(TwoWire& wire_, int command, int data_val = 0);
int readCO2_scd4x(TwoWire& wire_);
float readTemp_scd4x(TwoWire& wire_);
float readRH_scd4x(TwoWire& wire_);
bool isMeasurementRady(TwoWire& wire_);
void setAutomaticSelfCalibration(TwoWire& wire_, bool is_set);
bool isAutomaticSelfCalibration(TwoWire& wire_, HardwareSerial& sr_/*debug*/);