
#include "FrodoSupport.h"
#include "parameters.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iterator>

void frodo_encode(uint16_t* K, uint16_t* k)
{
    size_t mask = (1 << B) - 1;
    size_t mask16bit = (1 << 16) - 1;
    size_t bitIter = 0;

    size_t mul = (1 << (D - B));
    size_t kIter = 0;
    size_t tmp = 0;
    for (size_t i = 0; i < MATRIX_LEN; ++i)
    {
        tmp |= ((k[kIter] >> bitIter) & (mask));
        K[i] = mul * tmp;
        bitIter += B;
        tmp = 0;
        if (i % B_IN_16 == 0 && i > 0)
        {
            tmp = ((k[kIter] >> (bitIter - (bitIter ^ mask16bit))) & (mask));
            ++kIter;
            bitIter ^= mask16bit;
        }
    }
}

void frodo_decode(uint16_t* K, uint16_t* k)
{
    size_t modB = (1 << B) - 1; //  mod 2^B
    size_t mask16bit = (1 << 16) - 1;
    double mul = double(1 << B) / double(1 << D); // 2^B / q

    size_t kIter = 0;
    size_t bitIter = 0;
    int tmp = 0;
    for (size_t i = 0; i < MATRIX_LEN; ++i)
    {
        bitIter ^= mask16bit;
        tmp = uint16_t((0.5 + K[i] * mul)) & modB;
        if (i % B_IN_16 == 0 && i > 0)
        {
            k[kIter] |= (tmp << (bitIter)&mask16bit);
            ++kIter;
            k[kIter] = tmp >> (B - 16 + bitIter);
            bitIter ^= mask16bit;
        }
        else
        {
            k[kIter] |= tmp << bitIter;
        }
        bitIter += B;
    }
}

std::uint16_t rotate16bit(uint16_t a)
{
    for (int i = 0; i < 8; ++i)
    {
        a = (a >> i) | (a << i);
    }
    if (D == 15)
        a = a >> 1;
    return a;
}

void frodo_pack(uint16_t* C, uint8_t* b, uint16_t n1, uint16_t n2)
{
    size_t matrix_len = n1 * n2;
    size_t mask = (1 << 16) - 1;
    size_t kIter = 0;
    size_t bitIter = 0;
    size_t bitCut = 8;
    size_t bit = 0;
    uint16_t tmp = 0;
    int i = 0;
    int bI;
    while (i < matrix_len)
    {
        tmp = rotate16bit(C[i]);

        // maybe bitIter and bit should make in one
        // parametr, and take while % D != 0, and if % 8 == 0
        while (bitIter != D)
        {
            b[bI] |= (tmp >> bitIter) & (mask >> bitCut);
            bitIter += bitCut;
            bit += bitCut;
            bitCut = (D - bitIter); // mod

            if (bit % 8 == 0 && bit > 0)
                ++bI;
        }

        bitIter = 0;
        bitCut = 8 - bitCut;
        i += 1;
    }
}

void unrotate16bit(uint16_t& a)
{
    if (D == 15)
        a = a << 1;

    for (int i = 0; i < 8; ++i)
    {
        a = (a >> i) | (a << i);
    }
}

void frodo_unpack(uint16_t* C, uint8_t* b, uint16_t n1, uint16_t n2)
{
    size_t matrix_len = n1 * n2;
    size_t mask = (1 << 16) - 1;
    size_t kIter = 0;
    size_t bitIter = 0;
    size_t bitCut = 8;
    size_t bit = 0;
    uint16_t tmp = 0;
    int i = 0;
    int bI;
    while (i < matrix_len)
    {

        while (bitIter != D)
        {

            bitIter += bitCut;
            bit += bitCut;
            bitCut = (D - bitIter); // mod

            if (bit % 8 == 0 && bit > 0)
            {
                ++bI;
            }
        }
    }
}
