#include "rx8035.h"

void readI2C(byte* val, TwoWire& wire_, int addr, int readAddr, int read_to_byte)
{
    wire_.beginTransmission(addr);
    wire_.write(readAddr);
    wire_.endTransmission(false);

    wire_.requestFrom(addr, read_to_byte, true);
    for(int i = 0; i < read_to_byte; i++) val[i] = wire_.read();
}