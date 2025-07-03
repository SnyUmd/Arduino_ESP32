#define BH1750FVI_ADDRESS 0x23

#define COMMAND_POWER_DOWWN 0 //0b00000000
#define COMMAND_POWER_ON 1 //0b00000001
#define COMMAND_RESET 7 //0b0000111

// #define COMMAND_L_RESOLUTION_MODE 0x13 //0b00010011
// #define COMMAND_H_RESOLUTION_MODE 0x10 //0b00010000
// #define COMMAND_H_RESOLUTION_MODE2 0x11 //0b00010001

#define COMMAND_ONE_SHOT_L_RESOLUTION_MODE 0x23 //0b00100011
#define COMMAND_ONE_SHOT_H_RESOLUTION_MODE 0x20 //0b00100000
#define COMMAND_ONE_SHOT_H_RESOLUTION_MODE2 0x21 //0b00100001

//データシートのTYP時間では足りなかった。
#define WAIT_TIME_L_RESOLUTION_MODE 20 //ms  TYP16
#define WAIT_TIME_H_RESOLUTION_MODE 150 //ms  TYP120
#define WAIT_TIME_H_RESOLUTION_MODE2 150 //ms  TYP120

#include <Arduino.h>
#include <Wire.h>
// #include "CommonCls.hpp"

class Bh1750fviCtrl{
    private:
    int test = 10000;
    float processingMode2Value(int val);

    class clsTest;

    public:
    void initBh1750fvi(TwoWire& wire_);
    float readIllumi_Bh1750fvi_toFloat(TwoWire& wire_, int mode0_2);
    String readIllumi_Bh1750fvi_toString(TwoWire& wire_, int mode0_2);
    
    // void setTest(int val);
    // int getTest();
    // void globalTest(HardwareSerial& sr);
};


// inline void Bh1750fviCtrl::setTest(int val){
//     // test = val;
//     this -> test = val;
// }
// inline int Bh1750fviCtrl::getTest(){
//     // return test;
//     return this -> test;
// }
// inline void Bh1750fviCtrl::globalTest(HardwareSerial& sr){
//     sr.println(test);
// }