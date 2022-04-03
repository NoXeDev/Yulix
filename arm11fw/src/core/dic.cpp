#include <core/dic.h>
#include <core/memory.h>

irq_handler GIC_handlerArray[128];

void dic::enableCPUInterrupt()
{
    asm("cpsie if\n\t"); // Enable IRQs and FIQs
}

void dic::disableCPUInterrupt()
{
    asm("cpsid if\n\t"); // Mask IRQs and FIQs
}

void dic::reset()
{
    // For reset properly all registers R/W on DIC and GIC lets first disable it
    REG_GIC_CONTROL = 0; // Disable the GIC Interrupts
    REG_DIC_CONTROL = 0; // Also disable DIC Interrupts

    // Now start a clear
    memset(GIC_handlerArray, 0, sizeof(GIC_handlerArray)); // reset all handler into the GIC_handlerArray

    // set the CLRENABLE AND CLRPENDING to her reset value : https://developer.arm.com/documentation/ddi0360/f/mpcore-distributed-interrupt-controller/interrupt-distributor-registers
    for (int i = 0; i < 4; i++) {
        REG_DIC_CLRENABLE[i]  = ~0;
        REG_DIC_CLRPENDING[i] = ~0;
    }

    // same for DIC_PRIORITY / TARGETPROC / CFGREG
    for (int i = 0; i < 32; i++)   REG_DIC_PRIORITY[i] = 0;
    for (int i = 32; i < 128; i++) REG_DIC_TARGETPROC[i] = 0;
    for (int i = 0; i < 8; i++)    REG_DIC_CFGREG[i] = ~0;
    REG_DIC_CONTROL = 1; // Enable DIC interrupts

    REG_DIC_CLRENABLE[0] = ~0;
    for (int i = 0; i < 32; i++) REG_DIC_PRIORITY[i] = 0;
    for (int i = 0; i < 2; i++) REG_DIC_CFGREG[i] = ~0;
    REG_GIC_POI = 3;
    REG_GIC_PRIOMASK = 0xF << 4;
    REG_GIC_CONTROL = 1;

    u32 irq_s;
    do {
        irq_s = REG_GIC_PENDING;
        REG_GIC_IRQEND = irq_s;
    } while(irq_s != 1023);
}

void dic::setupIRQ(u32 id, irq_handler handler)
{
    GIC_handlerArray[id] = handler;
    REG_DIC_CLRPENDING[id >> 5] |= (1 << (id & 0x1F));
    REG_DIC_SETENABLE[id >> 5] |= (1 << (id & 0x1F));
    REG_DIC_TARGETPROC[id] = 1;
}