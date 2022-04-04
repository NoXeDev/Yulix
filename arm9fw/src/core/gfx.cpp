#include <core/gfx.h>
#include <pxi.h>

void Screen::init()
{
    MCU tmpMcuController = MCU();
    tmpMcuController.backlightScreen(true);
    PXI_DoCMD(PXI_INIT_SCREEN, 0, 0);   // Tell to ARM11 to init screen
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
    u32 args[] = {(u32)TOP_AND_BOTTOM_SCREEN, 0xFF0000};
    PXI_DoCMD(PXI_FILL_COLOR, args, 2);
}