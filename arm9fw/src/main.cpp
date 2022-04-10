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
    I2C i2cManager = I2C(I2C_DEV_MCU);

    // Wait for ARM11 to be ready
    PXI_WaitRemote(PXI_READY);
    mcuManager.wifiLED(true);

    Screen::init();

    //Screen::fillcolor(TOP_AND_BOTTOM_SCREEN, 0xFFF000);

    u8 oldVolumeVal = 0xFF;
    do {
        u8 volumeSlide = i2cManager.i2cReadRegister(0x09);
        if(oldVolumeVal != volumeSlide)
        {
            Screen::clear(ALTERNATE_FRAMEBUFFER);
            GFX::printStr(TOP_SCREEN, ALTERNATE_FRAMEBUFFER, Vector2{10, 10}, 0x00FF00, "YulixCFW");
            GFX::printStr(TOP_SCREEN, ALTERNATE_FRAMEBUFFER, Vector2{10, 20}, 0x00FF00, "Ready to boot !");
            GFX::printStr(TOP_SCREEN, ALTERNATE_FRAMEBUFFER, Vector2{10, 50}, 0xFF0000, to_hstring<u8>(volumeSlide));
            Screen::swap();
            oldVolumeVal = volumeSlide;
        }
        if(!(HID_PAD & BUTTON_A(false)))
        {
            break;
        }
    }
    while(true);

    Screen::shutdown();
    mcuManager.powerOff();

    while(true);
}