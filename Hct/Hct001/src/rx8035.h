#include <Wire.h>
#include <Arduino.h>

void readI2C(byte* val, TwoWire& wire_, int addr, int readAddr, int read_to_byte);