#include "common.h"

int randomSeed = 1;

void Srand(int seed) {
    randomSeed = seed;
}

int Rand(unsigned v) {
    randomSeed = randomSeed * 0x41c64e6d + 12345;
    return v * ((u_short)randomSeed) >> 0x10;
}
