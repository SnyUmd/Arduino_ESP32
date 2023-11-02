#include <Arduino.h>
// #include <string>
// #include <WiFi.h>
#include <WebServer.h>
// #include <Wire.h>
#include <iostream>
#include <vector>
// #include <EEPROM.h>
#include <BluetoothSerial.h>
#include "module/initialize.h"
#include "module/defHct.h"
#include "module/common.h"
#include "module/Timer.h"
#include "module/wifiCtrl.h"
#include "module/ledCtrl.h"
#include "module/rx8035.h"
#include "module/BzCtrl.h"
#include "module/htu21d.h"
#include "module/i2cCtrl.h"
#include "module/motorCtrl.h"
#include "module/ESP32_eep.h"
#include "module/Arduino_Ini_Setup.h"

using namespace std;

ledCtrl LC;

void init();
void setHttpAction();
void setNowOpen(int open_time);
void setAfter(int next_time, int adjustment_time);
void setDevice(int contentNum);
void outputValue();
void outputTime();
string getTemp();
string getHumd();
void closeValve_Now();
int getNextTime();
void IRAM_ATTR setClosing();
void IRAM_ATTR afterOpenValve();

void taskMotor_W(void* arg);
void taskMotor_F(void* arg);

void IRAM_ATTR openMotorW();
void IRAM_ATTR openMotorF();
void IRAM_ATTR closeMotorW();
void IRAM_ATTR closeMotorF();
void IRAM_ATTR intervalStop();

void IRAM_ATTR onTimer();
void regularAction(deviceStatus& device, int& led_cnt, bool bl_food);
String GetResponsMessage(String target, String action, String item = "", String value = "");
int getNextTime(deviceStatus device);
void readEEP();
void modeSetting();


