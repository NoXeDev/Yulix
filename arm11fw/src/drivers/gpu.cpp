#include <drivers/gpu.h>
#include <core/regs.h>

static bool bufferIsAlternate = false;

void GPU::gpuPowerOn()
{
    PDN_GPU_CNT = PDN_GPU_MAIN_ENABLE |
                    PDN_GPU_PSC_BLOCK_ENABLE |
                    PDN_GPU_GEOSHADER_BLOCK_ENABLE |
                    PDN_GPU_RASTERIZATION_BLOCK_ENABLE |
                    PDN_GPU_PPF_BLOCK_ENABLE |
                    PDN_GPU_PDC_BLOCK_ENABLE |
                    PDN_GPU_PDC_RELATED_ENABLE |
                    PDN_GPU_GLOBAL_CLOCK_ENABLE;
}

void GPU::lcdFillBlack(bool on)
{
    TOP_LCD_CFG = (on ? LCD_FILL_BLACK : 0);
    BOTTOM_LCD_CFG = (on ? LCD_FILL_BLACK : 0);
}

void GPU::lcdInit(u32 brightnessLevel)
{
    *(vu32 *)0x10202014 = 0x00000001; // ?
    *(vu32 *)0x1020200C &= 0xFFFEFFFE;  // ?
    *(vu32 *)0x10202240 = brightnessLevel;  // Backlight register (TOP)
    *(vu32 *)0x10202A40 = brightnessLevel;  // Backlight register (BOTTOM)
    *(vu32 *)0x10202244 = 0x1023E;  // ?
    *(vu32 *)0x10202A44 = 0x1023E;  // ?
}

void GPU::init(const FrameBuffers fbs[2])
{
    GPU::gpuPowerOn();
    GPU::lcdFillBlack(true);
    GPU::lcdInit(0x5F); //default max brightness 0x5F
    // isTop is the screen index
    for(u32 isTop = 0 ; isTop < 2 ; isTop++)
    {
        *GPU_REGS(0x00, isTop) = 0x000001c2;
        *GPU_REGS(0x04, isTop) = 0x000000d1;
        *GPU_REGS(0x08, isTop) = 0x000001c1;
        *GPU_REGS(0x0C, isTop) = 0x000001c1;
        *GPU_REGS(0x10, isTop) = (isTop ? 0x00000000 : 0x000000cd);
        *GPU_REGS(0x14, isTop) = 0x000000cf;
        *GPU_REGS(0x18, isTop) = 0x000000d1;
        *GPU_REGS(0x1C, isTop) = 0x01c501c1;
        *GPU_REGS(0x20, isTop) = 0x00010000;
        *GPU_REGS(0x24, isTop) = 0x0000019d;
        *GPU_REGS(0x28, isTop) = (isTop ? 0x00000002 : 0x00000052);
        *GPU_REGS(0x2C, isTop) = 0x00000192;
        *GPU_REGS(0x30, isTop) = 0x00000192;
        *GPU_REGS(0x34, isTop) = (isTop ? 0x00000192 : 0x0000004f);
        *GPU_REGS(0x38, isTop) = (isTop ? 0x00000001 : 0x00000050);
        *GPU_REGS(0x3C, isTop) = (isTop ? 0x00000002 : 0x00000052);
        *GPU_REGS(0x40, isTop) = (isTop ? 0x01960192 : 0x01980194);
        *GPU_REGS(0x44, isTop) = 0x00000000;
        *GPU_REGS(0x48, isTop) = (isTop ? 0x00000000 : 0x00000011);
        *GPU_REGS(0x5C, isTop) = (isTop ? 0x00f00190 : 0x00f00140);
        *GPU_REGS(0x60, isTop) = 0x01c100d1;
        *GPU_REGS(0x64, isTop) = (isTop ? 0x01920002 : 0x01920052);
        *GPU_REGS(0x68, isTop) = (isTop ? (u32)fbs[0].top_left : (u32)fbs[0].bottom);
        *GPU_REGS(0x6C, isTop) = (isTop ? (u32)fbs[1].top_left : (u32)fbs[1].bottom);
        *GPU_REGS(0x70, isTop) = (isTop ? 0x80341 : 0x80301);
        *GPU_REGS(0x74, isTop) = 0x00010501;
        *GPU_REGS(0x78, isTop) = 0;
        if(isTop)
        {
            *GPU_REGS(0x94, isTop) = (u32)fbs[0].top_right;
            *GPU_REGS(0x98, isTop) = (u32)fbs[1].top_right;
        }
        *GPU_REGS(0x90, isTop) = 0x000002D0;
        *GPU_REGS(0x9C, isTop) = 0x00000000;

        for(u32 i = 0 ; i < 256 ; i++)
        {
            *GPU_REGS(0x84, isTop) = 0x10101 * i;
        }
    }

    GPU::lcdFillBlack(false);
}

