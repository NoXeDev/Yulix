#include <drivers/crypto.h>
#include <fatfs/sdmmc/sdmmc.h>
#include <core/regs.h>
#include <core/memory.h>
#include <utils/alignedseqmemcpy.h>

/* original version by megazig */

#ifndef __thumb__
#define BSWAP32(x) {\
	asm\
	(\
		"eor r1, %1, %1, ror #16\n\t"\
		"bic r1, r1, #0xFF0000\n\t"\
		"mov %0, %1, ror #8\n\t"\
		"eor %0, %0, r1, lsr #8\n\t"\
		:"=r"(x)\
		:"0"(x)\
		:"r1"\
	);\
};

#define ADD_u128_u32(u128_0, u128_1, u128_2, u128_3, u32_0) {\
asm\
	(\
		"adds %0, %4\n\t"\
		"addcss %1, %1, #1\n\t"\
		"addcss %2, %2, #1\n\t"\
		"addcs %3, %3, #1\n\t"\
		: "+r"(u128_0), "+r"(u128_1), "+r"(u128_2), "+r"(u128_3)\
		: "r"(u32_0)\
		: "cc"\
	);\
}
#else
#define BSWAP32(x) {x = __builtin_bswap32(x);}

#define ADD_u128_u32(u128_0, u128_1, u128_2, u128_3, u32_0) {\
asm\
	(\
		"mov r4, #0\n\t"\
		"add %0, %0, %4\n\t"\
		"adc %1, %1, r4\n\t"\
		"adc %2, %2, r4\n\t"\
		"adc %3, %3, r4\n\t"\
		: "+r"(u128_0), "+r"(u128_1), "+r"(u128_2), "+r"(u128_3)\
		: "r"(u32_0)\
		: "cc", "r4"\
	);\
}
#endif /*__thumb__*/

/////////////////////////////////////////////

// TuxSH code
static void aes_change_ctrmode(void *ctr, u32 fromMode, u32 toMode)
{
    u32 *ctr32 = (u32 *)ctr;
    int i;
    if((fromMode ^ toMode) & AES_CNT_INPUT_ENDIAN(1))
    {
        for(i = 0; i < 4; ++i)
            BSWAP32(ctr32[i]);
    }

    if((fromMode ^ toMode) & AES_CNT_INPUT_ORDER(1))
    {
        u32 temp = ctr32[0];
        ctr32[0] = ctr32[3];
        ctr32[3] = temp;

        temp = ctr32[1];
        ctr32[1] = ctr32[2];
        ctr32[2] = temp;
    }
}

static void aes_batch(void *dst, const void *src, u32 blockCount)
{
    *AES_BLKCNT = blockCount << 16;
    *AES_CNT |=  AES_CNT_START;

    const u32 *src32    = (const u32 *)src;
    u32 *dst32          = (u32 *)dst;

    u32 wbc = blockCount;
    u32 rbc = blockCount;

    while(rbc)
    {
        if(wbc && ((*AES_CNT & 0x1F) <= 0xC)) //There's space for at least 4 ints
        {
            *AES_WRFIFO = *src32++;
            *AES_WRFIFO = *src32++;
            *AES_WRFIFO = *src32++;
            *AES_WRFIFO = *src32++;
            wbc--;
        }

        if(rbc && ((*AES_CNT & (0x1F << 0x5)) >= (0x4 << 0x5))) //At least 4 ints available for read
        {
            *dst32++ = *AES_RDFIFO;
            *dst32++ = *AES_RDFIFO;
            *dst32++ = *AES_RDFIFO;
            *dst32++ = *AES_RDFIFO;
            rbc--;
        }
    }
}

static void aes_advctr(void *ctr, u32 val, u32 mode)
{
    u32 *ctr32 = (u32 *)ctr;

    int i;
    if(mode & AES_CNT_INPUT_ENDIAN(1))
    {
        for(i = 0; i < 4; ++i) //Endian swap
            BSWAP32(ctr32[i]);
    }

    if(mode & AES_CNT_INPUT_ORDER(1))
    {
        ADD_u128_u32(ctr32[3], ctr32[2], ctr32[1], ctr32[0], val);
    }
    else
    {
        ADD_u128_u32(ctr32[0], ctr32[1], ctr32[2], ctr32[3], val);
    }

    if(mode & AES_CNT_INPUT_ENDIAN(1))
    {
        for(i = 0; i < 4; ++i) //Endian swap
            BSWAP32(ctr32[i]);
    }
}

