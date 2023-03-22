#include "pcg.h"
#include <stdint.h>

#define MULT 6364136223846793005ull
#define INC 3210482851ull
#define INIT_INTERN_STAT 0x4d595df4d0f33173ull

static uint64_t intern_stat = INIT_INTERN_STAT;

/* Seeds random number generator */
void pcg_seed(uint64_t s)
{
    intern_stat = INIT_INTERN_STAT ^ s;
    intern_stat = MULT * intern_stat + INC;
}

/* Gives the internal state of pcg */
uint64_t pcg_intern_stat(void) {
    return intern_stat;
}

/* Gives 32-bit random unsigned integer i: 0 <= i < 2^32 */
uint32_t pcg_uint32(void)
{
    uint64_t x = intern_stat;
    uint32_t y;
    unsigned count;

    intern_stat = MULT * intern_stat + INC;

    count = (unsigned)(x >> 59);                // 59 = 64 - 5
    x ^= x >> 18;                               // 18 = (64 - 27)/2
    y = (uint32_t)(x >> 27);                    // 27 = 32 - 5
    return y >> count | y << (-count & 31);     // -count & 31 -> 32 - count
}

/* 
 * Gives uniformly distributed random float64 (double) f such that 0.0 <= f < 1.0.
 * It can generate 2^53 possible values; This number has been chosen intentionally because
 * minimum possible difference between two float64 in the range of [0.5, 1] is 2^-53.
 */
double pcg_dbl(void)
{
    uint64_t x = intern_stat;
    unsigned count;

    intern_stat = MULT * intern_stat + INC;
    
    count = (unsigned)(x >> 59);
    x ^= x >> (5 + count);
    x *= 12605985483714917081ull;
    
    return ((x ^ (x >> 32)) >> 11) * PCG_DBL_EPS;   // 11 = 64 - 53
}
