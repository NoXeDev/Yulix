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
#define AES_BLKCNT      ((vu32 *)0x10009004)
#define AES_WRFIFO      ((vu32 *)0x10009008)
#define AES_RDFIFO      ((vu32 *)0x1000900C)
#define AES_KEYSEL      ((vu8 *)0x10009010)
#define AES_KEYCNT      ((vu8 *)0x10009011)
#define AES_CTR         ((vu32 *)0x10009020)
#define AES_MAC         ((vu32 *)0x10009030)

#define AES_KEY0        ((vu32 *)0x10009040)
#define AES_KEY1        ((vu32 *)0x10009070)
#define AES_KEY2        ((vu32 *)0x100090A0)
#define AES_KEY3        ((vu32 *)0x100090D0)

#define AES_KEYFIFO     ((vu32 *)0x10009100)
#define AES_KEYXFIFO    ((vu32 *)0x10009104)
#define AES_KEYYFIFO    ((vu32 *)0x10009108)

#define AES_CCM_DECRYPT_MODE    (0u << 27)
#define AES_CCM_ENCRYPT_MODE    (1u << 27)
#define AES_CTR_MODE            (2u << 27)
#define AES_CBC_DECRYPT_MODE    (4u << 27)
#define AES_CBC_ENCRYPT_MODE    (5u << 27)
#define AES_ECB_DECRYPT_MODE    (6u << 27)
#define AES_ECB_ENCRYPT_MODE    (7u << 27)

#define AES_CNT_START           (1 << 31)
#define AES_CNT_INPUT_ORDER(op) (op << 25) // 1 = Normal / 0 = Reverse
#define AES_CNT_OUTPUT_ORDER(op) (op << 24) // 1 = Normal / 0 = Reverse
#define AES_CNT_INPUT_ENDIAN(op) (op << 23) // 1 = Big / 0 = Little
#define AES_CNT_OUTPUT_ENDIAN(op) (op << 22) // 1 = Big / 0 = Little
#define AES_CNT_FLUSH_READ      (1 << 11)
#define AES_CNT_FLUSH_WRITE     (1 << 10)

#define AES_KEYCNT_HW_TYPE(type)    (type << 6)
#define AES_KEYCNT_ENABLE_FIFO_FLUSH      (1 << 7)