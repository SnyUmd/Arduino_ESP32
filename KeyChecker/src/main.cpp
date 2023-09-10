#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
// #include <vector>

#include "module/common.h"
#include "module/defKeyChecker.h"
#include "module/Initialize.h"
#include "module/Interrupt.h"
#include "module/wifiCtrl.h"

// put function declarations here:
//----------------------------
void swInterrupt_fn();
void setHttpAction();
void httpAction(int, String);
//----------------------------





//**********************************************************
//**********************************************************
//**********************************************************
void setup() {
  // put your setup code here, to run once:
  sr.begin(115200);

  initPort();
  // attachInterrupt(SW_INTERNAL, swInterrupt, FALLING);
  // initInterrupt(SW_INTERNAL, swInterrupt_fn, FALLING);

  if(!wifiInit(WiFi, sr, SSID, PASS, HOST_NAME, false))
  {
      while(1)
      {
          digitalWrite(LED_INTERNAL, !digitalRead(LED_INTERNAL));
          delay(200);
      }
  }
  else digitalWrite(LED_INTERNAL, LED_ON);
  setHttpAction();
  sr.print("setup successed");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  
}

//**********************************************************
/// @brief SW割り込み処理
void swInterrupt_fn()
{
  if(digitalRead(SW_INTERNAL) == !SW_OFF)
  {
    // チャタリング防止の為、割り込み停止
    attachInterrupt(SW_INTERNAL, NULL, FALLING);

    //割り込み処理--------------------------
    digitalWrite(LED_INTERNAL, !digitalRead(LED_INTERNAL));
    // ------------------------------------

    //割り込みセット
    attachInterrupt(SW_INTERNAL, swInterrupt_fn, FALLING);
  }
}

//**********************************************************
void setHttpAction()
{
    //バルブオープン
    server.on("/test", HTTP_ANY, [](){httpAction(0,"sts");});//http://keychecker.airport/test?sts=??
    //セット
    // server.on(httpContents[enmSet], HTTP_ANY, [](){httpAction(enmSet);});


    // 登録されてないパスにアクセスがあった場合
    server.onNotFound([](){
        server.send(404, "text/plain", "not found"); // 404を返す
    });

    server.begin();
}

//**********************************************************
void httpAction(int pattern, String param)
{
  String returnMessage = "";
  String param_val = server.arg(param);
  switch (pattern)
  {
    case 0:
      if(param_val == "ok")
        returnMessage = "OK";
      else if(param_val == "ng")
        returnMessage = "NG";
      else
        returnMessage = "-----";

      break;
    
    default:
      break;
  }
  server.send(200, "text/plain", returnMessage);

}


//**********************************************************
