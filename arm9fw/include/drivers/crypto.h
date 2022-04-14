#pragma once
#include <types.h>

#define AES_BLOCK_SIZE  0x10
#define SHA_256_HASH_SIZE   (256 / 8)
#define SHA_224_HASH_SIZE   (224 / 8)
#define SHA_1_HASH_SIZE     (160 / 8)

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
        Aes();
        void encrypt();
        void decrypt();
};