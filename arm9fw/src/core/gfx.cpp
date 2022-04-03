#include <core/gfx.h>
#include <pxi.h>

/*void Screen::init()
{
    PXI_DoCMD(PXI_INIT_SCREEN, 0, 0);   // Tell to ARM11 to init screen

    MCU::backlightScreen(true); // Turn on screen backlight
    Screen::clear();
}

void Screen::clear()
{
    PXI_DoCMD(PXI_CLEAR_SCREEN, 0, 0);
}

void Screen::swap()
{
    PXI_DoCMD(PXI_SWAP_FRAME_BUFFERS, 0, 0);
}

void Screen::shutdown()
{
    PXI_DoCMD(PXI_SHUTDOWN_SCREEN, 0, 0);
}

void Screen::fillcolor(screens screenType, u32 color)
{
    u32 args[] = {(u32)screenType, color};
    PXI_DoCMD(PXI_FILL_COLOR, args, 2);
}*/