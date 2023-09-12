#include "i2cCtrl.h"
#include "apds9930.h"
float readTestApds9930(TwoWire& wire_, HardwareSerial& sr)
{
    unsigned int temp;
    byte val[3];

    readI2C(val, wire_, APDS9930_ADDRESS, PDATAL, 3);
    temp = (unsigned int)val[1] << 8 | (unsigned int)val[0];
    temp &= 0xFFFC; //下位2bitを0に
    sr.println(val[0]);
    sr.println(val[1]);
    // float tempSigTemp = temp / (float)65536; //2^16 = 65536
    // float realTemperature = -46.85 + (175.72 * tempSigTemp); //From page 14
    return temp;
}
