//
// Created by akh on 30/12/2024.
//



#ifndef SHAKE_H
#define SHAKE_H

#include <cstdint>
#include <string>

// std::string shake(const uint8_t* input, size_t input_len, int shake_bit,
// 					   size_t output_byte);


// uint8_t* shake( uint8_t* input, size_t input_len, int shake_bit, size_t output_byte);

int shake(const uint8_t* in, size_t in_size, uint8_t* out, size_t outbytes_size, int shake_type);

#endif //SHAKE_H
