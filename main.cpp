#include "FrodoSupport.h"
#include "parameters.h"
#include <cstdint>
#include <iostream>
#include <random>

int generateRandomNumber(int bit)
{
	const int min = 0;
	const int max = (1 << bit); // 2^15

	std::random_device rd; // Seed generator
	std::mt19937 gen(rd()); // Mersenne Twister generator
	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}


int main(int argc, char *argv[])
{
	int matrix_len = 8 * 8;
	int b_len = matrix_len * B / 16;
	uint16_t H[matrix_len];
	uint16_t K[matrix_len];
	uint16_t b[b_len], b_e[b_len];
	for (int i = 0; i < matrix_len; ++i)
	{
		// H[i] = ;
		H[i] = 0;
		K[i] = 0;
	}
	for (int i = 0; i < b_len; ++i)
	{
		b[i] = generateRandomNumber(16);
		b_e[i] = 0;
	}

// 	 // std::cout<<frodo_hex(b,b_len)<<"\n";
// #ifdef FRODO
// 	std::string hex_b = "C1E291EAB2B411FA9F5140FDDFB31F044E528B76216A6F2E";
// #else
// 	std::string hex_b = "C1E291EAB2B411FA9F5140FDDFB31F044E528B76216A6F2E";
// #endif
//
	// frodo_uint16(b, b_len, hex_b);
	std::cout << frodo_hex(b, b_len) << "\n";
	frodo_encode(K, b);
	std::cout << "K_hex :" << frodo_hex(K, matrix_len) << "\n";
	frodo_decode(K, b_e);

	std::cout <<"b_e_hex: "<< frodo_hex(b, b_len) << "\n";
	std::cout << "b_e_hex: " << frodo_hex(b_e, b_len) << "\n";


	// // std::string hex_H = "651610C02D3941111EEF61EF63DE5374";
	// // std::cout<<"H_hex: "<< frodo_hex(H,matrix_len)<<"\n";
	// // frodo_uint16(H,matrix_len,hex_H);
	// std::cout<<"H_test:"<<frodo_hex(H,matrix_len)<<"\n";
	//
	// frodo_pack(H,b,2,4);
	// std::cout<<"b_test:"<<frodo_hex(b,b_len)<<"\n";
	// frodo_unpack(K,b,2,4);
	// std::cout<<"K_test:"<<frodo_hex(K,matrix_len)<<"\n";
	return 0;
}
