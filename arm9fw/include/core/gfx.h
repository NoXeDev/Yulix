#pragma once

#include <types.h>
#include <screenDefinitions.h>
#include <core/mcu.h>

#define SPACING_Y 10
#define SPACING_X 8

struct Vector2
{
    u32 x;
    u32 y;
};

class Screen
{
    public:
        static void init();
        static void clear(FrameBufferType framebfr);
        static void swap();
        static void shutdown();
        static void fillcolor(screens screenType, u32 color);
};

class GFX 
{
    public:
        static void putChar(screens screenCtx, bool currentFrameBufferState, FrameBufferType framebfr, u32 posX, u32 posY, u32 color, char data);
        static u32 printStr(screens screenCtx, FrameBufferType framebfr, Vector2 positions, u32 color, const char *text);
};