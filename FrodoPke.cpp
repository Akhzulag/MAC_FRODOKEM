//
// Created by akh on 31/12/2024.
//

#include "FrodoPke.h"
#include "FrodoRandom.h"
#include "FrodoSupport.h"
#include "parameters.h"
#include "SHAKE.h"

void frodo_KeyGen(uint16_t* pk, uint16_t* sk)
{
    std::cout << "KeyGen start" << std::endl;
    uint16_t A[len_A];
    uint16_t S[len_E_S];
    uint16_t E[len_E_S];
    uint16_t R[len_E_S];
    uint8_t seed_SE[len_seedSE];
    uint8_t seed_A[len_seedA];

    generateBitString(seed_SE, len_seedSE);
    generateBitString(seed_A, len_seedA);

    std::cout << "seed_SE:" << frodo_hex(seed_SE, len_seedSE) << std::endl;
//debug
    for(int i = 0; i < len_seedA; i++)
    {
        seed_A[i] = 0;
        seed_SE[i] = 0;
    }

    // frodo_uint8(seed_SE,len_seedSE,"747C6BB4055108ECCA06C7762881FED8");
    frodo_uint8(seed_SE,len_seedSE,"61983BD2A692C1C67976E4994F7CEE7A");
    std::cout << "seed_SE:" << frodo_hex(seed_SE, len_seedSE) << std::endl;


    frodo_uint8(seed_A,len_seedA,"6DC77917794FB8E920915880A6EF97ED");
    std::cout << "seed_A:" << frodo_hex(seed_A, len_seedA) << std::endl;
    frodo_gen(A, seed_A, len_seedA);

    uint8_t input_r[len_seedSE + 1];

    std::cout<<"A: ";
    for(int i = 0; i < 10000;++i)
    {
        printf("%02x", A[i]);
    }
    std::cout<<"\n";
//end debug

    input_r[0] = 0x5F;
    for (int i = 1; i < len_seedSE + 1; ++i)
    {
        input_r[i] = seed_SE[i - 1];
    }
    uint8_t r[((2 * nm * N) * lenChi) / 8];
    std::cout<<"sizeof:" <<sizeof(input_r) - 1 << " "<<len_seedSE + 1 <<"\n";
    shake(input_r, len_seedSE + 1, r, ((2 * nm * N) * lenChi) / 8,
          128); // think about lenCHI

    frodo_sampleMatrix(sk, r, N * nm);

    frodo_sampleMatrix(E, r + 2*N * nm, N * nm);

    matrix_transpose(sk, S, nm, N);
    matrix_mul(A, S, R, N, N, N, nm);
    matrix_add(R, E, R, len_E_S);

    std::cout << "B: " << frodo_hex(R, N * nm) << "\n";

    int j = 0;
    for (size_t i = 0; i < len_seedA / 2; i++)
    {
        pk[i] = (seed_A[j] | seed_A[j + 1] << 8);
        j += 2;
    }
    for (int i = len_seedA / 2; i < len_seedA / 2 + len_E_S; ++i)
    {
        pk[i] = R[i - len_seedA / 2];
    }
}

void frodo_enc(uint16_t* m, uint16_t* pk, uint16_t* c)
{
    uint16_t A[len_A];
    uint16_t S1[len_E_S];
    uint16_t E1[len_E_S];
    uint16_t E2[len_E_S];
    uint16_t* R = (pk + len_seedA / 2);
    uint16_t B1[len_E_S];
    uint16_t V[MATRIX_LEN];
    uint8_t seed_SE[len_seedSE];

    uint8_t seed_A[len_seedA];
    int j = 0;
    for (int i = 0; i < len_seedA; i += 2)
    {
        seed_A[i] = pk[j] & 0xFF;
        seed_A[i + 1] = pk[j] >> 8;
        ++j;
    }
    std::cout << "j:" << j << "\n";
    std::cout << "Encode:\n";
    std::cout << "seed_A:" << frodo_hex(seed_A, len_seedA) << std::endl;
    std::cout << "B: " << frodo_hex(R, N * nm) << "\n";
    uint8_t input_r[len_seedSE + 1];
    uint16_t K[MATRIX_LEN];
    for(int i = 0 ; i < MATRIX_LEN; ++i)
    {
        K[i] = 0;
    }
    generateBitString(seed_SE, len_seedSE);
    for(int i = 0; i < len_seedSE; i++)
    {
        seed_SE[i] = 0;
    }

    frodo_uint8(seed_SE,len_seedSE,"C8867840D0A62825E3511078BACDBE41");
    std::cout << "seed_SE:" << frodo_hex(seed_SE, len_seedSE) << std::endl;

    frodo_gen(A, seed_A, len_seedA);
    std::cout<<"A: ";
    for(int i = 0; i < 10000;++i)
    {
        printf("%02x", A[i]);
    }
    std::cout<<"\n";

    input_r[0] = 0x96;
    for (int i = 1; i < len_seedSE + 1; ++i)
    {
        input_r[i] = seed_SE[i - 1];
    }
    uint8_t r[((2 * nm * N + nm * nm) * lenChi) / 8];

    shake(input_r, len_seedSE + 1  , r, ((2 * nm * N + nm * nm) * lenChi) / 8,
          128); // think about
    std::cout<<"SE:"<<frodo_hex(r,((2 * nm * N + nm * nm) * lenChi) / 8 ) << std::endl;

    // uint16_t r[((2 * nm * N + nm * nm) * lenChi)];
    //
    // j = 0;
    // for(int i = 0; i < ((2 * nm * N + nm * nm) * lenChi); ++i)
    // {
    //     r[i] = (r[j + 1 ] << 8 | r_t[j]);
    //     j+=2;
    // }

    frodo_sampleMatrix(S1, r, N * nm);


    frodo_sampleMatrix(E1, r + 2 * N * nm, N * nm);


    frodo_sampleMatrix(E2, r + 4 * nm * N, nm * nm);


    matrix_mul(S1, A, B1, nm, N, N, N);
    matrix_add(B1, E1, B1, nm * N);

    matrix_mul(S1, R, V, nm, N, N, nm);
    matrix_add(V, E2, V, nm * nm);


    std::cout<<"B1: "<<frodo_hex(B1, N * nm) << "\n";
    std::cout<<"V: "<<frodo_hex(V,nm*nm)<<"\n";

    for (int i = 0; i < len_E_S; ++i)
    {
        c[i] = B1[i];
    }

    frodo_encode(K, m);
    matrix_add(K, V, V, MATRIX_LEN);
    for (int i = len_E_S; i < len_E_S + MATRIX_LEN; ++i)
    {
        c[i] = V[i - len_E_S];
    }
}

void frodo_dec(uint16_t* c, uint16_t* sk, uint16_t* m)
{
    int size_c1 = nm * N;
    int size_c2 = nm * nm;
    uint16_t S[nm * N];
    uint16_t M[nm * nm];
    uint16_t tmp[nm * nm];
    matrix_transpose(sk, S, nm, N);
    matrix_mul(c, S, tmp, nm, N, N, nm);
    matrix_sub((c + size_c1), tmp, M, size_c2);
    std::cout<<"M: "<<frodo_hex(M,nm*nm) << std::endl;
    frodo_decode(M, m);
}
