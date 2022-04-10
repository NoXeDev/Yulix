#include <core/mcu.h>
#include <pxi.h>
#include <screenDefinitions.h>
#include <core/gfx.h>
#include <utils/cstr.h>

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

    //Screen::fillcolor(TOP_AND_BOTTOM_SCREEN, 0xFFF000);
    GFX::printStr(TOP_SCREEN, Vector2{10, 10}, 0x00FF00, "YulixCFW");
    GFX::printStr(TOP_SCREEN, Vector2{10, 20}, 0x00FF00, "Ready to boot !");
    GFX::printStr(TOP_SCREEN, Vector2{10, 50}, 0xFF0000, to_hstring<u8>(0x5F));
    
    while(HID_PAD & BUTTON_A(false));

    Screen::shutdown();
    mcuManager.powerOff();

    while(true);
}