#define SCD4X_ADDRESS 0x62

//測定スタートコマンド
#define START_PERIODIC_MEASUREMENT_MSB 0x21
#define START_PERIODIC_MEASUREMENT_LSB 0xb1
//測定値読込コマンド
#define READ_MEASUREMENT_MSB 0xec
#define READ_MEASUREMENT_LSB 0x05
//測定終了コマンド
#define STOP_PERIODIC_MEASUREMENT_MSB 0x3f
#define STOP_PERIODIC_MEASUREMENT_LSB 0x86 
//センサ高度設定コマンド
#define SET_SENSOR_ALTITUDE_MSB 0x24
#define SET_SENSOR_ALTITUDE_LSB 0x27
//センサ高度データ(9m)
#define SENSOR_ALTITUDE_VAL_MSB 0x00
#define SENSOR_ALTITUDE_VAL_LSB 0x09
#define SENSOR_ALTITUDE_VAL_CRC 0x09

//測定完了確認コマンド
#define GET_DATA_READY_STATUS_MSB 0xe4
#define GET_DATA_READY_STATUS_LSB 0xb8

//CO2オートキャリブレーション設定コマンド
#define SET_AUTOMATIC_CALIBRATION_ENABLED_MSB 0x24
#define SET_AUTOMATIC_CALIBRATION_ENABLED_LSB 0x16

//CO2オートキャリブレーション確認コマンド
#define GET_AUTOMATIC_CALIBRATION_ENABLED_MSB 0x23
#define GET_AUTOMATIC_CALIBRATION_ENABLED_LSB 0x13

#include "scd4x.hpp"
#include "def_stc00.hpp"

//*************************************
//電源投入後1秒後にスタンバイになる。
void initScd4x(TwoWire& wire_){
    delay(1500);

    //センサ高度設定
    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(SET_SENSOR_ALTITUDE_MSB);
    wire_.write(SET_SENSOR_ALTITUDE_LSB);
    wire_.write(SENSOR_ALTITUDE_VAL_MSB);
    wire_.write(SENSOR_ALTITUDE_VAL_LSB);
    wire_.write(SENSOR_ALTITUDE_VAL_CRC);
    wire_.endTransmission();
    delay(100);
}
//*************************************

//*************************************
//コマンドを送信する
bool sendCommandWcd4x(TwoWire& wire_, int command, int data_val)
{
    int command_msb, command_lsb, data_msb, data_lsb, data_crc;
    byte command_buf[2], data_buf[3];

    if(command > 0xffff || data_val > 0xffffff) return false;

    wire_.beginTransmission(SCD4X_ADDRESS);

    //コマンドセット
    command_msb = command >> 8;
    command_lsb = command & 0x100FF;
    command_buf[0] = command_msb;
    command_buf[1] = command_lsb;

    // Wire.write(command_msb);
    // Wire.write(command_lsb);    
    wire_.write(command_buf, 2);

    //データセット
    if(data_val != 0)
    {
        data_msb = data_val >> 16;
        data_lsb = (data_val & 0x100FFFF) >> 8;
        data_crc = data_val & 0x10000FF;
        data_buf[0] = data_msb;
        data_buf[1] = data_lsb;
        data_buf[2] = data_crc;

        // Wire.write(data_msb);
        // Wire.write(data_lsb);
        // Wire.write(data_crc);
        wire_.write(data_buf, 3);
    }
    wire_.endTransmission();
    delay(100);
    return true;
}

//*************************************
//CO2を測定
int readCO2_scd4x(TwoWire& wire_){
    int co2Result;
    byte co2Val[4];

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(START_PERIODIC_MEASUREMENT_MSB);
    wire_.write(START_PERIODIC_MEASUREMENT_LSB);
    wire_.endTransmission();

    while(!isMeasurementRady(wire_)){
        digitalWrite(PORT_LED, !digitalRead(PORT_LED));//debug
        delay(100);
    }
    digitalWrite(PORT_LED, LED_OFF);//debug

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(READ_MEASUREMENT_MSB);
    wire_.write(READ_MEASUREMENT_LSB);
    wire_.endTransmission();
    delay(100);
    wire_.requestFrom(SCD4X_ADDRESS, 9);
    delay(100);

    for(int i = 0; i < 3; i++)co2Val[i] = wire_.read();
    co2Result = co2Val[0] << 8 | co2Val[1];

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(STOP_PERIODIC_MEASUREMENT_MSB);
    wire_.write(STOP_PERIODIC_MEASUREMENT_LSB);
    wire_.endTransmission();

    return co2Result;
}


