#include <core/gfx.h>
#include <pxi.h>
#include <font.h>
#include <string.h>

void Screen::init()
{
    MCU tmpMcuController = MCU();
    tmpMcuController.backlightScreen(true);
    PXI_DoCMD(PXI_INIT_SCREEN, 0, 0);   // Tell to ARM11 to init screen
    Screen::clear(ALL_FRAMEBUFFERS);
}

void Screen::clear(FrameBufferType framebfr)
{
    u32 args[] = {(u32)framebfr};
    PXI_DoCMD(PXI_CLEAR_SCREEN, args, 1);
}

void Screen::swap()
{
    PXI_DoCMD(PXI_SWAP_FRAME_BUFFERS, 0, 0);
}

void Screen::shutdown()
{
    Screen::clear(ALL_FRAMEBUFFERS);
    PXI_DoCMD(PXI_SHUTDOWN_SCREEN, 0, 0);
    MCU tmpMcuController = MCU();
    tmpMcuController.backlightScreen(false);
}

void Screen::fillcolor(screens screenType, u32 color)
{
    u32 args[] = {(u32)TOP_AND_BOTTOM_SCREEN, 0xFF0000};
    PXI_DoCMD(PXI_FILL_COLOR, args, 2);
}

void GFX::putChar(screens screenCtx, bool currentFrameBufferState, FrameBufferType framebfr, u32 posX, u32 posY, u32 color, char data)
{
    // TOP_AND_BOTTOM_SCREEN have no impact here
    u8 *FrameBuffer;
    if(screenCtx == TOP_SCREEN)
    {
        FrameBuffer = VRAM_FBS[(framebfr == CURRENT_FRAMEBUFFER ? currentFrameBufferState : !currentFrameBufferState)].top_left;
    } else 
    {
        FrameBuffer = VRAM_FBS[(framebfr == CURRENT_FRAMEBUFFER ? currentFrameBufferState : !currentFrameBufferState)].bottom;
    }

    for(u32 y = 0; y < 8; y++)
    {
        char charInFontPos = font[data * 8 + y];
        for(u32 x = 0; x < 8; x++)
        {
            if(((charInFontPos >> (7 - x)) & 1) == 1)
            {
                u32 screenPos = (posX * SCREEN_HEIGHT * 3 + (SCREEN_HEIGHT - y - posY - 1) * 3) + x * 3 * SCREEN_HEIGHT;

                // Write RGB font char according to screenPos calculation
                FrameBuffer[screenPos] = color;
                FrameBuffer[screenPos + 1] = color >> 8;
                FrameBuffer[screenPos + 2] = color >> 16;
            }
        }
    }
}

u32 GFX::printStr(screens screenCtx, FrameBufferType framebfr, Vector2 positions, u32 color, const char *text)
{
    u32 line_i = 0;
    for(u32 i = 0; i < strlen(text); i++)
    {
        if(line_i >= (((screenCtx ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH) - positions.y) / SPACING_X))
        {
            positions.y += SPACING_Y;
            line_i = 1;
            if(text[i] == ' ') break;
        }

        GFX::putChar(screenCtx, (bool)PXI_DoCMD(PXI_GET_ACTIVE_FRAMEBUFFER, 0, 0), framebfr, (positions.x + line_i * SPACING_X), positions.y, color, text[i]);

        line_i++;
    }

    return positions.y;
}