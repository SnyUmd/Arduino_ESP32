//bmp280.hのBPM280_ADDRESSを確認(電源投入後にLEDが点滅し続ける場合はここが怪しい)
//defStc000.hのBLE_NAMEを確認

#include "main.hpp"
//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
void setup()
{
    sr.begin(115200);

#pragma region test0
    // int test_data = 0x115599;
    // int buf_;
    // sr.print("msb :");
    // sr.println(test_data >> 16);
    // sr.print("lsb :");
    // sr.println((test_data & 0x100FFFF) >> 8);
    // sr.print("crc :");
    // sr.println(test_data & 0x10000FF);

    // gTest = 200;
    // sr.println(clsBh1750.getTest());
    // clsBh1750.globalTest(sr);
    // clsBh1750.setTest(gTest);
    // sr.println(clsBh1750.getTest());
    // clsBh1750.globalTest(sr);

    // while(1);

#pragma endregion

    sr.println("----- Start -----");

    sr.println("----- PORT setting -----");
    initPort();

    sr.println("----- Json setting -----");
    setJsonSensor();
    serializeJson(jsonSensor, outputtext, 100);
    sr.println(outputtext);

    sr.println("----- I2C setting -----");
    Wire.begin();

    delay(100);
    set_sensorInstallationStatus(Wire);
    sr.println("success set device installation status");
    for(IsDevice isD : isDevice){
        sr.print("0x");
        sr.print(isD.address, HEX);
        sr.print(", ");
        sr.println((isD.isCommunication ? "true" : "false"));
    }


    // while(true) delay(250);

    if(isDevice[enmTempHumd_Htu21].isCommunication);
    if(isDevice[enmPresure_Bmp280].isCommunication){
        if(!InitBmp280(bmp, Wire, sr))
        {
            // while(1)
            for(int i = 0; i < 10; i++)
            {
                digitalWrite(PORT_LED, !digitalRead(PORT_LED));
                delay(500);
                if(InitBmp280(bmp, Wire, sr)) break;
            }
            digitalWrite(PORT_LED, LED_OFF);
        }
    }
    if(isDevice[enmIllumi_Bh1750fvi].isCommunication)
        clsBh1750.initBh1750fvi(Wire);
    if(isDevice[enmCo2_Scd4x].isCommunication)
        initScd4x(Wire);

#pragma region InputTest
    // String sss = "sw down";
    // while (true)
    // {
    //     if(digitalRead(PORT_SW_DEBUG) == LOW)
    //         sr.printf("%s\r\n", sss);
    //     delay(200);
    // }

#pragma endregion
#pragma region testBh1750//照度測定テスト
    //照度測定テスト
    // while(true){
    //     if(digitalRead(PORT_SW_DEBUG) == LOW)
    //     {
    //         if(modeBh1750 >= 2) modeBh1750 = 0;
    //         else modeBh1750++;
    //         sr.print("change mode BH1750 : ");
    //         sr.println(modeBh1750);
    //     }

    //     if(digitalRead(PORT_SW) == LOW){

    //         float test_f = clsBh1750.readIllumi_Bh1750fvi_toFloat(Wire, modeBh1750, sr);
    //         String buf_s = clsBh1750.readIllumi_Bh1750fvi_toString(Wire, modeBh1750, sr);

    //         // sr.println(clsBh1750.readIllumi_Bh1750fvi_toFloat(Wire, modeBh1750, sr));
    //         sr.println(buf_s);
    //     }

    //     delay(200);
    // }
    // while(true);//**************************************************************************
#pragma endregion

    sr.println("----- Buzzer setting -----");
    InitBz();
    bzPowerOn();


#pragma region EEP setting//----------------------------------------------
    sr.println("----- EEPROM setting -----");
    EEP_Init(eep, eepSize);
    EEP_Write2(eep, wifiSts.eep_address_measurement_num, "0");
    sr.println("----- Setting mode -----");
    // modeSetting();//セッティングモード
    wifiStatusIntake();//WiFi情報を変数に格納
    readEEP();//Flashの記録情報を出力
    digitalWrite(PORT_LED, LED_OFF);

    // wifiStatusIntake();
#pragma endregion//-------------------------------------------------------

#pragma region WiFi setting//----------------------------------------------
    sr.println("----- WiFi setting -----");

    if(!wifiInit(wf, sr, wifiSts.ssid, wifiSts.pass, wifiSts.host_name, false))
    {
        sr.println("wifi networ error");
        ledOkOrNg(false);
        sr.println("software reset");
        ESP.restart();
    }

    EEP_Write2(eep, wifiSts.eep_address_start_time, getTimeWifi());
    prgStartTime = getTimeWifi();
    digitalWrite(PORT_LED, !digitalRead(PORT_LED));
    delay(25);
    digitalWrite(PORT_LED, !digitalRead(PORT_LED));
    delay(25);
    digitalWrite(PORT_LED, !digitalRead(PORT_LED));
    delay(25);
    digitalWrite(PORT_LED, !digitalRead(PORT_LED));
    delay(25);


#pragma endregion//-------------------------------------------------------

#pragma region server setting//----------------------------------------------
    sr.println("----- Server setting -----");
    // server.begin(8000);
    setHttpAction(wifiSts.portNum.toInt());
    sr.print("Port number : ");
    sr.println(wifiSts.portNum);

#pragma endregion//-------------------------------------------------------

    BzGoUp(5, 10);


#pragma region task //未使用
    // void task0(void* arg){}
    // void task1(void* arg){}
    //タスク実行
    // xTaskCreatePinnedToCore(task0, "task0", 4096, NULL, 1, NULL, 0);
    // xTaskCreatePinnedToCore(task1, "task1", 4096, NULL, 1, NULL, 0);

    // xTaskCreatePinnedToCore(
    //        タスク関数,
    //        "タスク名",
    //        スタックメモリサイズ,
    //        NULL,
    //        タスク優先順位,
    //        タスクハンドルポインタ,
    //        Core ID
    // );

#pragma endregion
#pragma region timer//未使用
    // void IRAM_ATTR fn0(){}
    // void IRAM_ATTR fn1(){}
    // void IRAM_ATTR fn2(){}
    // void IRAM_ATTR fn3(){}

    //タイマー割込みセット
    //hw_timer_t* timer0 = NULL, timer1 = NULL, timer2 = NULL, timer3 = NULL;
    // setTimerInterrupt(timer0, 0, fn0);
    // setTimerInterrupt(timer1, 1, fn1);
    // setTimerInterrupt(timer2, 2, fn2);
    // setTimerInterrupt(timer3, 3, fn3);

    //タイマーを止めるとき
    // timerAlarmDisable(timer0);
    //タイマーをセットするとき
    // startTimerInterrupt(timer0, 3 * 1000000, false);※3秒後にタイマー割込み


    setTimerInterrupt(tLightOff, tLightOff_Num, lightOff);
    sr.println("Successed set light timer init");

    setTimerInterrupt(tLedFlash, tLedFlash_Num, ledFlash);
    sr.println("Successed set led flash timer init");

#pragma endregion

#pragma region test1//----------------------------------------------------
/*
    sr.println("+++++test1 start+++++");

    float pressure = readPressBmp280(bmp);
    float temp = getTemp_htu21d();
    float humd = getHumd_htu21d();
    String ill = clsBh1750.readIllumi_Bh1750fvi_toString(wr, modeBh1750);

    sr.printf("press:%4.0fhPa\r\n", pressure / 100);

    float a = getTemp_htu21d();
    float b = getHumd_htu21d();
    sr.printf("temp2 : %2.2f ℃\r\n", temp);
    sr.printf("humd : %2.2f ％\r\n", humd);
    sr.printf("illuminance : %s lx\r\n", ill);

    sr.println("+++++test1 end+++++");
    // while(1);
*/
#pragma endregion//-------------------------------------------------------


    sr.println("-----loop Start-----");
    digitalWrite(PORT_LED, LED_ON);
    delay(2000);
    digitalWrite(PORT_LED, LED_OFF);
}

