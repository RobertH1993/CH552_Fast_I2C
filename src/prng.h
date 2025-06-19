#ifndef _PRNG
#define _PRNG

#include <stdint.h>

void prng_seed(uint8_t seed);
uint8_t prng_next();
uint8_t prng_max(uint8_t max);


#endif