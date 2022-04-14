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


// BUTTONS STUFF //////////////////////
#define HID_PAD (*(vu32*)(0x10146000))
#define BUTTON_A    (1 << 0)
#define BUTTON_B    (1 << 1)

// CRYPTO STUFF //////////////////////
#define SHA_CNT         ((vu32 *)0x1000A000)
#define SHA_BLKCNT      ((vu32 *)0x1000A004)
#define SHA_HASH        ((vu32 *)0x1000A040)
#define SHA_FIFO        ((vu32 *)0x1000A080)


#define SHA_CNT_START   (1 << 0)
#define SHA_CNT_FINAL_ROUND(bitState) (bitState << 1) // 1 = enable/busy ; 0 = normal
#define SHA_CNT_INPUTDMA(enable) (enable << 2)
#define SHA_CNT_OUTPUT_ENDIANESS(mode) (mode << 3) // 0 = little ; 1 = big
#define SHA_256_MODE    (0 << 4)
#define SHA_224_MODE    (1 << 4)

// AES

#define AES_CNT         ((vu32 *)0x10009000)