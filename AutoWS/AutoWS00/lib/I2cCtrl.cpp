#include "I2cCtrl.hpp"

// void I2cCtrl::initI2C(TwoWire& wire_)
// {
//     wire_.begin();
// }

// void I2cCtrl::initI2C(TwoWire& wire_, int clock_speed = 100000)
// {
//     wire_.begin();
//     wire_.setClock(clock_speed);
// }

void I2cCtrl::initI2C(TwoWire& wire_, int clock_speed, int sda_port, int scl_port)
{
    wire_.begin(sda_port, scl_port);
    wire_.setClock(clock_speed);
}

void I2cCtrl::readI2C(byte* val, TwoWire& wire_, int addr, int readAddr, int read_to_byte, int wait_time)
{
    wire_.beginTransmission(addr);
    wire_.write(readAddr);
    // wire_.endTransmission(false);
    wire_.endTransmission();
    delay(wait_time);
    // wire_.requestFrom(addr, read_to_byte, true);
    wire_.requestFrom(addr, read_to_byte);
    delay(wait_time);
    for(int i = 0; i < read_to_byte; i++) val[i] = wire_.read();
}

/// @brief I2Cアドレスのスキャンを行う。
/// @param wire_ 
/// @param sr_ 
void I2cCtrl::checkI2cAddress(TwoWire& wire_, HardwareSerial& sr_)
{
    byte error, address;
    int nDevices;

    nDevices = 0;
    for(address = 1; address < 127; address++ ) 
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        wire_.beginTransmission(address);
        error = wire_.endTransmission();
    
        if (error == 0)
        {
            sr_.print("I2C device found at address 0x");
            if (address<16) 
                sr_.print("0");
            sr_.print(address,HEX);
            sr_.println("  !");
        
            nDevices++;
        }
        else if (error==4) 
        {
            sr_.print("Unknown error at address 0x");
            if (address<16) 
                sr_.print("0");
            sr_.println(address,HEX);
        }    
    }
    if (nDevices == 0)
        sr_.println("No I2C devices found\n");
    else
        sr_.println("done\n");
}


bool I2cCtrl::checkDevice(TwoWire& wire_, int address){
    wire_.beginTransmission(address);
    byte error = wire_.endTransmission();
    if(error == 0)
        return true;
    else
        return false;
}
