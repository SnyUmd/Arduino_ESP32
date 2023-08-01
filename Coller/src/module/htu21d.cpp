#include "htu21d.h"
#include "i2cCtrl.h"

//*******************************************************
//温度取得
float readTempHTD21D(TwoWire& wire_)
{
    unsigned int temp;
    byte val[3];
    // readI2C(val, wire_, HTDU21D_ADDRESS, TEMP_MEASURE_HOLD, 3);
    readI2C(val, wire_, HTDU21D_ADDRESS, TEMP_MEASURE, 3);
    temp = (unsigned int)val[0] << 8 | (unsigned int)val[1];
    temp &= 0xFFFC; //下位2bitを0に

    float tempSigTemp = temp / (float)65536; //2^16 = 65536
    float realTemperature = -46.85 + (175.72 * tempSigTemp); //From page 14
    return realTemperature;
}

//*******************************************************
//湿度取得
float readHumdHTD21D(TwoWire& wire_)
{
    unsigned int humd;
    byte val[3];
    // readI2C(val, wire_, HTDU21D_ADDRESS, TEMP_MEASURE_HOLD, 3);
    readI2C(val, wire_, HTDU21D_ADDRESS, HUMD_MEASURE, 3);
    humd = (unsigned int)val[0] << 8 | (unsigned int)val[1];
    humd &= 0xFFFC; //下位2bitを0に

    float tempSigHumd = humd / (float)65536; //2^16 = 65536
    float realHumd = -6 + (125 * tempSigHumd);
    return realHumd;
}