static void aes_setiv(const void *iv, u32 mode)
{
    const u32 *iv32 = (const u32 *)iv;
    *AES_CNT = (*AES_CNT & ~(AES_CNT_INPUT_ENDIAN(1) | AES_CNT_INPUT_ORDER(1))) | mode;

    //Word order for IV can't be changed in REG_AESCNT and always default to reversed
    if(mode & AES_CNT_INPUT_ORDER(1))
    {
        AES_CTR[0] = iv32[3];
        AES_CTR[1] = iv32[2];
        AES_CTR[2] = iv32[1];
        AES_CTR[3] = iv32[0];
    }
    else
    {
        AES_CTR[0] = iv32[0];
        AES_CTR[1] = iv32[1];
        AES_CTR[2] = iv32[2];
        AES_CTR[3] = iv32[3];
    }
}
//////

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

Aes::Aes(u8 keyslot, AES_MODE modeOfAes)
{
    // Brief configuration of the instance
    this->keysolt = keyslot;
    this->modeOfAes = modeOfAes;
    if(modeOfAes == CTR_MODE)
    {
        this->cryptoModeCfg = AES_CTR_MODE;
    }
}

void Aes::process(void *dst, const void *src, void *iv, u32 blockCount, AES_OP operation, u32 ivMode)
{
    // First need select keyslot
    if(this->keysolt > 0x3F)
        return;

    *AES_KEYSEL = keysolt;
    *AES_CNT |= (1 << 26); // Update keyslot

    /////////////////////////

    /// Determine mode of encryption / decryption
    switch (this->modeOfAes)
    {
        case CTR_MODE:
            // Already configured
            break;

        case CBC_MODE:
        {
            this->cryptoModeCfg = (operation == AES_ENCRYPT ? AES_CBC_ENCRYPT_MODE : AES_CBC_DECRYPT_MODE);
            break;
        }
        
        case ECB_MODE:
        {
            this->cryptoModeCfg = (operation == AES_ENCRYPT ? AES_ECB_ENCRYPT_MODE : AES_ECB_DECRYPT_MODE);
            break;
        }

        default:
            break;
    }

    /*  First fill CNT register
        Parameters : 
            - Normal input order
            - Normal output order
            - Big endian input
            - Big endian output
            - Flush read and write
    */
    *AES_CNT = this->cryptoModeCfg | AES_CNT_INPUT_ORDER(1) | AES_CNT_OUTPUT_ORDER(1) | AES_CNT_INPUT_ENDIAN(1) | AES_CNT_OUTPUT_ENDIAN(1) | AES_CNT_FLUSH_READ | AES_CNT_FLUSH_WRITE;
    //

    u32 blocks;
    while(blockCount != 0)
    {
        if(this->modeOfAes != ECB_MODE)
        {
            // Set IV
            aes_setiv(iv, ivMode);
        }
        blocks = (blockCount >= 0xFFFF) ? 0xFFFF : blockCount; // Limit block count

        //Save the last block for the next decryption CBC batch's iv (TuxSH code)
        if((this->modeOfAes == CBC_MODE) && (operation == AES_DECRYPT))
        {
            memcpy(iv, src + (blocks - 1) * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
            aes_change_ctrmode(iv, AES_CNT_INPUT_ENDIAN(1) | AES_CNT_INPUT_ORDER(1), ivMode);
        }

        //Process the current batch
        aes_batch(dst, src, blocks);

        if((this->modeOfAes == CBC_MODE) && (operation == AES_ENCRYPT))
        {
            memcpy(iv, dst + (blocks - 1) * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
            aes_change_ctrmode(iv, AES_CNT_INPUT_ENDIAN(1) | AES_CNT_INPUT_ORDER(1), ivMode);
        } else if(this->modeOfAes == CTR_MODE)
        {
            //advctr
            aes_advctr(iv, blocks, ivMode);
        }

        src += blocks * AES_BLOCK_SIZE;
        dst += blocks * AES_BLOCK_SIZE;
        blockCount -= blocks;
    }
}