//*************************************
void loop()
{
    server.handleClient();//サーバ動作

    if(!digitalRead(PORT_SW_DEBUG)){
        delay(300);
        if(!timerAlarmEnabled(tLedFlash)){
            startTimerInterrupt(tLedFlash, ledFlashStepTime, true);
        }
        else{
            stopTimerInterrupt(tLedFlash);
            digitalWrite(PORT_LED, LED_OFF);
        }
    }

    /*
    if(!digitalRead(PORT_SW_DEBUG) && digitalRead(PORT_LED) == LED_OFF)
    {
        delay(50);
        digitalWrite(PORT_LED, !digitalRead(PORT_LED));
        lightingStartTime = GetTime();
    }
    else if(digitalRead(PORT_LED) == LED_ON){
        if(CheckElapsedTime(lightingStartTime, lightOffTime))
            digitalWrite(PORT_LED, LED_OFF);
    }
    */
   delay(5);
}


//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************

//*************************************
void setHttpAction(int port_num)
{
    //読込
    server.on("/read", HTTP_ANY, [](){
        // bzReceivedRing(); 
        digitalWrite(PORT_ACCESS, LOW);
        httpAction(0);
        digitalWrite(PORT_ACCESS, HIGH);});
    //セット
    server.on("/set", HTTP_ANY, [](){bzReceivedRing(); httpAction(1);});

    server.on("/test", HTTP_ANY, [](){httpAction(99);});

    server.on("/reboot", HTTP_ANY, [](){httpAction(2);});

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        ledOkOrNg(false);
        bzErrorSound();
        server.send(404, "text/plain; charset=utf-8", "Not found"); // 404を返す
    });

    server.begin(port_num);
}

