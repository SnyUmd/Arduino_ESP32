#include <Arduino.h>
#include <Wire.h>

#define ADDRESS_TEMP 0xFA
#define ADDRESS_TEMP_XLSB 0xFC
#define ADDRESS_TEMP_MSB 0xFA
#define ADDRESS_TEMP_LSB 0xFB

#define ADDRESS_PRESS 0xF7
#define ADDRESS_PRESS_XLSB 0xF9
#define ADDRESS_PRESS_MSB 0xF7
#define ADDRESS_PRESS_LSB 0xF8

//bit7,6,5 通常モードでの非アクティブ期間 t-standby を制御
//000 -> 0.5
//001 -> 62.5
//010 -> 125
//011 -> 250
//100 -> 500
//*101 -> 1000
//110 -> 2000
//111 -> 4000
//bit4,3,2 IIR フィルターの時定数を制御
//bit1 resurb
//bit0 1 → 3線SPIインターフェイス有効
#define ADDRESS_COMFIG 0xF5

//bit7,6,5 温度データのオーバーサンプリング制御
//*000 -> skipped
//001 -> x1  16bit/0.0050
//010 -> x2  17bit/0.0025
//011 -> x4  18bit/0.0012
//100 -> x8  19bit/0.0006
//101,110,111 -> x16 20bit/0.0003
//------------------
//bit4,3,2 気圧データのオーバーサンプリング制御
//000 -> skipped
//001 -> x1  16bit/2.62Pa x1
//010 -> x2  17bit/1.31Pa x1
//011 -> x4  18bit/0.66Pa x1
//100 -> x8  19bit/0.33Pa x1
//*101,110,111 -> x16 20bit/0.16Pa x2
//------------------
//bit1,0 電源モード
//00    -> sleep
//01,10 -> forced mode
//*11    -> normal mode
#define ADDRESS_CTRL_MEAS 0xF4

//測定リクエスト後 bit3が0になったのを確認してデータを吸いだす。
#define ADDRESS_STATUS 0xF3 

#define ADDRESS_RESET 0xE0
#define ADDRESS_ID 0xD0
#define ADDRESS_CAL_DATA 0x88//20byte [0x88 - 0xA1]