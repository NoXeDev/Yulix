#pragma once
#include <types.h>
#include <screenDefinitions.h>

class GPU
{
    public:
        static void gpuPowerOn();
        static void lcdFillBlack(bool off);
        static void lcdInit(u32 brightnessLevel);
        static void init(const FrameBuffers fbs[2]);
        static void clear(const FrameBuffers fb);
        static void swap();
        static void shutdownLcd();
        static void fillColor(bool isTop, u32 color, const FrameBuffers fbs[2]);
        static bool getActiveFramebuffer();
};