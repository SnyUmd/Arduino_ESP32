#include <WiFi.h>
#include <Espalexa.h>

#include "AutoWS_main.hpp"

Espalexa espalexa;

//**********************************************************************
void httpAction(int contentNum){
    int portLED;

    String returnMessage = "";
    String paramItem = server.arg("item");
    switch(contentNum)
    {
      case 0://read-------------------------------------------------------
        returnMessage = "Open";
        break;
      case 1:
        returnMessage = "Close";
        break;
      case 99:
        returnMessage ="test";
      default:
        returnMessage = "default";
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

    server.on("/test", HTTP_ANY, [](){httpAction(99);});

    // server.on("/reboot", HTTP_ANY, [](){httpAction(2);});

    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        clsCommon.ledOkOrNg(false, PORT_LED_INTERNAL);
        // bzErrorSound();
        server.send(404, "text/plain; charset=utf-8", "Not found"); // 404を返す
    });

    server.begin(port_num);
}



//**********************************************************************
void IRAM_ATTR onTimer0(){
    waterSts = !waterSts;
    clsWaterCtrl.SetWater(waterSts, PORT_WATER_OPEN, PORT_WATER_CLOSE);
    clsWaterCtrl.SetLED(waterSts, PORT_LED_GREEN, PORT_LED_ORANGE, LED_OFF_EX);
    sTime = GetTime();
    flgCheckTime = true;
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
    // server.begin(8000);
    setHttpAction(wifiSts.portNum.toInt());
    sr.print("Port number : ");
    sr.println(wifiSts.portNum);

#pragma endregion//-------------------------------------------------------


  clsCommon.LedFlashEX(2, 200, PORT_LED_GREEN);
  delay(200);
  clsCommon.LedFlashEX(2, 200, PORT_LED_ORANGE);
  delay(1000);

  setTimerInterrupt(timer, 0, onTimer0);//テスト用タイマー　定期ON/OFF
  startTimerInterrupt(timer, 4000000, true);

}

//**********************************************************************
//**********************************************************************
void loop() {
  server.handleClient();//サーバ動作

  clsCommon.setWaterLED();
  if(flgCheckTime && CheckElapsedTime(sTime, endTime)){
    clsWaterCtrl.ResetWater(PORT_WATER_OPEN, PORT_WATER_CLOSE, PORT_LED_GREEN, PORT_LED_ORANGE, LED_OFF_EX);
    flgCheckTime = false;
  }
}