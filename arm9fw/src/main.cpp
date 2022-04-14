#include <core/mcu.h>
#include <pxi.h>
#include <screenDefinitions.h>
#include <core/gfx.h>
#include <utils/cstr.h>
#include <fatfs/sdmmc/sdmmc.h>
#include <drivers/crypto.h>
#include <core/memory.h>

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
    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 10}, 0x00FF00, "YulixCFW");
    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 20}, 0x00FF00, "Ready to boot !");
    //GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 40}, 0xFF0000, "Press A for boot.");

    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 40}, 0xFF0000, "CID was hashed");
    __attribute__((aligned(4))) u8 cid[AES_BLOCK_SIZE],
                                   shaSum[SHA_256_HASH_SIZE],
                                   nandCtr[AES_BLOCK_SIZE];

    sdmmc_sdcard_init();
    sdmmc_get_cid(true, (u32 *)cid);

    Sha hasher = Sha(SHA_256_MODE);
    hasher.hash(shaSum, cid, sizeof(cid));
    
    memcpy(nandCtr, shaSum, sizeof(nandCtr));

    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 60}, 0xFF00FF, u8_hstring(nandCtr, AES_BLOCK_SIZE));


    while(HID_PAD & BUTTON_A);

    // TODO : Bootloader process

    Screen::shutdown();
    mcuManager.powerOff();

    while(true);
}