//*************************************
void httpAction(int contentNum)
{
    int portLED;

    String returnMessage = "";
    String paramItem = server.arg("item");
    switch(contentNum)
    {
        case 0://read-------------------------------------------------------
            if(paramItem == "temp")
            { 
                ledOkOrNg(true);
                sr.println("receive temp");
                // bzReceivedRing();
                returnMessage = "Device : ";
                returnMessage += wifiSts.host_name;
                returnMessage += "\r\n";
                returnMessage += getTimeWifi();
                returnMessage += "\r\n";
                returnMessage += "temperature : ";
                returnMessage += String(getTemp_htu21d(), 1);
                returnMessage += " ℃";
                returnMessage += " \r\n";
                
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "Start time : ";
                returnMessage += prgStartTime;
                returnMessage += " hpa\r\n";
                returnMessage += " ----------------";
                returnMessage += " hpa\r\n";
                returnMessage += "access num : ";
                accessNum++;
                returnMessage += accessNum;
            }
            else if(paramItem == "humd")
            {
                ledOkOrNg(true);
                sr.println("receive humd");
                // bzReceivedRing();
                returnMessage = "Device : ";
                returnMessage += wifiSts.host_name;
                returnMessage += "\r\n";
                returnMessage += getTimeWifi();
                returnMessage += "\r\n";
                returnMessage += "humidity : ";
                returnMessage += String(getHumd_htu21d(), 0);
                returnMessage += " %HR";
                returnMessage += "\r\n";
                
                returnMessage += "----------------";
                returnMessage += "\r\n";
                returnMessage += "Start time : ";
                returnMessage += prgStartTime;
                returnMessage += "\r\n";
                returnMessage += "----------------";
                returnMessage += "\r\n";
                returnMessage += "access num : ";
                accessNum++;
                returnMessage += accessNum;
            }
            else if(paramItem == "press")
            {
                ledOkOrNg(true);
                sr.println("receive press");
                // bzReceivedRing();
                returnMessage = "Device : ";
                returnMessage += wifiSts.host_name;
                returnMessage += "\r\n";
                returnMessage += getTimeWifi();
                returnMessage += "\r\n";
                returnMessage += "Atmospheric pressure : ";
                returnMessage += String(readPressBmp280(bmp) /100, 0);
                returnMessage += " hpa";
                returnMessage += "\r\n";
                
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "Start time : ";
                returnMessage += prgStartTime;
                returnMessage += "\r\n";
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "access num : ";
                accessNum++;
                returnMessage += accessNum;
            }
            else if(paramItem == "ill")
            {
                ledOkOrNg(true);
                sr.println("receive Illuminance");
                // bzReceivedRing();
                returnMessage = "Device : ";
                returnMessage += wifiSts.host_name;
                returnMessage += "\r\n";
                returnMessage += getTimeWifi();
                returnMessage += "\r\n";
                returnMessage += "Illuminance : ";
                returnMessage += clsBh1750.readIllumi_Bh1750fvi_toString(wr, modeBh1750);
                returnMessage += " lx";
                returnMessage += "\r\n";
                
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "Start time : ";
                returnMessage += prgStartTime;
                returnMessage += "\r\n";
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "access num : ";
                accessNum++;
                returnMessage += accessNum;
            }
            else if(paramItem == "co2")
            {
                ledOkOrNg(true);
                sr.println("receive co2");
                // bzReceivedRing();
                returnMessage = "Device : ";
                returnMessage += wifiSts.host_name;
                returnMessage += "\r\n";
                returnMessage += getTimeWifi();
                returnMessage += "\r\n";
                returnMessage += "CO2 : ";
                returnMessage += readCO2_scd4x(Wire);
                returnMessage += " ppm";
                returnMessage += "\r\n";
                
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "Start time : ";
                returnMessage += prgStartTime;
                returnMessage += "\r\n";
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "access num : ";
                accessNum++;
                returnMessage += accessNum;
            }
            else if(paramItem == "all")
            {
                ledOkOrNg(true);
                sr.println("receive all");
                // bzReceivedRing();

                returnMessage = "Device : ";
                returnMessage += wifiSts.host_name;
                returnMessage += "\r\n";
                returnMessage += getTimeWifi();
                returnMessage += "\r\n";

                returnMessage += "Temperature : ";
                returnMessage += String(getTemp_htu21d(), 1);
                returnMessage += " ℃\r\n";

                returnMessage += "Humidity : ";
                returnMessage += String(getHumd_htu21d(), 0);
                returnMessage += " %RH\r\n";

                returnMessage += "Illuminance : ";
                returnMessage += clsBh1750.readIllumi_Bh1750fvi_toString(Wire, modeBh1750);
                returnMessage += " lx\r\n";

                returnMessage += "Atmospheric pressure : ";
                returnMessage += String(readPressBmp280(bmp) /100, 0);
                returnMessage += " hpa\r\n";

                returnMessage += "CO2 : ";
                returnMessage += readCO2_scd4x(Wire);
                returnMessage += " ppm";
                returnMessage += "\r\n";

                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "Start time : ";
                returnMessage += prgStartTime;
                returnMessage += "\r\n";
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "access num : ";
                accessNum++;
                returnMessage += accessNum;
            }
            else if(paramItem == "access_num")
            {
                // bzReceivedRing();
                bool blEeepW = false;
                returnMessage = "Device : ";
                returnMessage += wifiSts.host_name;
                returnMessage += "\r\n";
                returnMessage += "Start time : ";
                returnMessage += prgStartTime;
                returnMessage += "\r\n";
                returnMessage += " ----------------";
                returnMessage += "\r\n";
                returnMessage += "access num : ";
                accessNum++;
                returnMessage += accessNum;
            }
            else if(paramItem == "light"){
                if(digitalRead(PORT_LED) == LED_ON){
                    startTimerInterrupt(tLightOff, lightOffTime, false);
                    returnMessage = "LED lighting now \n";
                    returnMessage += "reset lighting time";
                }
                else{
                    digitalWrite(PORT_LED, LED_ON);
                    startTimerInterrupt(tLightOff, lightOffTime, false);
                    returnMessage = "LED lighting\n";
                    returnMessage += "set lighting time";
                }
            }
            else if(paramItem == "test"){
                // bzReceivedRing();
                // returnMessage = "{\"code\": 200,\"data\": {\"pref\": \"福岡県\",\"address\": \"福岡市南区塩原\",\"city\": \"福岡市南区\",\"town\": \"塩原\",\"fullAddress\": \"福岡県福岡市南区塩原\"}}";
                returnMessage = "test";
            }
            else{
                ledOkOrNg(false);
                bzErrorSound();
                returnMessage = "error";
            }
            break;
        case 1://set-------------------------------------------------------
            if(paramItem == "ssid")
            {
                ledOkOrNg(true);
                bzReceivedRing();

            }
            else if(paramItem == "pass")
            {
                ledOkOrNg(true);
                bzReceivedRing();

            }
            else if(paramItem == "hostname")
            {
                ledOkOrNg(true);
                bzReceivedRing();

            }
            else
            {
                ledOkOrNg(true);
                bzErrorSound();
                returnMessage = "error";
                break;
            }

        case 2://reboot-------------------------------------------------------

            break;
        case 99:
            returnMessage = "test";

    }

    //デバイスへ直接アクセス時のCORSブロック回避のヘッダ************
    server.sendHeader("Access-Control-Allow-Origin", "*", false);
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type", false);
    //^************************************************

    //ngrok経由通信でのCORSブロック回避のヘッダ************
    server.sendHeader("Authentication", "ngrok-skip-browser-warning", true);
    // server.sendHeader("Content-Type", "multipart/form-data", true);//これにするとファイルとして保存する動きになる。
    //^************************************************
    server.send(200, "text/plain; charset=utf-8", returnMessage);
    bzReceivedRing();
    // digitalWrite(PORT_ACCESS, HIGH);
}


