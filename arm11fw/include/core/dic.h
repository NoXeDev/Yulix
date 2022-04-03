#pragma once
#include <types.h>
#include <core/regs.h>

class dic {
    public:
        static void reset();
        static void setupIRQ(u32 id, irq_handler handler);
        static void enableCPUInterrupt();
        static void disableCPUInterrupt();
};