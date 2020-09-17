#include <Arduino.h>
#include "def.h"


//void BzNote(int type, int iTime, int scale);
void BzNote(float type, float iTime, float scale, float iPase);
float getScale0(float iNote);
float getScale1(float iNote);
void InitBz();
void BuzzerRing_Dot(int ringNum, int iPeriod);

//Music*********************
void BzRing(float frq, float ringTime);
void BzExtend(float extendTime);
void BzStop(float stopTime);
void BzRest(float restTime);
//Music*********************

void Bz_DragonQuest_Preface0(int iBarNum);
void Bz_DragonQuest_Preface();

void BzGoUp(float ringTime, float stopTime);
void BzGoDown(float ringTime, float stopTime);
