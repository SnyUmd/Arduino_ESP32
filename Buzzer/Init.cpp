#include <Arduino.h>
#include "init.h"
#include "def.h"

// float flSound[][12] =
//     {
//         {130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220, 233.082, 246.942},
//         {261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883},
//         {523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880, 932.328, 987.767},
//         {1046.502, 1108.731, 1174.659, 1244.508, 1318.51, 1396.913, 1479.978, 1567.982, 1661.219, 1760, 1864.655, 1975.533},
//         {2093.005, 2217.461, 2349.318, 2489.016, 2637.02, 2793.826, 2959.955, 3135.963, 3322.438, 3520, 3729.31, 3951.066},
//         {4186.009, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
//         /*　0ド , 1ド# , 2レ, 3レ#, 4ミ , 5ファ , 6ファ# , 7ソ , 8ソ# , 9ラ , 10ラ# , 11シ　*/
//     };


//test = 0;

void Initialize_Port()
{

    //LED
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GRE, OUTPUT);

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GRE, HIGH);

    //スポットライト
    //pinMode(SPOT_LIGHT, OUTPUT);
    //digitalWrite(SPOT_LIGHT, LOW);

    //人感センサ
    //pinMode(HUMAN_SENSOR, INPUT);
    //pinMode(15, INPUT);

    //ブザー
    //pinMode(BUZZER_PWM, OUTPUT);
    //digitalWrite(BUZZER_PWM, LOW);
    
    //テスト用
    pinMode(26, OUTPUT);
    digitalWrite(26, LOW);

    //タクトスイッチ
    pinMode(SWITCH0, INPUT_PULLUP);
}