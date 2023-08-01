
#define HTDU21D_ADDRESS 0x40  //Unshifted 7-bit I2C address for the sensor

#define TEMP_MEASURE_HOLD  0xE3
#define HUMD_MEASURE_HOLD  0xE5
#define TEMP_MEASURE  0xF3
#define HUMD_MEASURE  0xF5
#define WRITE_USER_REG  0xE6
#define READ_USER_REG  0xE7
#define SOFT_RESET  0xFE

#include <Arduino.h>
#include <Wire.h>

float readTempHTD21D(TwoWire& wire_);
float readHumdHTD21D(TwoWire& wire_);
