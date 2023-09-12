#define APDS9930_ADDRESS 0x39

#define DEVICE_ID 0x12
#define DEVICE_STATUC 0x13

#define CH0DATAL 0x14
#define CH0DATAH 0x15

#define CH1DATAL 0x16
#define CH1DATAH 0x17

#define PDATAL 0x18
#define PDATAH 0x19

#include <Arduino.h>
#include <Wire.h>

float readTestApds9930(TwoWire&, HardwareSerial&);