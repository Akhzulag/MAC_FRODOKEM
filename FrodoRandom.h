#ifndef FRODO_RANDOM_H
#define FRODO_RANDOM_H

#include <cmath>
#include <cstdint>
#include <iostream>

uint16_t frodo_sample(uint16_t r);

void frodo_sampleMatrix(int16_t* E, int16_t* r, size_t n_size);

void frodo_sampleMatrix(uint16_t* E, uint8_t* r, size_t r_size);


void frodo_gen(uint16_t* A, uint8_t* seed_A, size_t seed_bit);

int generateRandomNumber(int bit);

void generateBitString(uint8_t* bitString, size_t len);

void generateBitString(uint16_t* bitString, size_t len);


#endif // FRODO_RANDOM_H
