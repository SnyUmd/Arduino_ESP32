#include "i2cCtrl.h"

void readI2C(byte* val, TwoWire& wire_, int addr, int readAddr, int read_to_byte)
{
    wire_.beginTransmission(addr);
    wire_.write(readAddr);
    // wire_.endTransmission(false);
    wire_.endTransmission();
    delay(60);
    // wire_.requestFrom(addr, read_to_byte, true);
    wire_.requestFrom(addr, read_to_byte);
    delay(60);
    for(int i = 0; i < read_to_byte; i++) val[i] = wire_.read();
}