class clsPowerCtrl
{
    public:
        static void DeepSleep_WakeupPort(gpio_num_t portNum, bool blHigh);
        static void DeepSleep_WakeupTime(int iWakeupTime);
};