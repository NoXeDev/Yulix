#pragma once
#include <types.h>

struct FrameBuffers {
    u8 *top_left;
    u8 *top_right;
    u8 *bottom;
};

enum FrameBufferType
{
    CURRENT_FRAMEBUFFER,
    ALTERNATE_FRAMEBUFFER,
    ALL_FRAMEBUFFERS
};

enum screens
{
    TOP_SCREEN,
    BOTTOM_SCREEN,
    TOP_AND_BOTTOM_SCREEN
};

// Define FrameBuffer (const)
static const struct FrameBuffers VRAM_FBS[2] = {
    {
        (u8 *)0x18300000,
        (u8 *)0x18300000,
        (u8 *)0x18346500,
    },
    {
        (u8 *)0x18400000,
        (u8 *)0x18400000,
        (u8 *)0x18446500,
    }
};

#define SCREEN_TOP_WIDTH     400
#define SCREEN_BOTTOM_WIDTH  320
#define SCREEN_HEIGHT        240
#define SCREEN_TOP_FBSIZE    (3 * SCREEN_TOP_WIDTH * SCREEN_HEIGHT)
#define SCREEN_BOTTOM_FBSIZE (3 * SCREEN_BOTTOM_WIDTH * SCREEN_HEIGHT)