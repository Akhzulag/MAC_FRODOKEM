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
#define lenk 128
#define lenChi 16
#define support_s 11
inline int T_x[12] = { 9456, 8857, 7280, 5249, 3321, 1844, 898, 384, 144, 47, 13, 3 };

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
inline int T_x[11] = { 11278, 10277, 7774, 4882, 2545, 1101, 396, 118, 29, 6, 1 };
#endif



#endif
