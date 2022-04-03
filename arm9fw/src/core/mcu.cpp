#include <core/mcu.h>

I2C comMcu = I2C(I2C_DEV_MCU);

void MCU::powerOff()
{
    comMcu.i2cWriteRegister(0x20, 1 << 0); // bit 0 = power off
}

void MCU::backlightScreen(bool isOn)
{
    comMcu.i2cWriteRegister(0x22, (isOn ? 0x2A : 0x15)); // bit 1 & 3 & 5 (push to LCDs & bottom screen backlight on & top screen backlight on)
}

void MCU::wifiLED(bool isOn)
{
    comMcu.i2cWriteRegister(0x2A, isOn);
}