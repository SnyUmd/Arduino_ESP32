#include <Arduino.h>

#include "class/clsWaterCtrl.hpp"
#include "modules/def.hpp"

class ClsCommon{
    private:
        ClsWaterCtrl clsWaterCtrl;
    public:
        void initPort();
        void ledOkOrNg(bool is_ok, int port_led);
        void LedFlashEX(int flash_num, int interval, int led_port_ex);
        void setWaterLED();
};