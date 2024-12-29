#include <cmath>

#ifndef parameters
#define parameters

// #define FRODO // uncomment if you want Frodo-640, comment if you want Frodo-976
#ifdef FRODO
#define N 640
#define D 15
#define q (1 << D)
#define modQ (q - 1)
#define B 2
#define nm 8
#define MATRIX_LEN 64
#define B_IN_16 int(ceil(16.0 / B))
#else
#define N 976
#define D 16
#define q (1 << D)
#define modQ (q - 1)
#define B 3
#define nm 8
#define MATRIX_LEN 64
#define B_IN_16 int(ceil(16.0 / B))
#endif


#endif
