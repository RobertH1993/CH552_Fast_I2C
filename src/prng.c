#include "prng.h"

static uint8_t rng_state = 0xA5;

void prng_seed(uint8_t seed){
    rng_state = seed;
}

inline uint8_t prng_next(){
    rng_state ^= rng_state << 3;
    rng_state ^= rng_state >> 5;
    rng_state ^= rng_state << 1;
    return rng_state;
}

uint8_t prng_max(uint8_t max){
    return prng_next() % max;
}