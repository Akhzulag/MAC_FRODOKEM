#include "FrodoRandom.h"
#include "SHAKE.h"
#include "parameters.h"
#include <cstdint>

uint16_t frodo_sample(uint16_t r)
{
    uint16_t t = r >> 1;
    uint16_t e = 0;
    for (int z = 0; z < support_s; z++)
    {
        if (t > T_x[z])
        {
            e++;
        }
    }
    if (r & 1)
    {
        e = q - e;
    }
    return e;
}

void frodo_sampleMatrix(uint16_t* E, uint16_t* r, size_t n_size)
{
    for (int i = 0; i < n_size; i++)
    {
        E[i] = frodo_sample(r[i]);
    }
}

void frodo_sampleMatrix(uint16_t* E, uint8_t* r, size_t r_size)
{
    int j = 0;
    for (int i = 0; i < r_size; i++)
    {
        E[i] = frodo_sample(r[j] | (r[j + 1] << 8)) & modQ;
        j += 2;
    }
}

void frodo_gen(uint16_t* A, uint8_t* seed_A, size_t seed_byte)
{
    size_t k = seed_byte + 2;
    uint8_t mask = (1 << 8) - 1;
    uint8_t b[k];
    for (int j = 2; j < k; j++)
    {
        b[j] = seed_A[j - 2];
    }
    for (uint16_t i = 0; i < N; i++)
    {

        // write <i> in little endian byte order
        b[0] = (i >> 8) & mask;
        b[1] = i & mask;


        uint8_t tmp[2 * N];
        shake(b, k , tmp, 2 * N, 128);
        uint16_t tmp1[N];
        int ty = 0;

        for (int tk = 0; tk < 2 * N; tk += 2)
        {
            tmp1[ty] = ((tmp[tk] << 8) | tmp[tk + 1]) & modQ;
            ty+=1;
        }

        for (int j = 0; j < N; j++)
        {
            A[i * N + j] = tmp1[j];
        }
    }
}

#include <random>

int generateRandomNumber(int bit)
{
    const int min = 0;
    const int max = (1 << bit) - 1; // 2^15

    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister generator
    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

void generateBitString(uint8_t* bitString, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        bitString[i] = generateRandomNumber(8);
    }
}
void generateBitString(uint16_t* bitString, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        bitString[i] = generateRandomNumber(16);
    }
}

