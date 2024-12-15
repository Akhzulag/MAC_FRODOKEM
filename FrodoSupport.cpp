
#include "FrodoSupport.h"
#include "parameters.h"

#include <cmath>
#include <cstddef>
#include <cstdint>

void frodo_encode(uint16_t* K, uint16_t* k)
{
    size_t lenK = B * 4; // B*\overline{m}*\overline{n} / 16
    size_t mask = (1 << B) - 1;
    size_t bitIter = 0;

    size_t mul = (1 << (D - B));
    size_t kIter = 0;
    size_t tmp = 0;
    for (size_t i = 0; i < MATRIX_LEN; ++i)
    {
        if (i % 16 == 0 && i > 0)
        {
            tmp = ((k[kIter] >> (bitIter - bitIter % 16)) & (mask));
            kIter += 1;
            bitIter %= 16;
        }

        tmp |= ((k[kIter] >> bitIter) & (mask));
        K[i / nm + i % nm] = mul * tmp; //
        bitIter += B;
    }
}

void frodo_decode(uint8_t* K, uint16_t* k) {}