void GPU::clear(const FrameBuffers fbs[2])
{
    FrameBuffers fb = (bufferIsAlternate ? fbs[1] : fbs[0]);
    vu32 *GPU_MEMORY_FILL_PSC0 = (vu32 *)0x10400010,
         *GPU_MEMORY_FILL_PSC1 = (vu32 *)0x10400020;

    GPU_MEMORY_FILL_PSC0[0] = (u32)fb.top_left >> 3; //Start address
    GPU_MEMORY_FILL_PSC0[1] = (u32)(fb.top_left + SCREEN_TOP_FBSIZE) >> 3; //End address
    GPU_MEMORY_FILL_PSC0[2] = 0; //Fill value
    GPU_MEMORY_FILL_PSC0[3] = (2 << 8) | 1; //32-bit pattern; start

    GPU_MEMORY_FILL_PSC1[0] = (u32)fb.bottom >> 3; //Start address
    GPU_MEMORY_FILL_PSC1[1] = (u32)(fb.bottom + SCREEN_BOTTOM_FBSIZE) >> 3; //End address
    GPU_MEMORY_FILL_PSC1[2] = 0; //Fill value
    GPU_MEMORY_FILL_PSC1[3] = (2 << 8) | 1; //32-bit pattern; start

    while(!((GPU_MEMORY_FILL_PSC0[3] & 2) && (GPU_MEMORY_FILL_PSC1[3] & 2)));
}

void GPU::swap()
{
    *GPU_REGS(0x78, true) = (*GPU_REGS(0x78, true) & 0xFFFFFFFE) | !bufferIsAlternate;
    *GPU_REGS(0x78, false) = (*GPU_REGS(0x78, false) & 0xFFFFFFFE) | !bufferIsAlternate;
    bufferIsAlternate = !bufferIsAlternate;
}

void GPU::shutdownLcd()
{
    *(vu32 *)0x10202A44 = 0;
    *(vu32 *)0x10202244 = 0;
    *(vu32 *)0x10202014 = 0;
}

void GPU::fillColor(bool isTop, u32 color, const FrameBuffers fbs[2])
{
    FrameBuffers fb = (bufferIsAlternate ? fbs[1] : fbs[0]);
    vu32 *REGs_PSC = (isTop ? (vu32 *)0x10400010 : (vu32 *)0x10400020);
    u32 StartAddress;
    u32 EndAddress;
    if(isTop)
    {
        StartAddress = (u32)fb.top_left >> 3;
        EndAddress = (u32)(fb.top_left + SCREEN_TOP_FBSIZE) >> 3;
    } else {
        StartAddress = (u32)fb.bottom >> 3;
        EndAddress = (u32)(fb.bottom + SCREEN_BOTTOM_FBSIZE) >> 3;
    }


    REGs_PSC[0] = StartAddress;
    REGs_PSC[1] = EndAddress;
    REGs_PSC[2] = color; //Fill value
    REGs_PSC[3] = (1 << 8) | 1; //16-bit pattern; start

    while(!(REGs_PSC[3] & 2));
}

bool GPU::getActiveFramebuffer()
{
    return bufferIsAlternate;
}