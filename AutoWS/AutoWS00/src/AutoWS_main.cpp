#include <WiFi.h>
#include <Espalexa.h>

#include "AutoWS_main.hpp"

Espalexa espalexa;

//**********************************************************************
String checkDeviceSts(){
  String result = "";
  result += "{\n";
  result += "\"open\":";
  result += ary_blDeviceSts[enmDevStsOpen];
  result += ",\n";
  result += "\"close\":";
  result += ary_blDeviceSts[enmDevStsClose];
  result += ",\n";
  result += "\"watering\":";
  result += ary_blDeviceSts[enmDevStsWatering];
  result += ",\n";
  result += "\"signalopen\":";
  result += ary_blDeviceSts[enmDevStsSignalOpen];
  result += ",\n";
  result += "\"signalclose\":";
  result += ary_blDeviceSts[enmDevStsSignalClose];
  result += ",\n";
  result += "}";
  return result;
}

//**********************************************************************
void httpAction(int contentNum){
    int portLED;
    long elapsed_time;
    int paramTime = server.arg("time").toInt();

    String returnMessage = "";
    switch(contentNum)
    {
      case 0://open
        stopTimerInterrupt(timer_close);//クローズタイマーを止める
        clsWaterCtrl.WaterOpen();
        ary_blDeviceSts[enmDevStsOpen] = true;
        ary_blDeviceSts[enmDevStsClose] = false;
        startTimerInterrupt(timer_close, MAX_WATERINT_TIME * 1000000, false);
        returnMessage = "set open";
        returnMessage += "\n";
        returnMessage += "\n";
        returnMessage += checkDeviceSts();
        break;
      case 1://close
        clsWaterCtrl.WaterClose();
        ary_blDeviceSts[enmDevStsOpen] = false;
        ary_blDeviceSts[enmDevStsClose] = true;
        stopTimerInterrupt(timer_close);
        returnMessage = "set close";
        returnMessage += "\n";
        returnMessage += "\n";
        returnMessage += checkDeviceSts();
        break;
      case 2://run
        if(paramTime > MAX_WATERINT_TIME){
          returnMessage = "Error ";
          returnMessage += paramTime;
          returnMessage += " : Time is less than ";
          returnMessage += MAX_WATERINT_TIME;
          returnMessage += " seconds";
          break;
        }
        stopTimerInterrupt(timer_close);//クローズタイマーを止める
        ary_blDeviceSts[enmDevStsOpen] = false;
        ary_blDeviceSts[enmDevStsClose] = false;
        clsWaterCtrl.WaterOpen();
        ary_blDeviceSts[enmDevStsOpen] = true;
        ary_blDeviceSts[enmDevStsClose] = false;
        startTimerInterrupt(timer_close, paramTime * 1000000, false);
        returnMessage = "set start watering ";
        returnMessage += paramTime;
        returnMessage += " sec";
        returnMessage += "\n";
        returnMessage += "\n";
        returnMessage += checkDeviceSts();
        break;
      case 3://read
        returnMessage = "sts read";
        returnMessage += "\n";
        returnMessage += "\n";
        returnMessage += checkDeviceSts();
        break;
      case 99:
        returnMessage ="test";
        returnMessage += "\n";
        returnMessage += "\n";
        returnMessage += checkDeviceSts();
        break;
      default:
        returnMessage = "default";
        returnMessage += "\n";
        returnMessage += "\n";
        returnMessage += checkDeviceSts();
        break;
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
    // bzReceivedRing();
    // digitalWrite(PORT_ACCESS, HIGH);
}

//**********************************************************************
void setHttpAction(int port_num)
{
    //読込
    server.on("/open", HTTP_ANY, [](){
        httpAction(0);
        // digitalWrite(PORT_ACCESS, HIGH);
    });
    //セット
    server.on("/close", HTTP_ANY, [](){/*bzReceivedRing();*/ httpAction(1);});

    server.on("/run", HTTP_ANY, [](){/*bzReceivedRing();*/ httpAction(2);});

    server.on("/read", HTTP_ANY, [](){/*bzReceivedRing();*/ httpAction(3);});

    server.on("/test", HTTP_ANY, [](){httpAction(99);});

    // server.on("/reboot", HTTP_ANY, [](){httpAction(2);});

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        clsCommon.ledOkOrNg(false, PORT_LED_INTERNAL);
        // bzErrorSound();
        server.send(404, "text/plain; charset=utf-8", "Not found"); // 404を返す
    });

    server.begin(port_num);
    // server.begin();
}



//**********************************************************************
void IRAM_ATTR onTimer(){
}

//**********************************************************************
void IRAM_ATTR onTimer_Close(){
  clsWaterCtrl.WaterClose();
  ary_blDeviceSts[enmDevStsOpen] = false;
  ary_blDeviceSts[enmDevStsClose] = true;
}


//**********************************************************************
//**********************************************************************
void setup() {
    sr.println("----- Set serial -----");
    sr.begin(115200);
    sr.println("----- Set port -----");
    clsCommon.initPort();

/*Alexa操作　無効
    // WifiSetup();

    // espalexa.addDevice("ESP_LED", firstLightChanged,0); //simplest definition, default state off
    // espalexa.begin();

    // LedFlash(5, 200, LED_INTERNAL);
*/

#pragma region WiFi setting//----------------------------------------------
    sr.println("----- WiFi setting -----");

    // if(!wifiInit(wf, sr, wifiSts.ssid, wifiSts.pass, wifiSts.host_name, false))
    if(!wifiInit(wf, sr, wifiSts.ssid, wifiSts.pass, wifiSts.host_name, false))
    {
        sr.println("wifi networ error");
        clsCommon.ledOkOrNg(false, PORT_LED_INTERNAL);
        sr.println("software reset");
        ESP.restart();
    }
    clsCommon.ledOkOrNg(true, PORT_LED_INTERNAL);
#pragma endregion//-------------------------------------------------------


#pragma region server setting//----------------------------------------------
    sr.println("----- Server setting -----");
    setHttpAction(wifiSts.portNum);
    sr.print("Port number : ");
    sr.println(wifiSts.portNum);

#pragma endregion//-------------------------------------------------------

  setTimerInterrupt(timer_close, enmStop, onTimer_Close);
  // setTimerInterrupt(timer_close, enmReserve1, onTimer);
  // setTimerInterrupt(timer_close, enmReserve2, onTimer);
  // setTimerInterrupt(timer_close, enmReserve3, onTimer);


  clsCommon.LedFlashEX(2, 200, PORT_LED_OPEN);
  delay(200);
  clsCommon.LedFlashEX(2, 200, PORT_LED_CLOSE);
  delay(200);

  clsWaterCtrl.WaterClose();
  ary_blDeviceSts[enmDevStsOpen] = false;
  ary_blDeviceSts[enmDevStsClose] = true;
}

//**********************************************************************
//**********************************************************************
void loop() {
  server.handleClient();//サーバ動作
}