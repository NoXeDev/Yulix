#pragma once
#include <drivers/i2c.h>

class MCU 
{
    public:
        static void powerOff();
        static void wifiLED(bool isOn);
        static void backlightScreen(bool isOn);
};