// #include "ClsHttp.h"
// #include <Arduino.h>


// int ClsHttp::test(int (*fn)(int), int sts)
// {
//     // server.on("/target", HTTP_ANY, [](){
//     // if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
//     //   target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
//     // }
//     // server.send(200, "text/plain", target); // 値をクライアントに返す
//     //   });
//     return fn(sts);
// }

// /*bool ClsHttp::receiveRequest(WebServer server_, std::string recceive_message, void (*fanc)())
// {
//     //recceive_message:"/message"
//     std::string headerMess = recceive_message.substr(0, 1);
//     if (headerMess != "/")
//         return false;

//     server.on(recceive_message, HTTP_ANY, [](){
//     if (server.method() == HTTP_POST) { // POSTメソッドでアクセスされた場合
//       target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
//     }
//     server.send(200, "text/plain", target); // 値をクライアントに返す
//       });
//       return true;
// }*/

