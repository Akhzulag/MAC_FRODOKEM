#include <cstdint>
#include <iostream>

#ifndef FrodoSupport
#define FrodoSupport

void frodo_encode(uint8_t* K, uint16_t* k);

void frodo_decode(uint8_t* K, uint16_t* k);

void frodo_pack();

void frodo_unpack();

#endif
