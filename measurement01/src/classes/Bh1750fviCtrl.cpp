#include "Bh1750fviCtrl.hpp"


float Bh1750fviCtrl::processingMode2Value(int val){
    float result_f = 0;
    float buf_i;

    //動作１-----------------------
    // float toAdd = 0;
    // if(val & 1) toAdd = 0.5f;
    // result_f = round((float)(val >> 1) / 1.2f) + toAdd;
    //^---------------------------

    //動作2-----------------------
    //１．測定値を右に1シフト
    //２．最下位ビットが1の時はシフトした値に0.5をプラス
    //３．１，２で加工した測定値を1.2で割る
    //４．小数点第二位で四捨五入
    //５．小数点以下が0.35~0.74の時　小数点第一位を0.5にする。

    if(val & 1) result_f += 0.5f;
    result_f += val >> 1;
    result_f = result_f / 1.2f;
    
    buf_i = result_f - (int)result_f;
    if(buf_i > 0.35f && buf_i < 0.74f) 
        result_f = (int)result_f + 0.5f;
    else 
        result_f = round(result_f);
    //^---------------------------
    return result_f;
}

void Bh1750fviCtrl::initBh1750fvi(TwoWire& wire_){
    // wire_.beginTransmission(BH1750FVI_ADDRESS);
    // wire_.write(COMMAND_POWER_DOWWN);
    // wire_.endTransmission();
    // delay(100);
    delay(100);

    //パワーオン
    wire_.beginTransmission(BH1750FVI_ADDRESS);
    wire_.write(COMMAND_POWER_ON);
    wire_.endTransmission();
    delay(10);

    //リセット
    wire_.beginTransmission(BH1750FVI_ADDRESS);
    wire_.write(COMMAND_RESET);
    wire_.endTransmission();
    delay(10);

    //パワーダウン
    wire_.beginTransmission(BH1750FVI_ADDRESS);
    wire_.write(COMMAND_POWER_DOWWN);
    wire_.endTransmission();
    delay(10);
}

float Bh1750fviCtrl::readIllumi_Bh1750fvi_toFloat(TwoWire& wire_, int mode0_2)
{
    int bufIllmi = 0;
    float resultIllmi = 0;
    byte illmiVal[3];
    int aryModeCommands[3][2] = 
    {
        {COMMAND_ONE_SHOT_L_RESOLUTION_MODE, WAIT_TIME_L_RESOLUTION_MODE},
        {COMMAND_ONE_SHOT_H_RESOLUTION_MODE, WAIT_TIME_H_RESOLUTION_MODE},
        {COMMAND_ONE_SHOT_H_RESOLUTION_MODE2, WAIT_TIME_H_RESOLUTION_MODE2}
    };

    //パワーオンコマンド送信
    wire_.beginTransmission(BH1750FVI_ADDRESS);
    wire_.write(COMMAND_POWER_ON);
    wire_.endTransmission();
    delay(10);

    //測定コマンド送信
    wire_.beginTransmission(BH1750FVI_ADDRESS);
    wire_.write(aryModeCommands[mode0_2][0]);
    wire_.endTransmission();
    delay(aryModeCommands[mode0_2][1]);

    //測定データ抽出
    wire_.requestFrom(BH1750FVI_ADDRESS, 2);
    delay(aryModeCommands[mode0_2][1]);
    for(int i = 0; i < 2; i++) {
        illmiVal[i] = wire_.read();
    }
    bufIllmi = (illmiVal[0] << 8) + illmiVal[1];
    delay(10);
    wire_.endTransmission();

    //動作2-----------------------
    if(mode0_2 == 2)
        resultIllmi = processingMode2Value(bufIllmi);
    else
        resultIllmi = round((float)bufIllmi / 1.2f);
    //^---------------------------

    return resultIllmi;
}

String Bh1750fviCtrl::readIllumi_Bh1750fvi_toString(TwoWire& wire_, int mode0_2){
    String resutl_s = (String)readIllumi_Bh1750fvi_toFloat(wire_, mode0_2);
    resutl_s.remove(resutl_s.length() - 1, 1);
    return resutl_s;
}


