#include <types.h>
#include <core/memory.h>
#include <pxi.h>
#include <core/dic.h>
#include <drivers/gpu.h>
#include <screenDefinitions.h>

extern "C" void PXI_IRQHandler(void)
{
    u32 pxi_args[PXI_FIFO_LEN];
    u32 pxi_cmd;
    u32 ret = 0;

    pxi_cmd = PXI_Recv();
    switch (pxi_cmd) {
        default:
            ret = 0xFFFFFFFF;
            break;

        case PXI_INIT_SCREEN:
        {
            PXI_RecvArray(pxi_args, 0);
            GPU::init(VRAM_FBS);
            break;
        }

        case PXI_CLEAR_SCREEN:
        {
            PXI_RecvArray(pxi_args, 0);
            GPU::clear(VRAM_FBS);
            break;
        }

        case PXI_SWAP_FRAME_BUFFERS:
        {
            PXI_RecvArray(pxi_args, 0);
            GPU::swap();
            break;
        }

        case PXI_SHUTDOWN_SCREEN:
        {
            PXI_RecvArray(pxi_args, 0);
            GPU::shutdownLcd();
            break;
        }

        case PXI_FILL_COLOR:
        {
            PXI_RecvArray(pxi_args, 2);
            if((screens)pxi_args[0] == TOP_SCREEN)
            {
                GPU::fillColor(true, pxi_args[1], VRAM_FBS);
            }
            if((screens)pxi_args[0] == BOTTOM_SCREEN)
            {
                GPU::fillColor(false, pxi_args[1], VRAM_FBS);
            }
            if((screens)pxi_args[0] == TOP_AND_BOTTOM_SCREEN)
            {
                GPU::fillColor(true, pxi_args[1], VRAM_FBS);
                GPU::fillColor(false, pxi_args[1], VRAM_FBS);
            }
            break;
        }

        case PXI_GET_ACTIVE_FRAMEBUFFER:
        {
            PXI_RecvArray(pxi_args, 0);
            ret = GPU::getActiveFramebuffer();
            break;
        }

        case PXI_TEST:
        {
            PXI_RecvArray(pxi_args, 0);
            GPU::init(VRAM_FBS);
            GPU::clear(VRAM_FBS);
            GPU::fillColor(true, 0x00FF00, VRAM_FBS);
            break;
        }
    }

    PXI_Send(ret);
}

extern "C" void arm11fw_main(void)
{
    PXI_Reset();
    dic::reset();

    dic::setupIRQ(IRQ_PXI_RX, PXI_IRQHandler);
    dic::enableCPUInterrupt();

    PXI_SetRemote(PXI_READY);

    do {
        asm("wfi\n\t");
    }while(true);
}