//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
void setup()
{
    sr.begin(115200);
    sr.println("----- START -----");
    //------------------------------------------------------------------
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
    (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Free heap size: %d bytes\n", esp_get_free_heap_size());
    printf("Internal free heap size: %d bytes\n", esp_get_free_internal_heap_size());
    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    uint32_t bytes = info.total_free_bytes + info.total_allocated_bytes;

    printf("PSRAM size: %d bytes\n", bytes);
    printf("Free PSRAM size: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    //------------------------------------------------------------------

    sr.println("----- PORT setting -----");
    initPort();
    LC.ledFlash(PORT_LED_F, 5, 3);
    LC.ledFlash(PORT_LED_W, 5, 3);
    digitalWrite(PORT_LED_F, LED_OFF);
    digitalWrite(PORT_LED_W, LED_OFF);
    initI2C(wr);

    sr.println("----- Buzzer setting -----");
    InitBz();
    bzPowerOn();


//-------------------------------------------------------
    sr.println("----- EEPROM setting -----");
    EEP_Init(ep, eepSize);
    sr.println("----- WiFi status EEPROM Write -----");

    if(digitalRead(PORT_SW) == LOW)
    {
       modeSetting();
    }

    wifiSts.ssid = EEP_Read(ep, wifiSts.eep_address_ssid, '*', ep.length());
    wifiSts.pass = EEP_Read(ep, wifiSts.eep_address_pass, '*', ep.length());
    wifiSts.host_name = EEP_Read(ep, wifiSts.eep_address_host_name, '*', ep.length());
//-------------------------------------------------------
    readEEP();
    wifiInit(WiFi, sr, wifiSts.ssid, wifiSts.pass, wifiSts.host_name, false);
    setHttpAction();
    sr.println("-----loop Start-----");
    
    // setVal.interval = 20;
    // operationReservation = enmRegularOppenning;
    // setAfter(setVal.interval, setVal.length);
    LC.ledFlash(PORT_LED_W, 5, 2);
    digitalWrite(PORT_LED_W, LED_OFF);
    LC.ledFlash(PORT_LED_F, 5, 2);
    digitalWrite(PORT_LED_F, LED_OFF);

    motorAction(MOTOR_OPEN, 20, false);
    motorAction(MOTOR_OPEN, 20, true);

    motorAction(MOTOR_CLOSE, 20, false);
    motorAction(MOTOR_CLOSE, 20, true);

    BzGoUp(5, 10);
    
    xTaskCreatePinnedToCore(taskMotor_W, "taskMotor_W", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(taskMotor_F, "taskMotor_F", 4096, NULL, 1, NULL, 0);

    // xTaskCreatePinnedToCore(
    //        タスク名,
    //        "タスク名",
    //        スタックメモリサイズ,
    //        NULL,
    //        タスク優先順位,
    //        タスクハンドルポインタ,
    //        Core ID
    // );

    setTimerInterrupt(deviceSts_W.tClose, deviceSts_W.timerNumClose, closeMotorW);
    setTimerInterrupt(deviceSts_F.tClose, deviceSts_F.timerNumClose, closeMotorF);
    setTimerInterrupt(deviceSts_W.tOpen, deviceSts_W.timerNumOpen, openMotorW);
    setTimerInterrupt(deviceSts_F.tOpen, deviceSts_F.timerNumOpen, openMotorF);

}

//*************************************
void loop()
{
    // if(deviceSts_W.serverInit && deviceSts_F.serverInit) server.handleClient();
    server.handleClient();
}


//*************************************
void taskMotor_W(void* arg)
{
    int cntLED = 0;
    while(1){
        cntLED++;
        regularAction(deviceSts_W, cntLED, false);
    }
}

//*************************************
void taskMotor_F(void* arg)
{
    int cntLED = 0;
    while(1)
    {
        cntLED++;
        regularAction(deviceSts_F, cntLED, true);
    }
}

void regularAction(deviceStatus& device, int& led_cnt, bool bl_food)
{
    int len;
    bool ringed = false;
    bool canRing = true;
    // deviceStatus *p_device;
    // if(bl_food) p_device = &deviceSts_F;
    // else p_device = &deviceSts_W;

    if(device.oppenning && !device.opened)
    {
        // device.serverInit = false;
        if(bl_food && deviceSts_W.ringing) canRing = false;
        else if(deviceSts_F.ringing) canRing = false;
        else if(setRinging) canRing = false;
        else canRing = true;

        if (canRing && device.ring && !device.ringing)
        {
            device.ringing = true;
            // p_device = &device;
            setHctMelody(sr, device.melody.c_str());
            device.ringing = false;
            ringed = true;
            // p_device = &device;
        }

        digitalWrite(device.portLED, LED_ON);
        device.oppenning = false;
        device.opened = true;

        // モータを動かす前にクロージングタイマーをセット
        // if(device.flgNow) len = device.nowLength;
        // else len = device.length;

        //クローズタイマーセット
        startTimerInterrupt(device.tClose, 3000000, false);
        
        motorAction(MOTOR_OPEN, MOTOR_RANGE, bl_food);
        device.flgNow = false;

        if(!ringed)
        {
            canRing = true;
            if(bl_food && deviceSts_W.ringing) canRing = false;
            else if(deviceSts_F.ringing) canRing = false;
            else if(setRinging) canRing = false;
            else canRing = true;
            if (canRing && device.ring && !device.ringing)
            {
                device.ringing = true;
                // p_device = &device;
                setHctMelody(sr, device.melody.c_str());
                device.ringing = false;
                ringed = true;
                // p_device = &device;
            }
        }
        
    }
    // if(device.closing && device.opened)
    if(device.closing && device.opened)
    {
        // device.serverInit = false;
        device.closing = false;
        device.opened = false;
        motorAction(MOTOR_CLOSE, MOTOR_RANGE, bl_food);
        digitalWrite(device.portLED, LED_OFF);
        // device.serverInit = true;
    }
    if(device.adjustment && !device.opened)
    {
        motorAction(device.adjustmentLeft, device.adjustMotion, bl_food);
        device.adjustment = false;
    }
    led_cnt++;
    if(led_cnt > 500)
    {
        led_cnt = 0;
        if(device.interval > 0 && !device.opened){
            digitalWrite(device.portLED, !digitalRead(device.portLED));
        }
    }
    delay(1);

}


//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************

//*************************************
void setHttpAction()
{
    //バルブオープン
    server.on(httpContents[enmNow], HTTP_ANY, [](){setDevice(enmNow);});
    //セット
    server.on(httpContents[enmSet], HTTP_ANY, [](){setDevice(enmSet);});

    //値取得
    server.on(httpContents[enmGet], HTTP_ANY, [](){outputValue();});

    //位置の調整
    server.on(httpContents[enmAdjust], HTTP_ANY, [](){setDevice(enmAdjust);});

    //wifi情報登録
    server.on(httpContents[enmWifi], HTTP_ANY, [](){setDevice(enmWifi);});
    //wifi情報登録Post
    server.on(httpContents[enmWF], HTTP_POST, [](){setDevice(enmWF);});

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        bzErrorSound();
        server.send(404, "text/plain", errorMessage[enmNotFound]); // 404を返す
    });

    server.begin();
}

//*************************************
void setDevice(int contentNum)
{
    deviceStatus device;
    deviceStatus *p_device;
    auto *func = &openMotorF;

    int portLED;

    String returnMessage = "";
    // String paramSts = server.arg("sts");
    String paramMelody = server.arg("melody");
    // String paramLength = server.arg("length");
    String paramInterval = server.arg("interval");
    String paramTarget = server.arg("target");
    String paramDirection = server.arg("direction");
    String paramRing = server.arg("ring");
    String paramMotion = server.arg("motion");

    String paramItem = server.arg("item");
    String paramValue = server.arg("value");
    // String paramSsid = server.arg("ssid");
    // String paramPass = server.arg("pass");
    // String paramHostname = server.arg("hostname");

    String Position = "water";
    switch(contentNum)
    {
        case enmNow://-------------------------------------------------------
            if(paramTarget == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
                sr.println("area = w");
                func = &openMotorW;
            }
            else if(paramTarget == "f")
            {
                sr.println("area = f");
                Position = "food";
                device = deviceSts_F;
                p_device = &deviceSts_F;
                func = &openMotorF;
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }
            
            if( device.opened == false && 
                device.oppenning == false && 
                device.closing == false)
            {
                bzReceivedRing();
                // if(paramLength != "") device.nowLength = atoi(paramLength.c_str());
                // else device.nowLength = DEFAULT_LENGTH;
                device.flgNow = true;
                device.oppenning = true;
                *p_device = device;
                returnMessage = GetResponsMessage(Position, "now");
                // returnMessage = "successed";
                // returnMessage = 
                //     "{%s:\"%s\",%s:\"%s\",%s:\"%s\",%s:\"%s\"}", 
                //     ResponsWord.Position, Position,
                //     ResponsWord.Motion, "\"now\"",
                //     ResponsWord.Item, "\"\"",
                //     ResponsWord.Value, "\"\"";

                // returnMessage += "{";
                // returnMessage += ResponsWord.Position + ":\"" + Position + "\",";
                // returnMessage += ResponsWord.Motion + ":\"now\",";
                // returnMessage += ResponsWord.Item + ":\"\",";
                // returnMessage += ResponsWord.Value + ":\"\"";
                // returnMessage += "}";
            }
            else{
                bzErrorSound();
                returnMessage = "error";
            }
            break;
        case enmSet://-------------------------------------------------------
            if(paramTarget == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
                func = &openMotorW;

                portLED = PORT_LED_W;
                // bzReceivedRing();
            }
            else if(paramTarget == "f")
            {
                device = deviceSts_F;
                p_device = &deviceSts_F;
                func = &openMotorF;
                Position = "food";

                portLED = PORT_LED_F;
                // bzReceivedRing();
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }

            if(paramMelody != "" || paramRing == "true" || paramRing == "false")
            {
                if(paramMelody != ""){
                    device.melody = paramMelody;
                    sr.println(paramMelody);

                    if(getNextTime(device) < 10){
                        while(deviceSts_F.ringing || deviceSts_W.ringing);
                        setRinging = true;
                        setHctMelody(sr, device.melody.c_str());
                        setRinging = false;
                    }
                    returnMessage = GetResponsMessage(Position, "set", "melody", device.melody);
                }
                if(paramRing == "true" || paramRing == "false")
                {
                    if(paramRing == "true") device.ring = true;
                    else device.ring = false;
                    returnMessage = GetResponsMessage(Position, "set", "ring", paramRing);
                }
                bzReceivedRing();
                *p_device = device;
                // returnMessage = "successed";

            }
            // else if(paramLength != "") {
            //     device.length = atoi(paramLength.c_str());
            //     *p_device = device;
            //     bzReceivedRing();
            //     // returnMessage = "successed";
            //     returnMessage = GetResponsMessage(Position, "set", "length", to_string(device.length).c_str());
            // }
            else if(paramInterval == "0")
            {
                sr.println("interval stop");
                device.interval =  0;
                // stopTimerInterrupt(p_t);
                // timerEnd(p_t);
                timerAlarmDisable(device.tOpen);

                digitalWrite(device.portLED, LED_OFF);
                *p_device = device;
                bzReceivedRing();
                // returnMessage = "successed";
                returnMessage = GetResponsMessage(Position, "set", "interval", to_string(device.interval).c_str());
            }
            else if(paramInterval != "") {
                device.interval = atoi(paramInterval.c_str());
                if(device.interval < 10)device.interval = 10;
                startTimerInterrupt(device.tOpen, device.interval * 1000000, true);
                device.setTime = GetTime();
                digitalWrite(device.portLED, LED_ON);
                *p_device = device;
                bzReceivedRing();
                // returnMessage = "successed";
                returnMessage = GetResponsMessage(Position, "set", "interval", to_string(device.interval).c_str());
            }
            // else if(paramRing == "true" || paramRing == "false")
            // {
            //     if(paramRing == "true")
            //         device.ring = true;
            //     else
            //         device.ring = false;
            //     *p_device = device;
            //     bzReceivedRing();
            //     returnMessage = "successed";
            // }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }
            break;

        case enmAdjust://--------------------------------------------------
            // bool blF = false;
            // bool blLeft = false;
            if(paramTarget == "w")
            { 
                device = deviceSts_W;
                p_device = &deviceSts_W;
            }
            else if(paramTarget == "f")
            {
                device = deviceSts_F;
                p_device = &deviceSts_F;
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
                break;
            }

            if( device.opened == false && 
                device.oppenning == false && 
                device.closing == false)
            {
                bzReceivedRing();

                if(paramDirection == "l") device.adjustmentLeft = true;
                else device.adjustmentLeft = false;

                if(paramMotion != "") sscanf(paramMotion.c_str(), "%d", &device.adjustMotion);

                device.adjustment = true;
                *p_device = device;
                returnMessage = "successed";
            }
            else
            {
                bzErrorSound();
                returnMessage = "error";
            }
            break;
        // case enmWifi:
        //     break;
        case enmWifi://--------------------------------------------------
            if(paramItem == "ssid")
            {
                sr.println(paramValue && paramValue != "");
                returnMessage = paramValue;
                EEP_Write(ep, wifiSts.eep_address_ssid, paramValue);
                bzReceivedRing();
            }
            else if(paramItem == "pass")
            {
                sr.println(paramValue && paramValue != "");
                returnMessage = paramValue;
                EEP_Write(ep, wifiSts.eep_address_pass, paramValue);
                bzReceivedRing();
            }
            else if(paramItem == "hostname" && paramValue != "")
            {
                sr.println(paramValue);
                returnMessage = paramValue;
                EEP_Write(ep, wifiSts.eep_address_host_name, paramValue);
                bzReceivedRing();
            }
            else
            {
                bzErrorSound();
                returnMessage = "wifi error";
            }
            break;
        case enmWF:
            if(server.hasArg("plain") == false);
            String body = server.arg("plain");
            // deserializeJson(jsonDocument, body);
            break;
    }
    server.send(200, "text/plain", returnMessage);
}

//*************************************
void outputValue()
{
    String returnMessage = "";
    String paramItem = server.arg("item");
    String paramTarget = server.arg("target");

    deviceStatus device;
    deviceStatus *p_device;

    String Position = "water";

    // if(paramSts == paramWord_get[enm_time])
    // {
    //     bzReceivedRing();
    //     struct tm nowTime = getTimeInf();
    //     char s[20] = {};
    //     arrangeTime(s, nowTime);
    //     returnMessage = s;
    //     // server.send(200, "text/plain", s);
    // }
    if(paramItem == paramWord_get[enm_temperture])
    {
        bzReceivedRing();
        returnMessage = getTemp().c_str();
    }
    else if(paramItem == paramWord_get[enm_humidity])
    {
        bzReceivedRing();
        returnMessage = getHumd().c_str();
    }
    // else if(paramItem == "dq")
    //     Bz_DragonQuest_Preface();
    else
    {
        if(paramTarget == "w")
        { 
            device = deviceSts_W;
            p_device = &deviceSts_W;
        }
        else if(paramTarget == "f")
        {
            device = deviceSts_F;
            p_device = &deviceSts_F;
            Position = "food";
        }
        else
        {
            bzErrorSound();
            returnMessage = "error";
            server.send(200, "text/plain", returnMessage);
            return;
        }
        if(paramItem == paramWord_get[enm_interval])
        {
            bzReceivedRing();
            // returnMessage = device.interval;
            returnMessage = GetResponsMessage(Position, "output", paramItem, to_string(device.interval).c_str());
        }
        else if(paramItem == paramWord_get[enm_ring])
        {
            bzReceivedRing();
            if(device.ring) returnMessage = GetResponsMessage(Position, "output", paramItem, "true");
            else returnMessage = GetResponsMessage(Position, "output", paramItem, "false");
        }
        else if(paramItem == paramWord_get[enm_melody])
        {
            bzReceivedRing();
            // returnMessage = device.melody;
            returnMessage = GetResponsMessage(Position, "output", paramItem, device.melody);
            setHctMelody(sr, device.melody.c_str());
            
        }
        else if(paramItem == paramWord_get[enm_next])
        {
            // sr.println(getNextTime(device));
            bzReceivedRing();
            // returnMessage = getNextTime(device);
            returnMessage = GetResponsMessage(Position, "output", paramItem, to_string(getNextTime(device)).c_str());
        }
        else if(paramItem == "settime")
        {
            bzReceivedRing();
            returnMessage = device.setTime;
            returnMessage = GetResponsMessage(Position, "output", paramItem, to_string(device.setTime).c_str());
        }
        else if(paramItem == paramWord_get[enm_all])
        {
            bzReceivedRing();
            // struct tm nowTime = getTimeInf();
            // arrangeTime(s, nowTime);
            // returnMessage = s;
            // returnMessage += "\r";
            // returnMessage += getTemp().c_str();
            // returnMessage += "\r";
            // returnMessage += getHumd().c_str();

            String ringVal = "";
            if(device.ring) ringVal = "true";
            else ringVal = "false";
            returnMessage += "[";
            returnMessage += GetResponsMessage(Position, "output", "interval", to_string(device.interval).c_str()) + ",";
            returnMessage += GetResponsMessage(Position, "output", "ring", ringVal) + ",";
            returnMessage += GetResponsMessage(Position, "output", "melody", device.melody) + ",";
            returnMessage += GetResponsMessage(Position, "output", "next", to_string(getNextTime(device)).c_str());
            returnMessage += "]";
        }
        else{
            bzErrorSound();
            returnMessage = errorMessage[enmStsError_get];
        }
    }
    server.send(200, "text/plain", returnMessage);
}

String GetResponsMessage(String target, String action, String item, String value)
{
    String resultMessage = "";
    resultMessage += "{";
    resultMessage += ResponsWord.Target + ":\"" + target + "\",";
    resultMessage += ResponsWord.Action + ":\"" + action + "\",";
    resultMessage += ResponsWord.Item + ":\"" + item + "\",";
    resultMessage += ResponsWord.Value + ":\"" + value + "\"";
    resultMessage += "}";
    return resultMessage;
}

//*************************************
void IRAM_ATTR openMotorW()
{
    deviceSts_W.setTime = GetTime();
    deviceSts_W.oppenning = true;
} 

//*************************************
void IRAM_ATTR openMotorF()
{
    deviceSts_F.setTime = GetTime();
    deviceSts_F.oppenning = true;
} 

//*************************************
void IRAM_ATTR closeMotorW()
{
    deviceSts_W.closing = true;
} 
//*************************************
void IRAM_ATTR closeMotorF()
{
    deviceSts_F.closing = true;
} 

//*************************************
void IRAM_ATTR intervalStop(){ } 

//*************************************
string getTemp()
{
    float fTemp = readTempHTD21D(wr);
    sr.println(fTemp);
    return to_string(fTemp).substr(0, 5);
}

//*************************************
string getHumd()
{
    float fHumd = readHumdHTD21D(wr);
    return to_string(fHumd).substr(0, 5);
}

//*************************************
int getNextTime(deviceStatus device)
{
    if(device.interval == 0)
    {
        return 0;
    }
    else
    {
        long nowT = GetTime();
        long setT = device.setTime;
        sr.print("set time：");
        sr.println(setT);
        sr.print("now time：");
        sr.println(nowT);
        sr.print("result：");
        sr.println(device.interval - ((nowT - setT)/1000));
        
        if(nowT < setT) nowT + 4294967295;
        return device.interval - ((nowT - setT)/1000);
    }
}

void readEEP()
{
    sr.println("----- Read EEPROM-----");
    sr.print("ssid : ");
    sr.println(EEP_Read(ep, wifiSts.eep_address_ssid, '*', ep.length()));
    sr.print("pass : ");
    sr.println(EEP_Read(ep, wifiSts.eep_address_pass, '*', ep.length()));
    sr.print("host name : ");
    sr.println(EEP_Read(ep, wifiSts.eep_address_host_name, '*', ep.length()));
}


// }
//*************************************
// void IRAM_ATTR setClosing_Regular()
// {
//     deviceSts.closingRun = true;
// }

//*************************************
// void IRAM_ATTR afterOpenValve()
// {
//     openTime = setVal.length * 1000000;
//     deviceSts.regularOppenning = true;
// }



// *************************************
void modeSetting()
{
    BluetoothSerial Serial_BT;
    HardwareSerial Serial00 = Serial;

    sr.println("********* Setting mode *********");
    BzGoDown(5, 10);
    digitalWrite(PORT_LED_W, LED_ON);
    digitalWrite(PORT_LED_F, LED_ON);
    // EEP_Write(ep, wifiSts.eep_address_ssid ,SSID);
    // EEP_Write(ep, wifiSts.eep_address_pass ,PASS);
    // EEP_Write(ep, wifiSts.eep_address_host_name ,HOST_NAME);
    sr.println("Reset wifi status");
    //Bluetoothのセット--------------------
    // if (!mESP32_BLSerial_Set(Serial_BT, "BT_um", Serial00))
    if(!Serial_BT.begin("BT32"))
    {
        //digitalWrite(BLUE_LED, 0);
        sr.print("-----Bluetooth err-----");
        while (true);
    }
    else
    {
        sr.print("-----Bluetooth set-----");

        //digitalWrite(BLUE_LED, 1);
    }
    while (true)
    {

    }
}

//*************************************
//現在時刻取得
void outputTime()
{
    bzReceivedRing();
    struct tm nowTime = getTimeInf();
    char s[20] = {};
    arrangeTime(s, nowTime);
    server.send(200, "text/plain", s);
}