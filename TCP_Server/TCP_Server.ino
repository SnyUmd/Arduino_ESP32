#include "Def.h"
#include "LED_Ctrl.h"

LED_Ctrl LC;
// TaskHandle_t thp[1];//マルチスレッドのタスクハンドル格納用

void setup()
{
    init();
    LC.LedFlash(PORT_LED0, 10, 5);
    LC.LedFlash(PORT_LED1, 10, 5);
}

void loop()
{
    LC.LedFlash(PORT_LED0, 50, 1);
    // LC.LedFlash(PORT_LED, 100, 1);
}

// void subProcess(void *args)
void subProcess(void *pvParameters)
{
    while(1)
    {
        LC.LedFlash(PORT_LED1, 100, 1);
        // delay(1);
    }
}

void init()
{

    xTaskCreatePinnedToCore(subProcess, "subProcess", 4096, NULL, 3, NULL, 0); 
    // タスク名,
    // "タスク名",
    // スタックメモリサイズ,
    // NULL,
    // タスク優先順位,
    // タスクハンドルポインタ,
    // Core ID
    //内容は([タスク名], "[タスク名]", [スタックメモリサイズ(4096or8192)],
    //      NULL, [タスク優先順位](1-24,大きいほど優先順位が高い)],
    //      [宣言したタスクハンドルのポインタ(&thp[0])], [Core ID(0 or 1)]); 

    // LED
    pinMode(PORT_LED0, OUTPUT);
    pinMode(PORT_LED1, OUTPUT);
    digitalWrite(PORT_LED0, LED_OFF);
    digitalWrite(PORT_LED1, LED_OFF);

    // モーター
    // pinMode(PORT_MOTER_IN1, OUTPUT);
    // pinMode(PORT_MOTER_IN2, OUTPUT);
    // pinMode(PORT_MOTER_IN3, OUTPUT);
    // pinMode(PORT_MOTER_IN4, OUTPUT);
    // digitalWrite(PORT_MOTER_IN1, LOW);
    // digitalWrite(PORT_MOTER_IN2, LOW);
    // digitalWrite(PORT_MOTER_IN3, LOW);
    // digitalWrite(PORT_MOTER_IN4, LOW);
}