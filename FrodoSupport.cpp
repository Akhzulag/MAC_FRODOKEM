#include "FrodoSupport.h"
#include "parameters.h"

#include <cmath>
#include <filesystem>
#include <iterator>
#include <string>

void frodo_encode(uint16_t* K, uint16_t* k)
{
    size_t mask = (1 << B) - 1;
    size_t mask16bit = (1 << 16) - 1;
    size_t bitIter = 0;

    size_t mul = (1 << (D - B));
    size_t kIter = 0;
    uint16_t tmp = 0;
    int v = B_IN_16;
    size_t bit = 0;
    for (int i = 0; i < MATRIX_LEN; ++i)
    {
#ifndef FRODO
        if ((bitIter == 14 || bitIter == 15) && i > 0 && i % 15 != 0)
        {
            tmp = ((k[bit / 16] >> bitIter) & (mask >> (B - 16 + bitIter)));
            bit += 16 - bitIter;
            tmp |= ((k[bit / 16]) & (mask >> (16 - bitIter))) << (16 - bitIter);
            bit += B - 16 + bitIter;
            K[i] = mul * tmp;
            bitIter = B - 16 + bitIter;
            tmp = 0;
            continue;
        }
#else
        if (i % v == 0 && i > 0)
        {
            ++kIter;
        }
#endif

        tmp |= ((k[bit / 16] >> bitIter) & mask);
        K[i] = mul * tmp;
        bitIter = (bitIter + B) % 16;
        bit += B;
        tmp = 0;
    }
}

void frodo_decode(uint16_t* K, uint16_t* k)
{
    size_t mask = (1 << B) - 1; //  mod 2^B
    size_t mask16bit = (1 << 16) - 1;
    double mul = double(1 << B) / double(1 << D); // 2^B / q

    size_t kIter = 0;
    size_t bitIter = 0;
    int tmp = 0;
    int v = B_IN_16;
    size_t bit = 0;
    for (size_t i = 0; i < MATRIX_LEN; ++i)
    {
#ifndef FRODO
        if ((bitIter == 15 || bitIter == 14) && i > 0 && i % 15 != 0)
        {
            tmp = uint16_t(double(K[i]) * mul + 0.5) & mask;
            k[bit / 16] |= (tmp << bitIter) & mask16bit;

            bit += 16 - bitIter;
            k[bit / 16] |= tmp >> (16 - bitIter);
            bit += B - 16 + bitIter;

            bitIter = B - 16 + bitIter;

            continue;
        }
#else
        if (i % v == 0 && i > 0)
        {
            ++kIter;
        }
#endif
        tmp = uint16_t(double(K[i]) * mul + 0.5) & mask;
        k[bit / 16] |= tmp << bitIter;
        bitIter = (bitIter + B) % 16;
        bit += B;
    }
}

std::uint16_t rotate16bit(uint16_t a)
{
    uint16_t mask16 = (1 << 16) - 1;
    uint32_t mask15 = (1 << 15);
    for (int i = 0; i < 8; ++i)
    {
        uint16_t mask = (1 << 16) - 1;
        mask = (mask) ^ (1 << (15 - i)) & (mask) ^ (1 << i);
        uint16_t tmp = (a << (15 - 2 * i));
        uint16_t bit1 = ((a << (15 - 2 * i)) & (mask15 >> i));
        uint16_t bit2 = ((a >> (15 - 2 * i)) & (1 << i));
        a = (mask & a) | ((a << (15 - 2 * i)) & (mask15 >> i)) |
            ((a >> (15 - 2 * i)) & (1 << i));
    }
    if (D == 15)
        a = a >> 1;
    return a;
}

void frodo_pack(uint16_t* C, uint8_t* b, uint16_t n1, uint16_t n2)
{
    size_t matrix_len = n1 * n2;
    size_t mask = (1 << 8) - 1;
    size_t kIter = 0;
    size_t bitIter = 0;
    int bitCut = 8;
    size_t bit = 0;
    uint16_t tmp = 0;
    int i = 0;
    int bI = 0;

    while (i < matrix_len)
    {
        tmp = rotate16bit(C[i]);

        while (bitIter != D)
        {
            b[bit / 8] |= ((((tmp >> bitIter) & (mask >> (8 - bitCut)))))
                          << (bit % 8);
            bitIter += bitCut;
            bit += bitCut;
            if (D - bitIter == 0)
            {
                bitCut = 8 - bitCut;
                break;
            }
            if (D - bitIter >= 8)
            {
                bitCut = 8;
                continue;
            }
            bitCut = (D - bitIter); // mod

            bI = bit / 8;
        }

        bitIter = 0;
        i += 1;
    }
}

void unrotate16bit(uint16_t& a)
{
    if (D == 15)
        a = a << 1;
    uint16_t mask16 = (1 << 16) - 1;
    uint32_t mask15 = (1 << 15);
    for (int i = 0; i < 8; ++i)
    {
        uint16_t mask = (1 << 16) - 1;
        mask = (mask) ^ (1 << (15 - i)) & (mask) ^ (1 << i);
        uint16_t tmp = (a << (15 - 2 * i));
        uint16_t bit1 = ((a << (15 - 2 * i)) & (mask15 >> i));
        uint16_t bit2 = ((a >> (15 - 2 * i)) & (1 << i));
        a = (mask & a) | ((a << (15 - 2 * i)) & (mask15 >> i)) |
            ((a >> (15 - 2 * i)) & (1 << i));
    }
}

