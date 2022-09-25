#include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <iostream>
#include <Arduino.h>

class ClsHttp
{
    private:
        WebServer w_sv;
        
    public:
        ClsHttp(WebServer server)
        {
          w_sv = server;
        }

        // int aaa;
        int test(int (*fn)(int), int sts);
        
        // bool receiveRequest(WebServer server_, std::string recceive_message, void (*fanc)());
        
        template<class Fn>
        int call(Fn fn, int sts){
            return fn(sts);
            // fn(sts);
            // return sts;
        }

        template<class Fn0>
        // void httpSet(WebServer w_sv, std::string rq, Fn0 fn, int sts)
        void httpSet(std::string rq, Fn0 fn, int sts)
        {
            auto ab = fn(sts);
            // WebServer server(80); //WebServer情報に関する変数（インスタンス）
            //省略--アクセスポイントを設定--
            w_sv.on(rq, HTTP_ANY, ab); //URLを指定して処理する関数を指定
            // w_sv.onNotFound(handleNotFound); //URLが存在しない場合の処理する関数を指定
            // w_sv.begin(); //Webサーバーの開始
        }

};
