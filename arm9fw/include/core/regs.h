#pragma once
#include <types.h>

// I2C stuff ////////////////////////
#define I2C1_REGS_BASE  (0x10161000)
#define I2C2_REGS_BASE  (0x10144000)
#define I2C3_REGS_BASE  (0x10148000)

struct I2cRegs
{
    vu8 REG_I2C_DATA;
    vu8 REG_I2C_CNT;
    vu16 REG_I2C_CNTEX;
    vu16 REG_I2C_SCL;
};

// CNT
#define I2C_STOP          (1u)      // bit 0 : STOP
#define I2C_START         (1u<<1)   // bit 1 : START
#define I2C_PAUSE         (1u<<2)   // bit 2 : PAUSE (also error)
#define I2C_ACK           (1u<<4)   // bit 4 : ACK flag

enum {
    I2C_READ_DIRECTION = 0,
    I2C_WRITE_DIRECTION
};

#define I2C_DATA_DIRECTION(direction)   ((direction ? 0u : 1u) << 5)    // bit 5 : DATA DIRECTION (0 : write ; 1 : read)

#define I2C_IRQ_ENABLE    (1u<<6)   // bit 6 : Enable Interrupt 
#define I2C_ENABLE        (1u<<7)   // bit 7 : Start/busy
#define I2C_GET_ACK(reg)  ((bool)((reg)>>4 & 1u))
///////////////////////////////////////