#include <Arduino.h>
#include "defHct.h"


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

void BzTest();
void BzTest2();
void Bz_DragonQuest_Preface();

void BzGoUp(float ringTime, float stopTime);
void BzGoDown(float ringTime, float stopTime);

void bz(int num);
void receivedRing();
void errorSound();
void powerOn();
void prgBug();
void bzModeChange(int modeNum);
