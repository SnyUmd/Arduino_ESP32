#include <WiFi.h>
#include <WebServer.h>
#include "Http.h"

const char* ssid     = "Wi-ko1225_G"; // 自分のSSIDに書き換える
const char* password = "n17e92@53S19n"; // 自分のパスワードに書き換える
String hostname = "esp32";

WebServer server(80);
String target = "test"; // この変数をPOSTメソッドで書き換える

ClsHttp clsHttp;
// auto testFn(bool bl_port_sts);
// auto testFn = [](int port_sts){
auto testFn = [](int sts){
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
  return sts;
};

auto setLED = [](int sts)
{
  pinMode(2, OUTPUT);
  digitalWrite(2, sts);
};


// template<class Fn, class portSts> 
// template<class Fn> 
// void call(Fn fn, portSts ps)
// void call(Fn fn)
// {
//     fn();
// }

void setup() {
  // clsHttp.returnHttp0(testFn, 1);
  // int result = clsHttp.call(testFn, 1);
  // digitalWrite(2, clsHttp.call(testFn, 0));
  // digitalWrite(2, clsHttp.test(testFn, 1));
  // while(1);


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

  clsHttp.httpSet(server, "/led/on", setLED, 0);

  // 登録されてないパスにアクセスがあった場合
  server.onNotFound([](){
    server.send(404, "text/plain", "Not Found."); // 404を返す
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void init_port()
{
  
}


