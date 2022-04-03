#pragma once

#include <types.h>
#include <screenDefinitions.h>

class Screen
{
    public:
        static void init();
        static void clear();
        static void swap();
        static void shutdown();
        static void fillcolor(screens screenType, u32 color);
};

class GFX 
{
    public:
        
};