#define BUZZER_PWM 25
#define SWITCH0 0

#define PWM_BIT_NUM 8
#define PWM_RESOLUTION 256
#define PWM_FRQ1000 1000
#define PWM_FRQ5000 5000
#define PWM_CHANNEL0 0
#define PWM_PIN 25
#define PWM_DUTY 128

#include <Arduino.h>
using namespace std;

// extern float hctMelody[100][4];
// #include "defHct.h"

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
void bzReceivedRing();
void bzErrorSound();
void bzPowerOn();
void prgBug();
void bzModeChange(int modeNum);

void setHctMelody(HardwareSerial& sr, string val);
// void bzHctMelody();
