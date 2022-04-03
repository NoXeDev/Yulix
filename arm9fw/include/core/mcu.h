#pragma once
#include <drivers/i2c.h>

class MCU 
{
    public:
        MCU();
        void powerOff();
        void wifiLED(bool isOn);
        void backlightScreen(bool isOn);

    private:
        I2C comMcu;
};