//*************************************
// string getTemp()
// {
//     float fTemp = readTempHTD21D(Wire, i2cCtrl);
//     return to_string(fTemp).substr(0, 5);
// }
float getTemp_htu21d()
{
    float fTemp = readTempHTD21D(Wire);
    return fTemp;
}

//*************************************
// string getHumd()
// {
//     float fHumd = readHumdHTD21D(Wire);
//     return to_string(fHumd).substr(0, 5);
// }
float getHumd_htu21d()
{
    float fHumd = readHumdHTD21D(Wire);
    return fHumd;
}


//*************************************
//現在時刻取得
// void outputTime()
// {
//     bzReceivedRing();
//     struct tm nowTime = getTimeInf();
//     char s[20] = {};
//     arrangeTime(s, nowTime);
//     server.send(200, "text/plain", s);
// }
String getTimeWifi()
{
    struct tm nowTime = getTimeInf();
    char s[20] = {};
    arrangeTime(s, nowTime);
    return s;
}


//*************************************
void readEEP()
{
    sr.println("----- Read EEPROM-----");
    sr.print("ssid : ");
    sr.println(EEP_Read(eep, wifiSts.eep_address_ssid, '*', eep.length()));
    sr.print("pass : ");
    sr.println(EEP_Read(eep, wifiSts.eep_address_pass, '*', eep.length()));
    sr.print("host name : ");
    sr.println(EEP_Read(eep, wifiSts.eep_address_host_name, '*', eep.length()));
    sr.print("port number : ");
    sr.println(EEP_Read(eep, wifiSts.eep_address_port_num, '*', eep.length()));
}

