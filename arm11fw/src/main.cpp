#include <types.h>
#include <core/memory.h>
#include <pxi.h>
#include <core/dic.h>
#include <drivers/gpu.h>
#include <screenDefinitions.h>

GPU *gpuManager;

extern "C" void PXI_IRQHandler(void)
{
    u32 pxi_args[PXI_FIFO_LEN]  = {0};
    u8 pxi_cmd;

    pxi_cmd = PXI_GetRemote();
    switch (pxi_cmd) {
        default:
            break;

        case PXI_INIT_SCREEN:
        {
            PXI_RecvArray(pxi_args, 0);
            PXI_SetRemote(PXI_BUSY);
            gpuManager->init(VRAM_FBS);
            break;
        }

        case PXI_CLEAR_SCREEN:
        {
            PXI_RecvArray(pxi_args, 0);
            PXI_SetRemote(PXI_BUSY);
            gpuManager->clear(VRAM_FBS);
            break;
        }

        case PXI_SWAP_FRAME_BUFFERS:
        {
            PXI_RecvArray(pxi_args, 0);
            PXI_SetRemote(PXI_BUSY);
            gpuManager->swap();
            break;
        }

        case PXI_SHUTDOWN_SCREEN:
        {
            PXI_RecvArray(pxi_args, 0);
            PXI_SetRemote(PXI_BUSY);
            gpuManager->shutdownLcd();
            break;
        }

        case PXI_FILL_COLOR:
        {
            PXI_RecvArray(pxi_args, 2);
            PXI_SetRemote(PXI_BUSY);
            if((screens)pxi_args[0] == TOP_SCREEN)
            {
                gpuManager->fillColor(true, pxi_args[1], VRAM_FBS);
            }
            if((screens)pxi_args[0] == BOTTOM_SCREEN)
            {
                gpuManager->fillColor(false, pxi_args[1], VRAM_FBS);
            }
            if((screens)pxi_args[0] == TOP_AND_BOTTOM_SCREEN)
            {
                gpuManager->fillColor(true, pxi_args[1], VRAM_FBS);
                gpuManager->fillColor(false, pxi_args[1], VRAM_FBS);
            }
            break;
        }

        /*case PXI_TEST:
        {
            PXI_RecvArray(pxi_args, 2);
            PXI_SetRemote(PXI_BUSY);

            if(pxi_args[0] == 0xBABE && pxi_args[1] == 0xCAFE)
            {
                gpuManager->fillColor(true, 0x00F000, VRAM_FBS);
            }
            break;
        }*/
    }

    PXI_SetRemote(PXI_READY);
}

extern "C" void arm11fw_main(void)
{
    PXI_Reset();
    dic::reset();

    *gpuManager = GPU();    // Init class

    dic::setupIRQ(IRQ_PXI_SYNC, PXI_IRQHandler);
    PXI_EnableIRQ();
    dic::enableCPUInterrupt();

    PXI_SetRemote(PXI_READY);

    do {
        asm("wfi\n\t");
    }while(true);
}
