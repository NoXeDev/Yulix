#include <core/mcu.h>
#include <pxi.h>
#include <screenDefinitions.h>
#include <core/gfx.h>

// entry point of cpp code
extern "C" void arm9fwmain()
{
    PXI_Reset();

    // Power on backlight and wifi led
    MCU mcuManager = MCU();

    // Wait for ARM11 to be ready
    PXI_WaitRemote(PXI_READY);
    mcuManager.wifiLED(true);

    Screen::init();

    Screen::fillcolor(TOP_AND_BOTTOM_SCREEN, 0xFFF000);

    while(true);
}