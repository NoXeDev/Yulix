#include <core/memory.h>

void memcpy(void *dest, const void *src, u32 size)
{
    u8 *destc = (u8 *)dest;
    const u8 *srcc = (const u8 *)src;

    for(u32 i = 0; i < size; i++)
        destc[i] = srcc[i];
}

void memset(void *dest, u32 filler, u32 size)
{
    u8 *destc = (u8 *)dest;

    for(u32 i = 0; i < size; i++)
        destc[i] = (u8)filler;
}