//*************************************
void wifiStatusReset()
{
    EEP_Write2(eep, wifiSts.eep_address_ssid ,SSID_0);
    EEP_Write2(eep, wifiSts.eep_address_pass ,PASS_0);
    EEP_Write2(eep, wifiSts.eep_address_host_name ,HOST_NAME);
    wifiStatusIntake();
    sr.println("Reset wifi status");
}

//*************************************
void wifiStatusIntake()
{
    wifiSts.ssid = EEP_Read(eep, wifiSts.eep_address_ssid, '*', eep.length());
    wifiSts.pass = EEP_Read(eep, wifiSts.eep_address_pass, '*', eep.length());
    wifiSts.host_name = EEP_Read(eep, wifiSts.eep_address_host_name, '*', eep.length());
    wifiSts.portNum = EEP_Read(eep, wifiSts.eep_address_port_num, '*', eep.length());
}


// **************************************************************************
// **************************************************************************
// **************************************************************************

long timeSetting = GetTime();
const long endTime = 30000;
int timeout0 = 180000;
int timeout1 = 10000;

// int timeout0 = 5000;
// int timeout1 = 10000;
// *************************************

void modeSetting()
{
    bool blNum = false;
    BluetoothSerial srBT;
    HardwareSerial Serial00 = Serial;

    timeSetting = GetTime();
    sr.println("********* Setting mode *********");
    digitalWrite(PORT_LED, LED_ON);

    //Bluetoothのセット--------------------
    // if (!mESP32_BLSerial_Set(srBT, "BT_um", Serial00))
    if(!srBT.begin(DEVICE_NUM))
    {
        //digitalWrite(BLUE_LED, 0);
        sr.println("-----Bluetooth err-----");
        bzErrorSound();
        while (true);
    }
    else
    {
        sr.println("-----Bluetooth set-----");
        BzGoDown(5, 10);
        digitalWrite(PORT_LED, LED_ON);
    }
    String readBuf = "";

    int* p_address;
    String* p_sBuf;
    while (true)
    {
        blNum = false;
        timeSetting = GetTime();
        
        srBT.println("==================");
        srBT.println("s : wifi start");
        srBT.println("r : wifi status read");
        srBT.println("w : wifi status write");
        srBT.println("------------------");
        srBT.print("-> ");
        readBuf = serialRead(srBT, '\n', timeout0);

        if(readBuf == "")
        {
            ledOkOrNg(false);
            if(CheckElapsedTime(timeSetting, timeout0)) 
            {
                srBT.println("setting time out");
                sr.println("setting time out");
                break;
            }
            else continue;
        }
        else if(readBuf.indexOf(BTcommand[enm_start]) >= 0 || readBuf == "stop") //s
        {
            ledOkOrNg(true);
            sr.println("setting mode end");
            srBT.println("setting mode end");
            break;
        }
        else if(readBuf.indexOf(BTcommand[enm_read]) >= 0)//r
        {
            ledOkOrNg(true);
            srBT.print("ssid : ");
            // srBT.println(wifiSts.ssid);
            srBT.println(EEP_Read(eep, wifiSts.eep_address_ssid, '*', eep.length()));
            srBT.print("pass : ");
            // srBT.println(wifiSts.pass);
            srBT.println(EEP_Read(eep, wifiSts.eep_address_pass, '*', eep.length()));
            srBT.print("host name : ");
            // srBT.println(wifiSts.host_name);
            srBT.println(EEP_Read(eep, wifiSts.eep_address_host_name, '*', eep.length()));
            srBT.print("port number : ");
            // srBT.println(wifiSts.portNum);
            srBT.println(EEP_Read(eep, wifiSts.eep_address_port_num, '*', eep.length()));
            srBT.println("");
        }
        else if(readBuf.indexOf(BTcommand[enm_write]) >= 0)//w
        {
            ledOkOrNg(true);
            srBT.println("1 : ssid");
            srBT.println("2 : pass");
            srBT.println("3 : host name");
            srBT.println("4 : port number");
            srBT.println("0 : wifi 0");
            // srBT.println("8 : wifi1");
            // srBT.println("9 : wifi2");
            srBT.println("------------------");
            srBT.print("number -> ");
            // timeSetting = GetTime();
            readBuf = serialRead(srBT, '\n', timeout1);
            // timeSetting = GetTime();
            if(readBuf == "" || readBuf == "stop")
            {
                ledOkOrNg(false);
                continue;
            }
            else if(readBuf == "1")
            {
                ledOkOrNg(true);
                p_address = &wifiSts.eep_address_ssid;
                p_sBuf = &wifiSts.ssid;
            }
            else if(readBuf == "2"){
                ledOkOrNg(true);
                p_address = &wifiSts.eep_address_pass;
                p_sBuf = &wifiSts.pass;
            } 
            else if(readBuf == "3")
            {
                ledOkOrNg(true);
                p_address = &wifiSts.eep_address_host_name;
                p_sBuf = &wifiSts.host_name;
            }
            
            else if(readBuf == "4")
            {
                ledOkOrNg(true);
                blNum = true;
                p_address = &wifiSts.eep_address_port_num;
                p_sBuf = &wifiSts.portNum;
            }
            else if(readBuf == "0")
            {
                ledOkOrNg(true);
                EEP_Write2(eep, wifiSts.eep_address_ssid,SSID_1);
                EEP_Write2(eep, wifiSts.eep_address_pass,PASS_1);
                EEP_Write2(eep, wifiSts.eep_address_host_name,HOST_NAME);
                wifiStatusIntake();
                sr.println("set wifi 1");
                srBT.println("set wifi 1");
                continue;
            }
            else if(readBuf == "umd0")
            {
                ledOkOrNg(true);
                EEP_Write2(eep, wifiSts.eep_address_ssid,SSID_0);
                EEP_Write2(eep, wifiSts.eep_address_pass,PASS_0);
                EEP_Write2(eep, wifiSts.eep_address_host_name,HOST_NAME);
                wifiStatusIntake();
                sr.println("set wifi umd0");
                srBT.println("set wifi umd0");
                continue;
            }
            else if(readBuf == "umd1")
            {
                ledOkOrNg(true);
                EEP_Write2(eep, wifiSts.eep_address_ssid,SSID_2);
                EEP_Write2(eep, wifiSts.eep_address_pass,PASS_2);
                EEP_Write2(eep, wifiSts.eep_address_host_name,HOST_NAME);
                wifiStatusIntake();
                sr.println("set wifi umd1");
                srBT.println("set wifi umd1");
                continue;
            }
            else
            {
                ledOkOrNg(false);
                continue;
            }
            srBT.println("------------------");
            srBT.print("value -> ");
            // timeSetting = GetTime();
            readBuf = serialRead(srBT, '\n', 10000);
            if(blNum && readBuf != String(readBuf.toInt())) //数字以外が入力されたとき
            {
                ledOkOrNg(false);
                sr.println("port num error");
                srBT.println("port num error");
                continue;
            }
            else if(readBuf == "" || readBuf == "stop")
            {
                ledOkOrNg(false);
                continue;
            }
            else
            {
                ledOkOrNg(true);
                EEP_Write2(eep, *p_address, readBuf);
                *p_sBuf = readBuf;
                srBT.println("completion");
            }
        }
        // timeSetting = GetTime();
    }
    wifiStatusIntake();//WiFi情報を変数に格納
    readEEP();//Flashの記録情報を出力
    digitalWrite(PORT_LED, LED_OFF);
}



