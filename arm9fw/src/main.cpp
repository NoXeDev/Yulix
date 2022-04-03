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
    mcuManager.wifiLED(true);

    // Wait for ARM11 to be ready
    PXI_WaitRemote(PXI_READY);

    /*Screen::init();
    Screen::fillcolor(TOP_AND_BOTTOM_SCREEN, 0xFF0000);*/

    while(true);
}