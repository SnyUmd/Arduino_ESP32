class ClsWaterCtrl{
    private:
    public:
        void SetWater(bool is_open, int port_open, int port_close);
        void SetLED(bool is_open, int port_led_open, int port_led_close, int logic_led_off);
};