//*************************************
String serialRead(BluetoothSerial& srBT ,char c_end, long timeout)
{
    String result = "";
    timeSetting = GetTime();
    while (true)
    {
        if(digitalRead(PORT_SW) == LOW)
        {
            return "stop";
        }
        if(CheckElapsedTime(timeSetting, timeout)) 
        {
            srBT.println("time out");
            sr.println("time out");
            return "";
        }
        if(srBT.available())
        {
            timeSetting = GetTime();
            result += srBT.readString();
            if(result.indexOf(c_end) >= 0)
            {
                while(1)
                {
                    int crPoint = result.indexOf("\r");
                    if(crPoint >= 0)
                        result.remove(crPoint, 1);//crlfを削除
                    else break;
                }
                while(1)
                {
                    int lfPoint = result.indexOf("\n");
                    if(lfPoint >= 0)
                        result.remove(lfPoint, 1);//crlfを削除
                    else break;
                }
                return result;
            }
        }
    }
}


//*************************************
void ledOkOrNg(bool is_ok)
{
    int loopNum = 0;
    int waitTime = 0;

    if(is_ok)
    {
        loopNum = 1;
        waitTime = 50;
    }
    else
    {
        loopNum = 5;
        waitTime = 30;
    }

    for(int i = 0; i < loopNum; i++)
    {
        digitalWrite(PORT_LED, !digitalRead(PORT_LED));
        delay(waitTime);
        digitalWrite(PORT_LED, !digitalRead(PORT_LED));
        delay(waitTime);
    }
}

