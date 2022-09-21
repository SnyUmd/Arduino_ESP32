#include "TaskCtrl.h"
#include "Def.h"
#include "LED_Ctrl.h"

// TaskHandle_t thp[1];//マルチスレッドのタスクハンドル格納用

void TaskCtrl::setTask(TaskFunction_t task_, const char * const pc_name)
{
    xTaskCreatePinnedToCore(task_, pc_name, 4096, NULL, 3, NULL, 0); 
}

// void TaskCtrl::subProcess(void *pvParameters)
// {
//     LED_Ctrl::LedFlash(PORT_LED, 10, 1);
//     while(1)
//     {
//         delay(1);
//     }
// }