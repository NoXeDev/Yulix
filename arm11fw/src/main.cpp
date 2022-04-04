#include <types.h>
#include <core/memory.h>
#include <pxi.h>
#include <core/dic.h>
#include <drivers/gpu.h>
#include <screenDefinitions.h>

GPU *gpuManager;

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

        case PXI_TEST:
        {
            PXI_RecvArray(pxi_args, 0);
            PXI_SetRemote(PXI_BUSY);
            gpuManager->init(VRAM_FBS);
            gpuManager->clear(VRAM_FBS);
            gpuManager->fillColor(true, 0x00FF00, VRAM_FBS);
            break;
        }
    }

    PXI_Send(ret);
}

extern "C" void arm11fw_main(void)
{
    PXI_Reset();
    dic::reset();

    *gpuManager = GPU();    // Init class

    dic::setupIRQ(IRQ_PXI_RX, PXI_IRQHandler);
    dic::enableCPUInterrupt();

    PXI_SetRemote(PXI_READY);

    do {
        asm("wfi\n\t");
    }while(true);
}
