/*
 Inspired by the source code of Godmode9 written by Wolfvak
*/

#pragma once
#include <types.h>

#ifdef ARM9
#define PXI_BASE_REG    (0x10008000)
#define IRQ_PXI_SYNC (12)
#else
#define PXI_BASE_REG    (0x10163000)
#define IRQ_PXI_RX   (83)
#define IRQ_PXI_SYNC (80)
#endif

enum PXI_CMDS {
    PXI_NOOP = 0,
    PXI_READY,
    PXI_BUSY,
    PXI_INIT_SCREEN,
    PXI_CLEAR_SCREEN,
    PXI_SWAP_FRAME_BUFFERS,
    PXI_SHUTDOWN_SCREEN,
    PXI_FILL_COLOR,
    PXI_TEST
};

#define PXI_MAXBUFLEN (2048)
#define PXI_FIFO_LEN  (16)

struct PXI_REGS {
    vu8 PXI_SYNC_RECV;
    vu8 PXI_SYNC_SEND;
    vu8 PXI_UNKNOWN_REGISTER;
    vu8 PXI_IRQs_REGISTER;
    vu16 PXI_CNT;
    vu32 PXI_SEND;
    vu32 PXI_RECV;
};

#define PXI_CNT_SEND_FIFO_EMPTY_STATUS   (1 << 0)
#define PXI_CNT_SEND_FIFO_FULL_STATUS    (1 << 1)
#define PXI_CNT_SEND_FIFO_EMPTY_IRQ      (1 << 2)
#define PXI_CNT_SEND_FIFO_CLEAR          (1 << 3)
#define PXI_CNT_RECV_FIFO_EMPTY          (1 << 8)
#define PXI_CNT_RECV_FIFO_FULL           (1 << 9)
#define PXI_CNT_RECV_FIFO_NOT_EMPTY_IRQ  (1 << 10)
#define PXI_CNT_ERROR_ACK                (1 << 14)
#define PXI_CNT_ENABLE_FIFO              (1 << 15)

#define PXI_SYNC_ENABLE_IRQ              (1 << 7)
#define PXI_SYNC_TRIGGER_IRQ_ARM9        (1 << 6)
#define PXI_SYNC_TRIGGER_IRQ_ARM11       (1 << 5)

#define PXI_SYNC_DISABLE_IRQ             (0 << 7)

static PXI_REGS *pxi_regs = (PXI_REGS *)PXI_BASE_REG;

static inline void PXI_SetRemote(u8 msg)
{
    pxi_regs->PXI_SYNC_SEND = msg;
}

static inline u8 PXI_GetRemote(void)
{
    return pxi_regs->PXI_SYNC_RECV;
}

static inline void PXI_WaitRemote(u8 msg)
{
    while(PXI_GetRemote() != msg);
}

static inline void PXI_Sync(void)
{
    #ifdef ARM9
    pxi_regs->PXI_IRQs_REGISTER |= PXI_SYNC_TRIGGER_IRQ_ARM11;
    #else
    pxi_regs->PXI_IRQs_REGISTER |= PXI_SYNC_TRIGGER_IRQ_ARM9;
    #endif
}

static void PXI_Reset(void)
{
    pxi_regs->PXI_IRQs_REGISTER = 0;
    pxi_regs->PXI_CNT = PXI_CNT_SEND_FIFO_CLEAR | PXI_CNT_ENABLE_FIFO;
    for (int i=0; i<PXI_FIFO_LEN; i++) {
        pxi_regs->PXI_RECV;
    }
    pxi_regs->PXI_CNT = 0;
    pxi_regs->PXI_CNT = PXI_CNT_RECV_FIFO_NOT_EMPTY_IRQ | PXI_CNT_ENABLE_FIFO;
}

static inline void PXI_Send(u32 w)
{
    while(pxi_regs->PXI_CNT & PXI_CNT_SEND_FIFO_FULL_STATUS);
    pxi_regs->PXI_SEND = w;
}

static inline u32 PXI_Recv(void)
{
    while(pxi_regs->PXI_CNT & PXI_CNT_RECV_FIFO_EMPTY);
    return pxi_regs->PXI_RECV;
}

static void PXI_SendArray(const u32 *w, u32 c)
{
    if(c > PXI_FIFO_LEN) c = PXI_FIFO_LEN;
    while(c--)
		PXI_Send(*(w++));
}

static void PXI_RecvArray(u32 *w, u32 c)
{
    if(c > PXI_FIFO_LEN) c = PXI_FIFO_LEN;
    while(c--)
		*(w++) = PXI_Recv();
}

static u32 PXI_DoCMD(u8 cmd, const u32 *args, u32 argc)
{
    PXI_Send(cmd);
    PXI_SendArray(args, argc);
    return PXI_Recv();
}