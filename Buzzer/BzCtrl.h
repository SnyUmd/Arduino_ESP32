#include <Arduino.h>
#include "def.h"

void InitBz();
void BuzzerRing_Dot(int , int );

//Music*********************
void BzRing(float frq, float ringTime);
void BzExtend(float extendTime);
void BzRest(float restTime);
void BzStop(float stopTime);
//Music*********************

void Bz_DragonQuest_Preface();
void Bz_DragonQuest_Preface0(int iBarNum);

void BzGoUp(float ringTime, float stopTime);
void BzGoDown(float ringTime, float stopTime);
