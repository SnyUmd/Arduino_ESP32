#include <WiFi.h>
#include <string>
#include <WebServer.h>
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


// ****************************************************************
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

// ****************************************************************
auto setLED = [](int sts)
{
  pinMode(2, OUTPUT);
  digitalWrite(2, sts);
};

// ****************************************************************
template<class Fn>
void sv_on(WebServer & sv, const Uri & uri, Fn & fn, int fn_sts, String return_mess)
{
  sv.on(uri, HTTP_ANY, [&sv, &fn, &fn_sts, &return_mess](){
    if (sv.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      return_mess = sv.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    }
    fn(fn_sts);
    sv.send(200, "text/plain", return_mess); // 値をクライアントに返す
  });
}

// ****************************************************************
template<class Fn>
void sv_on00(WebServer & sv, const Uri & uri, Fn & fn, int fn_sts, const String & return_mess)
{
  String mss = "";
  sv.on(uri, HTTP_ANY, [&sv, &fn, &fn_sts, &return_mess](){
    fn(fn_sts);
    sv.send(200, "text/plain", return_mess); // 値をクライアントに返す
  })();
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
  LC.LedFlash(1, 10, 10);

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

  server.on("/led/on", HTTP_ANY, [](){
    if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    }
    setLED(1);
    server.send(200, "text/plain", "LED on"); // 値をクライアントに返す
  });

  server.on("/led/off", HTTP_ANY, [](){
    if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    }
    setLED(0);
    server.send(200, "text/plain", "LED off"); // 値をクライアントに返す
  });

  // httpSet(server, "/led/on", setLED, 1);
  // httpSet(server, "/led/off", setLED, 0);

  // server.on("/led/on", HTTP_ANY, [](){setLED(1);});
  // server.on("/led/off", HTTP_ANY, [](){setLED(0);});

  // server.on("/led/on", HTTP_ANY, [](){SetLED0(server, 1);});
  // server.on("/led/off", HTTP_ANY, [](){SetLED(server, 0);});

  // SetLED1(server, "/led/on");
  // sv_on(server, "/led/on", setLED, 1, "LED on");
  // sv_on(server, "/led/off", setLED, 0, "LED off");


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




