#include <Arduino.h>
#include "def.h"

class clsPowerCtrl
{
    public:
        static void DeepSleep_WakeupPort(gpio_num_t portNum, bool blHigh);

};