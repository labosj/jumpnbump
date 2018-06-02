//
// Created by edwin on 02-06-18.
//

#include "globals.h"
#include "util.h"

unsigned short rnd(unsigned short max) {
#if (RAND_MAX < 0x7fff)
#error "rand returns too small values"
#elif (RAND_MAX == 0x7fff)
    return (unsigned short)((rand()*2) % (int)max);
#else
    return (unsigned short) (rand() % (int) max);
#endif
}