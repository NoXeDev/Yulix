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

    __attribute__((aligned(4))) u8 cid[AES_BLOCK_SIZE];
    sdmmc_sdcard_init();
    sdmmc_get_cid(true, (u32 *)cid);

    __attribute__((aligned(4))) u8 cipherText[AES_BLOCK_SIZE],
    decryptedCipher[AES_BLOCK_SIZE];
    __attribute__((aligned(4))) u8 entry[AES_BLOCK_SIZE] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10};
        
    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 40}, 0x00FF00, "Original : ");
    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 50}, 0x00FF00, u8_hstring(entry, AES_BLOCK_SIZE));

    Aes cryptoTest = Aes(0x04, CTR_MODE);
    cryptoTest.process(cipherText, entry, cid, 1, AES_ENCRYPT, AES_CNT_INPUT_ENDIAN(1) | AES_CNT_INPUT_ORDER(1));

    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 70}, 0x00FF00, "Encrypted : ");
    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 80}, 0x00FF00, u8_hstring(cipherText, AES_BLOCK_SIZE));

    // Cid got edited so redump it
    sdmmc_get_cid(true, (u32 *)cid);

    cryptoTest.process(decryptedCipher, cipherText, cid, 1, AES_DECRYPT, AES_CNT_INPUT_ENDIAN(1) | AES_CNT_INPUT_ORDER(1));

    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 100}, 0x00FF00, "Decrypted : ");
    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 110}, 0x00FF00, u8_hstring(decryptedCipher, AES_BLOCK_SIZE));

    sdmmc_get_cid(true, (u32 *)cid);

    // TODO : fix the little graphics glitch here (duno why it happen)
    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 130}, 0x00FF00, "CID : ");
    GFX::printStr(TOP_SCREEN, CURRENT_FRAMEBUFFER, Vector2{10, 140}, 0x00FF00, u8_hstring(cid, AES_BLOCK_SIZE));

    while(HID_PAD & BUTTON_A);

    // TODO : Bootloader process

    Screen::shutdown();
    mcuManager.powerOff();

    while(true);
}