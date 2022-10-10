#include <Arduino.h>
#include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <iostream>

// int aaa;
int test(int (*fn)(int), int sts);

template<class Fn>
int call(Fn fn, int sts){
    return fn(sts);
}


// スタンダードの書き方　例
//   server.on("/target", HTTP_ANY, [](){
//     if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
//       target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
//     }
//     server.send(200, "text/plain", target); // 値をクライアントに返す
//   });