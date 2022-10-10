#include "Actions.h"
void ledOn()
{
    digitalWrite(PORT_LED, LED_ON);
}

void setMotorPort(int (&ary)[4], int port0, int port1, int port2, int port3)
{
    ary[0] = port0;
    ary[1] = port1;
    ary[2] = port2;
    ary[3] = port3;
}


void motorAction(int *port_sts)
{
    digitalWrite(PORT_MOTER1, port_sts[0]);
    digitalWrite(PORT_MOTER2, port_sts[1]);
    digitalWrite(PORT_MOTER3, port_sts[2]);
    digitalWrite(PORT_MOTER4, port_sts[3]);
}