#include <Arduino.h>

class TaskCtrl
{
private:
    // TaskHandle_t thp[1];//マルチスレッドのタスクハンドル格納用
public:
    void setTask(TaskFunction_t task_, const char * const pc_name);
    // void Task(void *args);
    // void subProcess(void *pvParameters);
};