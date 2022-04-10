#include <drivers/i2c.h>

I2C::I2C(I2cDevID deviceID)
{
    I2cDevice currentDevice = i2cDevTable[deviceID];
    switch(currentDevice.busID)
    {
        case 0:
            this->registers = (I2cRegs*)I2C1_REGS_BASE;
            break;
        case 1:
            this->registers = (I2cRegs*)I2C2_REGS_BASE;
            break;
        case 2:
            this->registers = (I2cRegs*)I2C3_REGS_BASE;
            break;
        default:
            break;
    };
    this->device = currentDevice;

    // Init I2C bus (according to 3dbrew)
    i2cWait();
    this->registers->REG_I2C_CNTEX = 2;
    this->registers->REG_I2C_SCL = 1280;

    // Now ready
}

void I2C::i2cWait()
{
    while(this->registers->REG_I2C_CNT & I2C_ENABLE);
}

bool I2C::i2cWriteRegister(u8 registerAddress, u8 data)
{
    const u8* in = &data;

    u32 maxErrRetry = 8;
    while(maxErrRetry > 0)
    {
        i2cWait();

        // Select the device
        this->registers->REG_I2C_DATA = this->device.devAddress;
        this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_START;
        i2cWait();
        if(!I2C_GET_ACK(this->registers->REG_I2C_CNT))
        {
            this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_PAUSE | I2C_STOP;
            maxErrRetry--;
            continue;
        }

        // Select the register and the data direction
        this->registers->REG_I2C_DATA = registerAddress;
        this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_DATA_DIRECTION(I2C_WRITE_DIRECTION);
        i2cWait();
        if(!I2C_GET_ACK(this->registers->REG_I2C_CNT))
        {
            this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_PAUSE | I2C_STOP;
            maxErrRetry--;
            continue;
        }

        break;
    }

    if(maxErrRetry == 0)
        return false;

    // Device was selected and register too, now, write the data and enjoy
    this->registers->REG_I2C_DATA = *in;
    this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_DATA_DIRECTION(I2C_WRITE_DIRECTION) | I2C_STOP;
    i2cWait();
    if(!I2C_GET_ACK(this->registers->REG_I2C_CNT))
    {
        this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_PAUSE | I2C_STOP;
        return false;
    }

    return true;
}

u8 I2C::i2cReadRegister(u8 registerAddress)
{
    u32 maxErrRetry = 8;
    while(maxErrRetry > 0)
    {
        i2cWait();

        // Select the device
        this->registers->REG_I2C_DATA = this->device.devAddress;
        this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_START;
        i2cWait();
        if(!I2C_GET_ACK(this->registers->REG_I2C_CNT))
        {
            this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_PAUSE | I2C_STOP;
            maxErrRetry--;
            continue;
        }

        // Select the register and the data direction
        this->registers->REG_I2C_DATA = registerAddress;
        this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_DATA_DIRECTION(I2C_WRITE_DIRECTION);
        i2cWait();
        if(!I2C_GET_ACK(this->registers->REG_I2C_CNT))
        {
            this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_PAUSE | I2C_STOP;
            maxErrRetry--;
            continue;
        }

        this->registers->REG_I2C_DATA = this->device.devAddress | 1u; // bit 0 : read
        this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_START;
        i2cWait();
        if(!I2C_GET_ACK(this->registers->REG_I2C_CNT))
        {
            this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_PAUSE | I2C_STOP;
            maxErrRetry--;
            continue;
        }

        break;
    }

    if(maxErrRetry == 0)
        return 0xFF;

    u8 out;
    this->registers->REG_I2C_CNT = I2C_ENABLE | I2C_DATA_DIRECTION(I2C_READ_DIRECTION) | I2C_ACK;
    i2cWait();
    out = this->registers->REG_I2C_DATA;
    return out;
}