//
// Created by akh on 31/12/2024.
//

#ifndef FRODOPKE_H
#define FRODOPKE_H

#include <cstdint>
#include <iostream>

void frodo_KeyGen(uint16_t* pk, uint16_t* sk);
void frodo_enc(uint16_t* m, uint16_t* pk, uint16_t* c);
void frodo_dec(uint16_t* c, uint16_t* sk, uint16_t* m);


#endif //FRODOPKE_H