void frodo_unpack(uint16_t* C, uint8_t* b, uint16_t n1, uint16_t n2)
{
    size_t matrix_len = n1 * n2;
    size_t mask = (1 << 8) - 1;
    size_t kIter = 0;
    int bitIter = 0;
    int bitCut = 8;
    size_t bit = 0;
    size_t bit8Cut = 0;
    uint8_t tmp = b[0];
    int i = 0;
    int bI = 0;
    int bitCutPrev = 0;
    while (i < matrix_len)
    {
        while (bitIter != D)
        {
            C[i] |= (b[bit / 8] >> bit8Cut & (mask >> (8 - bitCut)))
                    << (bitIter);

            bitIter += bitCut;
            bit += bitCut;
            bit8Cut = (bit8Cut + bitCut) % 8;
            if (D - bitIter == 0)
            {
                bitCut = 8 - bitCut;
                break;
            }
            if (D - bitIter >= 8)
            {
                bitCut = 8;
                continue;
            }
            bitCut = (D - bitIter);

            bI = bit / 8;
        }

        bitIter = 0;

        unrotate16bit(C[i]);
        ++i;
    }
}

uint32_t hexCharToNumber(char& hex)
{
    switch (hex)
    {
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default:
            if (hex > '0' && hex <= '9')
                return hex - '0';
            return 0; // TODO here must be exception, when get number32bit from
                      // other system
    }
}

void frodo_uint16(uint16_t* H, size_t len, std::string hex)
{
    size_t i_hex = 0;
    // std::reverse(hex.begin(), hex.end());
    for (int i = 0; i < len; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            H[i] |= hexCharToNumber(hex[i_hex + j]) << j * 4;
        }
        i_hex += 4;
    }
}

void frodo_uint8(uint8_t* H, size_t len, std::string hex)
{
    size_t i_hex = 0;
    // std::reverse(hex.begin(), hex.end());
    for (int i = 0; i < len; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            H[i] |= hexCharToNumber(hex[i_hex + j]) << j * 4;
        }
        i_hex += 2;
    }
}


std::string frodo_hex(uint16_t* H, size_t len)
{
    std::string result = "";
    uint16_t mask = (1 << 4) - 1;
    uint16_t shift = 0;
    char numHex[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                       '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < len; ++i)
    {
        for (int j = 0; j < 16; j += 4)
        {
            uint16_t debug = H[i] >> j;
            result =result +  numHex[(H[i] >> j) & mask] ;
        }
    }
    return result;
}

std::string frodo_hex(uint8_t* H, size_t len)
{
    std::string result = "";
    uint16_t mask = (1 << 4) - 1;
    uint16_t shift = 0;
    char numHex[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                       '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < len; ++i)
    {
        for (int j = 0; j < 8; j += 4)
        {
            uint16_t debug = H[i] >> j;
            result = result +  numHex[(H[i] >> j) & mask];
        }
    }
    return result;
}

void matrix_add(const uint16_t* m1, const uint16_t* m2, uint16_t* res,
                size_t size)
{
    uint32_t c = 0;
    for (int i = 0; i < size; i++)
    {
        c = m1[i] + m2[i];
        res[i] = c & modQ;
    }
}

void matrix_mul(const uint16_t* m1, const uint16_t* m2, uint16_t* res,
                size_t row1, size_t row2, size_t column1, size_t column2)
{
    if (row2 != column1)
    {
        std::cerr << "Matrix dimensions are not compatible for multiplication!"
                  << std::endl;
        return;
    }

    for (size_t i = 0; i < row1; ++i)
    {
        for (size_t j = 0; j < column2; ++j)
        {
            res[i * column2 + j] = 0;
        }
    }

    for (size_t i = 0; i < row1; ++i)
    {
        for (size_t j = 0; j < column2; ++j)
        {
            for (size_t k = 0; k < row2; ++k)
            {
                res[i * column2 + j] = uint16_t(res[i * column2 + j] +
                                           m1[i * row2 + k] * m2[k * column2 + j]) &
                                  modQ;
            }
        }
    }
}

void matrix_sub(const uint16_t* m1, const uint16_t* m2, uint16_t* res,
                size_t size)
{
    uint32_t c = 0;
    for (int i = 0; i < size; i++)
    {
        c = q + m1[i] - m2[i];
        res[i] = c & modQ;
    }
}

void matrix_transpose(const uint16_t* in, uint16_t* out, const int row, const int column)
{
    for(int i = 0; i < row; ++i)
    {
        for(int j = 0; j < column; ++j)
        {
            out[j * row + i] = in[i * column + j];
        }
    }
}

void swap_bytes(uint16_t &a)
{
    a = (a & 0xFF << 8) || (a >> 8);
}

