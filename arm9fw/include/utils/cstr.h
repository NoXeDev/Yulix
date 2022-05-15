#pragma once
#include <types.h>
#include <type_traits>
#include <string.h>

char intTo_StrOut[128];
template <typename t>
const char* to_str(t value)
{
    // Filtering non supported types
    static_assert((std::is_same_v<t, s32> || std::is_same_v<t, u32>), "Type not supported now");

    u8 isNegative = 0;
    if constexpr (std::is_same_v<t, s32>)
    {
        if(value < 0)
        {
            isNegative = 1;
            value *= -1;
            // Put - char
            intTo_StrOut[0] = '-';
        }
    }
    u8 size;
    u32 sizeTest = value;
    while(sizeTest / 10 > 0)
    {
        sizeTest /= 10;
        size++;
    }

    u8 i = 0;
    while(value / 10 > 0)
    {
        u8 remainder = value % 10;
        value /= 10;
        intTo_StrOut[isNegative + size - i] = remainder + '0';
        i++;
    }

    u8 remainder = value % 10;
    intTo_StrOut[isNegative + size - i] = remainder + '0';
    intTo_StrOut[isNegative + size + 1] = 0;
    return intTo_StrOut;
}

char floatTo_StrOut[128];
const char* float_to_str(float value, u8 decimalPlaces)
{
    if(decimalPlaces > 5) decimalPlaces = 5;
    char* intPtr = (char *)to_str<s32>((s32)value);
    char* floatPtr = floatTo_StrOut;

    if(value < 0)
        value *= -1;

    while(*intPtr != 0)
    {
        *floatPtr = *intPtr;
        intPtr++;
        floatPtr++;
    }

    *floatPtr = '.';
    floatPtr++;

    float newVal = value - (int)value;

    for(u8 i = 0; i < decimalPlaces ; i++)
    {
        newVal *= 10;
        *floatPtr = (int)newVal + '0';
        newVal -= (int)newVal;
        floatPtr++;
    }

    *floatPtr = 0;
    return floatTo_StrOut;
}

char hexTo_StrOut[128];
template <typename t>
const char* to_hstring(t value, bool hex0X)
{
    static_assert((std::is_same_v<t, u32> || std::is_same_v<t, u16> || std::is_same_v<t, u8>), "Type not supported now");
    t* valPtr = &value;
    u8* ptr;
    u32 tmp;
    u32 size;
    u32 hexTypeOffset = 0;
    if(hex0X)
    {
        hexTypeOffset = 2;
        hexTo_StrOut[0] = '0';
        hexTo_StrOut[1] = 'x';
    }
    if constexpr(std::is_same_v<t, u32>)
    {
        size = 4 * 2 - 1;
    } else if constexpr(std::is_same_v<t, u16>)
    {
        size = 2 * 2 - 1;
    } else if constexpr(std::is_same_v<t, u8>)
    {
        size = 1 * 2 - 1;
    }

    for(u32 i = 0; i < size; i++)
    {
        ptr = ((u8*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StrOut[size - (i * 2 + 1) + hexTypeOffset] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StrOut[size - (i * 2) + hexTypeOffset] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StrOut[size + 1 + hexTypeOffset] = 0;
    return hexTo_StrOut;
}

char u8To_StrOut[128];
const char* u8_hstring(u8* datas, u8 size)
{
    if(size > 60) // for not do any potential char array overflow with a big array
        return "OOB";
    // Really unsafe !!!
    const char* firstVal = to_hstring<u8>(datas[0], true);
    u32 charArrayOffset = strlen(firstVal);
    strcpy(u8To_StrOut, firstVal);
    for(u8 i = 1; i < size; i++)
    {
        strcat(u8To_StrOut, to_hstring<u8>(datas[i], false));
    }

    return u8To_StrOut;
}