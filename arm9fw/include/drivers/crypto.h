#pragma once
#include <types.h>

#define AES_BLOCK_SIZE  0x10
#define SHA_256_HASH_SIZE   (256 / 8)
#define SHA_224_HASH_SIZE   (224 / 8)
#define SHA_1_HASH_SIZE     (160 / 8)

enum AES_MODE {
    CCM_MODE,
    CTR_MODE,
    CBC_MODE,
    ECB_MODE
};

enum AES_OP
{
    AES_ENCRYPT,
    AES_DECRYPT
};

class Sha
{
    public:
        Sha(u32 mode);
        void hash(void *res, const void *src, u32 size);
    
    private:
        u32 mode;
};

class Aes
{
    public:
        Aes(u8 keyslot, AES_MODE modeOfAes);
        void process(void *dst, const void *src, void *iv, u32 blockCount, AES_OP operation, u32 ivMode);
    private:
        u8 keysolt;
        u32 cryptoModeCfg;
        AES_MODE modeOfAes;
};