#include <cstddef>
#include <cstdint>
#include <iostream>

#ifndef FrodoSupport
#define FrodoSupport

void frodo_encode(uint16_t* K, uint16_t* k);

void frodo_decode(uint16_t* K, uint16_t* k);

void frodo_pack(uint16_t* C, uint8_t* b, uint16_t n1, uint16_t n2);

void frodo_unpack(uint16_t* C, uint8_t* b, uint16_t n1, uint16_t n2);

std::string frodo_hex(uint16_t* H, size_t len);

std::string frodo_uint8(uint8_t* H, size_t len, std::string hex);
void frodo_uint16(uint16_t* H, size_t len, std::string hex);


std::string frodo_hex(uint8_t* H, size_t len);

#endif
