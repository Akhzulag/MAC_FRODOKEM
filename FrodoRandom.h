#ifndef FRODO_RANDOM_H
#define FRODO_RANDOM_H

#include <iostream>
#include <cmath>
#include <cstdint>

int frodo_sample(uint16_t r);

void frodo_sampleMatrix(int16_t *E, uint16_t* seed_E, int n1, int n2, int c);


#endif //FRODO_RANDOM_H