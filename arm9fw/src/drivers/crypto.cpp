#include <drivers/crypto.h>
#include <fatfs/sdmmc/sdmmc.h>
#include <core/regs.h>
#include <core/memory.h>
#include <utils/alignedseqmemcpy.h>

Sha::Sha(u32 mode)
{
    this->mode = mode;
}

static void sha_wait_idle()
{
    while(*SHA_CNT & 1);
}

// WARNING : ***** WE EXCEPT THE RES AND SRC TO BE 4 ALIGNED (__attribute__((aligned(4)))) *****
void Sha::hash(void *res, const void *src, u32 size)
{
    sha_wait_idle();
    *SHA_CNT = this->mode | SHA_CNT_OUTPUT_ENDIANESS(1) | SHA_CNT_FINAL_ROUND(0) | SHA_CNT_START;

    const u8 *src8 = (const u8 *)src;
    while(size >= 0x40)
    {
        sha_wait_idle();
        alignedseqmemcpy((void*)SHA_FIFO, src8, 0x40);

        src8 += 0x40;
        size -= 0x40;
    }

    sha_wait_idle();
    alignedseqmemcpy((void *)SHA_FIFO, src8, size);

    *SHA_CNT = (*SHA_CNT & ~SHA_CNT_FINAL_ROUND(0)) | SHA_CNT_FINAL_ROUND(1);

    while(*SHA_CNT & SHA_CNT_FINAL_ROUND(1));

    sha_wait_idle();

    u32 hashSize = SHA_256_HASH_SIZE;
    if(this->mode == SHA_224_MODE)
        hashSize = SHA_224_HASH_SIZE;

    alignedseqmemcpy(res, (void*)SHA_HASH, hashSize);
}