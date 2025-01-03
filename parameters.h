#include <cmath>

#ifndef parameters
#define parameters

#define FRODO // uncomment if you want Frodo-640, comment if you want Frodo-976
#ifdef FRODO
#define N 640
#define D 15
#define q (1 << D)
#define modQ (q - 1)
#define B 2
#define nm 8
#define MATRIX_LEN 64
#define B_IN_16 int(ceil(16.0 / B))
#define len_A (N * N)
#define len_E_S (N * nm)
#define len_seedSE (128 / 8)
#define len_sec (128 / 8)
#define len_Z (128 / 8)
#define len_seedA (128 / 8)
#define lenk 128
#define lenChi 16
#define support_s 12
inline int T_x[13] = {9288, 8720, 7216, 5264, 3384, 1918,
                      958,  422,  164,  56,   17,   4, 1};

#else
#define N 976
#define D 16
#define q (1 << D)
#define modQ (q - 1)
#define B 3
#define nm 8
#define MATRIX_LEN 64
#define B_IN_16 int(ceil(16.0 / B))
#define lenk 192
#define lenChi 16
#define support_s 10
inline int T_x[11] = {11278, 10277, 7774, 4882, 2545, 1101, 396, 118, 29, 6, 1};
#endif

#endif
