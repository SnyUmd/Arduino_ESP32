
#include "def_items.h"
#include <WebServer.h>
#include <WiFi.h>
#include <Arduino.h>

void ledOn();

void setMotorPort(int (&ary)[4], int port0, int port1, int port2, int port3);
void motorAction(int *port_sts);