//*************************************
float readTemp_scd4x(TwoWire& wire_){
    int tempResult;
    byte tempVal[4];

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(START_PERIODIC_MEASUREMENT_MSB);
    wire_.write(START_PERIODIC_MEASUREMENT_LSB);
    wire_.endTransmission();
    // delay(5000);
    while(!isMeasurementRady(wire_)){
        digitalWrite(PORT_LED, !digitalRead(PORT_LED));//debug
        delay(100);
    }
    digitalWrite(PORT_LED, LED_OFF);//debug
    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(READ_MEASUREMENT_MSB);
    wire_.write(READ_MEASUREMENT_LSB);
    wire_.endTransmission();
    delay(100);
    wire_.requestFrom(SCD4X_ADDRESS, 9);
    delay(100);

    for(int i = 3; i < 6; i++)tempVal[i - 3] = wire_.read();
    tempResult = tempVal[0] << 8 | tempVal[1];

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(STOP_PERIODIC_MEASUREMENT_MSB);
    wire_.write(STOP_PERIODIC_MEASUREMENT_LSB);
    wire_.endTransmission();

    return tempResult;
}

//*************************************
float readRH_scd4x(TwoWire& wire_){
    int rhResult;
    byte rhVal[4];

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(START_PERIODIC_MEASUREMENT_MSB);
    wire_.write(START_PERIODIC_MEASUREMENT_LSB);
    wire_.endTransmission();
    // delay(5000);
    while(!isMeasurementRady(wire_)){
        digitalWrite(PORT_LED, !digitalRead(PORT_LED));//debug
        delay(100);
    }
    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(READ_MEASUREMENT_MSB);
    wire_.write(READ_MEASUREMENT_LSB);
    wire_.endTransmission();
    delay(100);
    wire_.requestFrom(SCD4X_ADDRESS, 9);
    delay(100);

    for(int i = 6; i < 10; i++)rhVal[i - 6] = wire_.read();
    rhResult = rhVal[0] << 8 | rhVal[1];

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(STOP_PERIODIC_MEASUREMENT_MSB);
    wire_.write(STOP_PERIODIC_MEASUREMENT_LSB);
    wire_.endTransmission();

    return rhResult;
}

//*************************************
bool isMeasurementRady(TwoWire& wire_)
{
    byte responseVal[4];

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(GET_DATA_READY_STATUS_MSB);
    wire_.write(GET_DATA_READY_STATUS_LSB);
    wire_.endTransmission();
    delay(10);
    wire_.requestFrom(SCD4X_ADDRESS, 3);
    for(int i = 0; i < 3; i++)responseVal[i] = wire_.read();

    //受信データの11bit目が1で準備完了
    if((responseVal[1] & 0b100000100) != 0) return true;
    else return false;
}


//*************************************
void setAutomaticSelfCalibration(TwoWire& wire_, bool is_set){
    byte responseVal[4];

    wire_.beginTransmission(SCD4X_ADDRESS);
    delay(10);
    wire_.write(SET_AUTOMATIC_CALIBRATION_ENABLED_MSB);
    wire_.write(SET_AUTOMATIC_CALIBRATION_ENABLED_LSB);

    wire_.write(0x00);
    if(is_set)
        wire_.write(0x01);
    else
        wire_.write(0x00);
    wire_.write(0xb0);
    delay(10);
    wire_.endTransmission();
    delay(10);

    wire_.beginTransmission(SCD4X_ADDRESS);
    delay(10);
    wire_.write(0x36);
    wire_.write(0x15);
    delay(10);
    wire_.endTransmission();
    delay(1000);
}



//*************************************
bool isAutomaticSelfCalibration(TwoWire& wire_, HardwareSerial& sr_/*debug*/){
    byte responseVal[4];

    wire_.beginTransmission(SCD4X_ADDRESS);
    wire_.write(GET_AUTOMATIC_CALIBRATION_ENABLED_MSB);
    wire_.write(GET_AUTOMATIC_CALIBRATION_ENABLED_LSB);
    wire_.endTransmission();
    delay(10);
    wire_.requestFrom(SCD4X_ADDRESS, 3);
    for(int i = 0; i < 3; i++)responseVal[i] = wire_.read();
    sr_.println(responseVal[0]);
    sr_.println(responseVal[1]);
    if(responseVal[1] == 1) return true;
    else return false;
}
