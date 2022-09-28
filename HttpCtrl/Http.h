#include <Arduino.h>
#include <string>
#include <WiFi.h>
#include <WebServer.h>
#include <iostream>

// int aaa;
int test(int (*fn)(int), int sts);

// bool receiveRequest(WebServer server_, std::string recceive_message, void (*fanc)());

template<class Fn>
int call(Fn fn, int sts){
    return fn(sts);
}




