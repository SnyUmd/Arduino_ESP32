#include <Arduino.h>
#include <Wire.h>

// void initI2C(TwoWire& wire_);
// void initI2C(TwoWire& wire_, int clock_speed);
void initI2C(TwoWire& wire_, int clock_speed = 100000, int sda_port = 21, int scl_port = 22);
void readI2C(byte* val, TwoWire& wire_, int addr, int readAddr, int read_to_byte);