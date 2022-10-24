#include "i2cCtrl.h"

// void initI2C(TwoWire& wire_)
// {
//     wire_.begin();
// }

// void initI2C(TwoWire& wire_, int clock_speed = 100000)
// {
//     wire_.begin();
//     wire_.setClock(clock_speed);
// }

void initI2C(TwoWire& wire_, int clock_speed, int sda_port, int scl_port)
{
    wire_.begin(sda_port, scl_port);
    wire_.setClock(clock_speed);
}

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