//*************************************
void setJsonSensor()
{
    jsonSensor[sensorItem[enmDeviceName]] = DEVICE_NUM;
    jsonSensor[sensorItem[enmTime]] = "";
    jsonSensor[sensorItem[enmButtery]] = 3;
    jsonSensor[sensorItem[enmTemp]] = "";
    jsonSensor[sensorItem[enmHumd]] = "";
    jsonSensor[sensorItem[enmPress]] = "";
    jsonSensor[sensorItem[enmCo2]] = "";
    jsonSensor[sensorItem[enmIllumi]] = "";
}


//*************************************
//*************************************
void scd4x_Mesurement()
{
    int scd4x_Address = 0x62;
    int temp_data = 0;
    int rh_data = 0;
    int co2_data = 0;

    float fRH;
    float fTemp;
    float fCO2;
    byte val_scd4x[10];
    delay(2000);

    Wire.beginTransmission(scd4x_Address);
    Wire.write(0x21);
    Wire.write(0xb1);
    Wire.endTransmission();
    delay(6000);

    Wire.beginTransmission(scd4x_Address);
    Wire.write(0xec);
    Wire.write(0x05);
    Wire.endTransmission();
    delay(2000);
    Wire.requestFrom(scd4x_Address, 9);
    delay(2000);

    for(int i = 0; i < 9; i++)
    {
        val_scd4x[i] = Wire.read();
    }

    // rh_data = val_scd4x[2] << 16 +  val_scd4x[1] << 8 +  val_scd4x[0];
    rh_data = val_scd4x[6] << 8 | val_scd4x[7];
    fRH = 100 * rh_data / (float)65536;
    sr.print("RH : ");
    sr.println(fRH);
    sr.println(int(val_scd4x[7]));
    sr.println(int(val_scd4x[8]));
    sr.println(int(val_scd4x[9]));

    // temp_data = val_scd4x[5] << 16 +  val_scd4x[4] << 8 +  val_scd4x[3];
    temp_data = val_scd4x[4] << 8 | val_scd4x[5];
    fTemp = -45 + 175 * temp_data / (float)65536;
    sr.print("Temp: ");
    sr.println(fTemp);
    sr.println(int(val_scd4x[4]));
    sr.println(int(val_scd4x[5]));
    sr.println(int(val_scd4x[6]));

    // co2_data = val_scd4x[2] << 16 +  val_scd4x[1] << 8 +  val_scd4x[0];
    co2_data = val_scd4x[0] << 8 | val_scd4x[1];
    sr.print("CO2 : ");
    sr.println(co2_data);

    sr.println(int(val_scd4x[0]));
    sr.println(int(val_scd4x[1]));
    sr.println(int(val_scd4x[2]));

    Wire.beginTransmission(scd4x_Address);
    Wire.write(0x3f);
    Wire.write(0x86);
    Wire.endTransmission();
}


