#ifndef FrodoSupport
#define FrodoSupport
#include <cstddef>
#include <cstdint>
#include <iostream>
void frodo_encode(uint16_t* K, uint16_t* k);

void frodo_decode(uint16_t* K, uint16_t* k);

void frodo_pack(uint16_t* C, uint8_t* b, uint16_t n1, uint16_t n2);

void frodo_unpack(uint16_t* C, uint8_t* b, uint16_t n1, uint16_t n2);

std::string frodo_hex(uint16_t* H, size_t len);

void frodo_uint8(uint8_t* H, size_t len, std::string hex);
void frodo_uint16(uint16_t* H, size_t len, std::string hex);

std::string frodo_hex(uint8_t* H, size_t len);

void matrix_add(const uint16_t *m1, const uint16_t* m2, uint16_t* res, size_t size);

void matrix_mul(const uint16_t *m1, const uint16_t* m2, uint16_t* res, size_t row1, size_t row2, size_t column1, size_t column2);

void matrix_sub(const uint16_t *m1, const uint16_t* m2, uint16_t* res, size_t size);

void matrix_transpose(const uint16_t* in, uint16_t* out, int row, int column);

void swap_bytes(uint16_t& a);


#endif
