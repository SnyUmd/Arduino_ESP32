#include "rx8035.h"
#include "i2cCtrl.h"

#define ADR_RTC 0x32  //0110010

void getTimeRX8035(byte * val, TwoWire& wire_)
{
    readI2C(val, wire_, ADR_RTC, 0, 7);
}