//*************************************
//*************************************
// void BMP280_Mesurment()
// {
//     bool bl_test = true;

//     int osrs_t_f4 = 0b001 << 5;//001:16bit, 010:17bit, 011:18bit, 100:19bit, 101~111:20bit
//     int osrs_p_f4 = 0b001 << 2;//001:16bit, 010:17bit, 011:18bit, 100:19bit, 101~111:20bit
//     int mode_f4 = 0b00;//0:spi disable, 1:spi enable
//     int ctrl_reg_f4 = osrs_t_f4 + osrs_p_f4 + mode_f4;
//     sr.println(ctrl_reg_f4);

//     int t_sb_f5 = 0b000 << 5;
//     int filter_f5 = 0b000 << 2;
//     int is_spi_f5 = 0b01;//00:sleep, 01or10:forced, 11:normal
//     int ctrl_reg_f5 = t_sb_f5 + filter_f5 + is_spi_f5;
//     sr.println(ctrl_reg_f5);

//     if(bl_test)
//     {
//         sr.println("+++++test start+++++");
//         // checkI2cAddress(wr, sr);
        
//         unsigned int tempT, tempP;
//         int bmp280_address = 0x76;
//         int ctrlAddress_f4 = 0xF4;
//         int ctrlAddress_f5 = 0xF5;
//         int tmpAddress = 0xFA;
//         int pressAddress = 0xF7;
//         byte valAll[7], valTemp[3], valPres[3];
//         // readI2C(val, wire_, HTDU21D_ADDRESS, TEMP_MEASURE_HOLD, 3);

//         wr.beginTransmission(bmp280_address);
//         wr.write(ctrlAddress_f5);
//         wr.write(ctrl_reg_f5);
//         wr.endTransmission();
//         delay(200);


//         //温度測定
//         wr.beginTransmission(bmp280_address);
//         wr.write(ctrlAddress_f4);
//         wr.write(ctrl_reg_f4);
//         wr.endTransmission();
//         // delay(200);
//         readI2C(valAll, wr, bmp280_address, tmpAddress, 6, 60);
//         tempT = (unsigned int)valAll[0] << 12 | (unsigned int)valAll[1] << 4 | (unsigned int)valAll[2] >> 4;
//         tempP = (unsigned int)valAll[3] << 12 | (unsigned int)valAll[4] << 4 | (unsigned int)valAll[5] >> 4;
//         delay(200);
//         sr.println(valAll[0]);
//         sr.println(valAll[1]);
//         sr.println(valAll[2]);
//         sr.println(tempT);
//         sr.println(valAll[3]);
//         sr.println(valAll[4]);
//         sr.println(valAll[5]);
//         sr.println(tempP);

//         delay(200);
//         readI2C(valAll, wr, bmp280_address, ctrlAddress_f4, 1, 60);
//         delay(200);
//         sr.println(valAll[0]);
//         readI2C(valAll, wr, bmp280_address, ctrlAddress_f5, 1, 60);
//         delay(200);
//         sr.println(valAll[0]);
        
//         // float tempSigTemp = temp / (float)65536; //2^16 = 65536
//         // float realTemperature = -46.85 + (175.72 * tempSigTemp); //From page 14
//         // return realTemperature;

//         sr.println("+++++test end+++++");
//     }
// }

void accessCount()
{
    accessNum++;
    // return EEP_Write2(eep, wifiSts.eep_address_measurement_num, String(accessNum));
}

void set_sensorInstallationStatus(TwoWire& wire_)
{
    for(int i = 0; i < enmDeviceMax; i++)
    {
        isDevice[i].isCommunication = i2cCtrl.checkDevice(wire_, isDevice[i].address);
        // delay(0);

    }
}

void setLight(){
    digitalWrite(PORT_LED, !digitalRead(PORT_LED));
    startTimerInterrupt(tLightOff, lightOffTime, false);
}

void IRAM_ATTR lightOff(){
    digitalWrite(PORT_LED, LED_OFF);
    // timerAlarmDisable(tLightOff);
}

void IRAM_ATTR ledFlash(){
    // stopTimerInterrupt(tLedFlash);
    // timerAlarmDisable(tLedFlash);
    digitalWrite(PORT_LED, !digitalRead(PORT_LED));

    // digitalWrite(PORT_LED, !digitalRead(PORT_LED));
    // startTimerInterrupt(tLedFlash, lightOffTime, false);
    // delay(15);
}
