#pragma once
#include <core/regs.h>
#include <types.h>

enum I2cDevID
{
    I2C_DEV_MCU = 3
};

static const struct I2cDevice
{
    u8 busID, devAddress;
} i2cDevTable[] =
{
    {0, 0x4A},
    {0, 0x7A},
    {0, 0x78},
    {1, 0x4A},
    {1, 0x78},
    {1, 0x2C},
    {1, 0x2E},
    {1, 0x40},
    {1, 0x44},
    {2, 0xA6}, // Changed from 0xA6 to 0xD6 (if we need this, we need first check the version of the MCU firmware)
    {2, 0xD0},
    {2, 0xD2},
    {2, 0xA4},
    {2, 0x9A},
    {2, 0xA0},
    {1, 0xEE},
    {0, 0x40},
    {2, 0x54}
};

class I2C {
    public:
        I2C(I2cDevID deviceID);
        bool i2cWriteRegister(u8 registerAddress, u8 data);
        u8 i2cReadRegister(u8 registerAddress);

    private:
        I2cDevice device;
        I2cRegs *registers;
        void i2cWait();
};