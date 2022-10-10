#include <WiFi.h>
#include <string>
#include <WebServer.h>
#include <iostream>
#include "Http.h"
#include "LED_Ctrl.h"
#include "def_items.h"
#include "Actions.h"

const char* ssid     = "Wi-ko1225_G"; // 自分のSSIDに書き換える
const char* password = "n17e92@53S19n"; // 自分のパスワードに書き換える
// const char* ssid     = "Gal51-sn"; // 自分のSSIDに書き換える
// const char* password = "noenoe0714"; // 自分のパスワードに書き換える

String hostname = "esp32";
LED_Ctrl LC;

WebServer server(80);
String target = "test"; // この変数をPOSTメソッドで書き換える

int motorSts[5][4] = 
{
  {0, 0, 0, 0},
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1},
};

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
  {"/led/on/1", 1, "LED ON 1"},
  {"/motor", 2, "Motor Set"},
  {"/test", 100, "test"}
};


// ****************************************************************
auto actions = [&dic](WebServer& sv, int function)
{
  String resultMess = "";
  if (sv.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
      resultMess = sv.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
  }
  switch(function)
  {
    case 0:
      digitalWrite(PORT_LED, 0);
      resultMess = dic[0].returnMess;
      break;

    case 1:
      digitalWrite(PORT_LED,1);
      resultMess = dic[1].returnMess;
      break;

    case 2:
      int aryMotorPort[] = {0, 0, 0, 0};
      for(int i = 0; i < 1000; i++)
      {
        motorAction(motorSts[1]);
        delay(5);
        motorAction(motorSts[2]);
        delay(5);
        motorAction(motorSts[3]);
        delay(5);
        motorAction(motorSts[4]);
        delay(5);
      }
      motorAction(motorSts[0]);
      resultMess = dic[2].returnMess;
      break;

    // case 100:
    //   digitalWrite(PORT_LED, 1);
    //   resultMess = "test";
    //   break;
  }
  sv.send(200, "text/plain", resultMess);
};

// ****************************************************************
auto setLED = [](int sts)
{
  pinMode(2, OUTPUT);
  digitalWrite(2, sts);
};

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
  init_port();

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

  // auto testFn = [](){delay(1);};


  server.on(dic[0].uri, HTTP_ANY, [](){
    actions(server, dic[0].function);
  });

  server.on(dic[1].uri, HTTP_ANY, [](){
    actions(server, dic[1].function);
  });

  server.on(dic[2].uri, HTTP_ANY, [](){
    actions(server, dic[2].function);
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
  pinMode(PORT_LED, OUTPUT);
  digitalWrite(PORT_LED, LED_OFF);

  pinMode(PORT_MOTER1, OUTPUT);
  pinMode(PORT_MOTER2, OUTPUT);
  pinMode(PORT_MOTER3, OUTPUT);
  pinMode(PORT_MOTER4, OUTPUT);
  digitalWrite(PORT_MOTER1, LOW);
  digitalWrite(PORT_MOTER2, LOW);
  digitalWrite(PORT_MOTER3, LOW);
  digitalWrite(PORT_MOTER4, LOW);
}