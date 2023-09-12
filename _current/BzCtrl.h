#define BUZZER_PWM 25
#define SWITCH0 0

#define PWM_BIT_NUM 8
#define PWM_RESOLUTION 256
#define PWM_FRQ1000 1000
#define PWM_FRQ5000 5000
#define PWM_CHANNEL0 0
#define PWM_PIN 25
#define PWM_DUTY 128

#define PWM_FRQ_DO_0 1046.502
#define PWM_FRQ_RE_0 1174.659
#define PWM_FRQ_MI_0 1318.51
#define PWM_FRQ_FA_0 1396.913
#define PWM_FRQ_SO_0 1567.982
#define PWM_FRQ_RA_0 1760
#define PWM_FRQ_SI_0 1975.533
#define PWM_FRQ_DO_1 2093.005

#include <Arduino.h>
using namespace std;

void BzNote(float type, float iTime, float scale, float iPase);
float getScale0(float iNote);
float getScale1(float iNote);
void InitBz();

void BzRing(float frq, float ringTime);
void BzStop(float stopTime);

void Bz_DragonQuest_Preface();

void BzGoUp(float ringTime, float stopTime);
void BzGoDown(float ringTime, float stopTime);

void bzReceivedRing();
void bzErrorSound();
void bzPowerOn();

void setHctMelody(HardwareSerial& sr, string val);
