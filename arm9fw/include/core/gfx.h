#pragma once

#include <types.h>
#include <screenDefinitions.h>
#include <core/mcu.h>

class Screen
{
    public:
        void init();
        void clear();
        void swap();
        void shutdown();
        void fillcolor(screens screenType, u32 color);

    private:
        MCU *mcuManager;
};

class GFX 
{
    public:
        
};