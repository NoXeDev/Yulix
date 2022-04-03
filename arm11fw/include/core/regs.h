#pragma once
#include <types.h>

typedef void (*irq_handler)(void);

// DIC and GIC stuff //////

#define REG_GIC_BASE    (0x17E00100) // Located into  MPcore Private Memory Region
#define REG_DIC_BASE    (0x17E01000) // Same ^

#define REG_GIC_CONTROL    (*(vu32*)(REG_GIC_BASE + 0x00))
#define REG_GIC_PRIOMASK   (*(vu32*)(REG_GIC_BASE + 0x04))
#define REG_GIC_POI        (*(vu32*)(REG_GIC_BASE + 0x08))
#define REG_GIC_IRQACK     (*(vu32*)(REG_GIC_BASE + 0x0C))
#define REG_GIC_IRQEND     (*(vu32*)(REG_GIC_BASE + 0x10))
#define REG_GIC_LASTPRIO   (*(vu32*)(REG_GIC_BASE + 0x14))
#define REG_GIC_PENDING    (*(vu32*)(REG_GIC_BASE + 0x18))

#define REG_DIC_CONTROL    (*(vu32*)(REG_DIC_BASE + 0x00))
#define REG_DIC_SETENABLE  ((vu32*)(REG_DIC_BASE + 0x100))
#define REG_DIC_CLRENABLE  ((vu32*)(REG_DIC_BASE + 0x180))
#define REG_DIC_SETPENDING ((vu32*)(REG_DIC_BASE + 0x200))
#define REG_DIC_CLRPENDING ((vu32*)(REG_DIC_BASE + 0x280))
#define REG_DIC_PRIORITY   ((vu32*)(REG_DIC_BASE + 0x400))
#define REG_DIC_TARGETPROC ((vu8*) (REG_DIC_BASE + 0x800))
#define REG_DIC_CFGREG     ((vu32*)(REG_DIC_BASE + 0xC00))

//////////////////////////////////////////////////////////

#define PDN_GPU_CNT (*(vu32*)(0x10141200))
#define PDN_GPU_MAIN_ENABLE (1 << 0)
#define PDN_GPU_PSC_BLOCK_ENABLE (1 << 1)
#define PDN_GPU_GEOSHADER_BLOCK_ENABLE (1 << 2)
#define PDN_GPU_RASTERIZATION_BLOCK_ENABLE (1 << 3)
#define PDN_GPU_PPF_BLOCK_ENABLE (1 << 4)
#define PDN_GPU_PDC_BLOCK_ENABLE (1 << 5)
#define PDN_GPU_PDC_RELATED_ENABLE (1 << 6)
#define PDN_GPU_GLOBAL_CLOCK_ENABLE (1 << 16)

//                  GPU STUFF                   /////////
#define TOP_LCD_CFG (*(vu32*)(0x10202204))
#define BOTTOM_LCD_CFG (*(vu32*)(0x10202A04))
#define LCD_FILL_BLACK (0x01000000)

#define GPU_LCD_FRAMEBUFFER_SETUP_BASE_TOP  (0x10400400)
#define GPU_LCD_FRAMEBUFFER_SETUP_BASE_BOTTOM  (0x10400500)

#define GPU_REGS(offset, topbase) ((vu32*)((topbase ? GPU_LCD_FRAMEBUFFER_SETUP_BASE_TOP : GPU_LCD_FRAMEBUFFER_SETUP_BASE_BOTTOM) + offset))