#include <WiFi.h>
#include <string>
#include <WebServer.h>
#include <map>
#include <iostream>
#include "Http.h"
#include "LED_Ctrl.h"

const char* ssid     = "Wi-ko1225_G"; // 自分のSSIDに書き換える
const char* password = "n17e92@53S19n"; // 自分のパスワードに書き換える

// const char* ssid     = "Gal51-sn"; // 自分のSSIDに書き換える
// const char* password = "noenoe0714"; // 自分のパスワードに書き換える

String hostname = "esp32";
LED_Ctrl LC;

WebServer server(80);
String target = "test"; // この変数をPOSTメソッドで書き換える

enum EnmUriNum
{
  led_off = 0,
  led_on
};
enum EnmUriNum enmUriNum;

typedef struct{
  Uri uri;
  int function;
  String returnMess;
}dictionary;

const dictionary dic[]
{
  {"/led/off/1", 0, "LED OFF 1"},
  {"/led/on/1", 1, "LED ON 1"}
};


auto actions = [&dic](WebServer& sv, int function)
{
  String resultMess = "";
  if (sv.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      resultMess = sv.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
  }
  switch(function)
  {
    case 0:
      digitalWrite(2, 0);
      resultMess = dic[0].returnMess;
      break;
    case 1:
      digitalWrite(2,1);
      resultMess = dic[1].returnMess;
  }
  sv.send(200, "text/plain", resultMess);
};

// ****************************************************************
// auto testFn(bool bl_port_sts0);
// auto testFn = [](int port_sts){
auto testFn = [](int sts){
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
  return sts;
};

// ****************************************************************
auto setLED = [](int sts)
{
  pinMode(2, OUTPUT);
  digitalWrite(2, sts);
};

// ****************************************************************
void sv_on(WebServer & sv, Uri uri, int led_sts, String return_mess)
{
  // int sts = led_sts;
  String mess = return_mess;

  auto fn = [&](){
    if (sv.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      mess = sv.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    }
    setLED(led_sts);
    sv.send(200, "text/plain", mess); // 値をクライアントに返す
    Serial.println(mess);
    Serial.println(led_sts);
  };

  sv.on(uri, HTTP_ANY, fn);
}

// ****************************************************************
void SetLED0(WebServer & sv, int sts)
{
  pinMode(2, OUTPUT);
  digitalWrite(2, sts);
  String mss = (sts == 0) ? "LED OFF": "LED ON";
  sv.send(200, "text/plain", mss); // 値をクライアントに返す
}

// ****************************************************************
void setup() {
  pinMode(2, OUTPUT);
  LC.LedFlash(2, 5, 5);

  // シリアルコンソールのセットアップ
  Serial.begin(115200);
  delay(10);
  Serial.println();
  
  // WiFiに接続
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.setHostname(hostname.c_str());//ここでホスト名を設定します
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  LC.LedFlash(2, 2, 10);

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // アクセスされた際に行う関数を登録する
  server.on("/target", HTTP_ANY, [](){
    if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    }
    server.send(200, "text/plain", target); // 値をクライアントに返す
  });

  server.on("/aaa", HTTP_ANY, [](){
    if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    }
    server.send(200, "text/plain", "aaa"); // 値をクライアントに返す
  });

  server.on("/led/on/0", HTTP_ANY, [](){
    if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    }
    setLED(1);
    server.send(200, "text/plain", "LED on0"); // 値をクライアントに返す
  });

  server.on("/led/off/0", HTTP_ANY, [](){
    if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    }
    setLED(0);
    server.send(200, "text/plain", "LED off0"); // 値をクライアントに返す
  });

  server.on(dic[(int)enmUriNum.led_off], HTTP_ANY, [](){
    actions(server, dic[enmUriNum.led_off].function);
  });
  server.on("/led/on/1", HTTP_ANY, [](){
    actions(server, dic[1].function);
  });

  // 登録されてないパスにアクセスがあった場合
  server.onNotFound([](){
    server.send(404, "text/plain", "Not Found."); // 404を返す
  });

  server.begin();
}

// ****************************************************************
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

// ****************************************************************
void init_port()
{
  pinMode(2, OUTPUT);
}

// ****************************************************************
template<class Fn0>
void httpSet(WebServer& w_sv, const Uri& uri, Fn0 fn, int sts)
// void httpSet(const Uri uri, Fn0 fn, int sts)
{
    // auto ab = fn(sts);

    // WebServer sv(80); //WebServer情報に関する変数（インスタンス）
    //省略--アクセスポイントを設定--
    w_sv.on(uri, HTTP_ANY, fn(sts)); //URLを指定して処理する関数を指定
    // w_sv.onNotFound(handleNotFound); //URLが存在しない場合の処理する関数を指定
    // w_sv.begin(); //Webサーバーの開始
}


void setFunctions()
{

}

