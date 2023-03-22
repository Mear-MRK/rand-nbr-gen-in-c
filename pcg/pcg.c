#include "pcg.h"
#include <stdint.h>

#define INIT_INTERN_STAT 0x4d595df4d0f33173ull

static uint64_t intern_stat = INIT_INTERN_STAT;

/* Seeds random number generator */
void pcg_seed(uint64_t s)
{
    intern_stat = INIT_INTERN_STAT ^ s;
}

/* Gives the internal state of pcg */
uint64_t pcg_intern_stat(void) {
    return intern_stat;
}

static inline uint32_t rotr32(uint32_t x, unsigned r)
{
    return x >> r | x << (-r & 31);
}

/* Gives 32-bit random unsigned integer i: 0 <= i < 2^32 */
inline uint32_t pcg_uint32(void)
{
    uint64_t x = intern_stat;
    unsigned count = (unsigned)(x >> 59); // 59 = 64 - 5

    intern_stat = 3935559000370003845ull * intern_stat + 3210482851ull;
    
    x ^= x >> 18;                              // 18 = (64 - 27)/2
    return rotr32((uint32_t)(x >> 27), count); // 27 = 32 - 5
}

/* 
 * Gives uniformly distributed random float64 (double) f such that 0.0 <= f < 1.0.
 * It can generate 2^53 possible values; This number has been chosen intentionally because
 * minimum possible difference between two float64 in the range of [0.5, 1] is 2^-53.
 */
inline double pcg_dbl(void)
{
    return (pcg_uint32() ^ ((uint64_t)pcg_uint32()) << 21) * PCG_DBL_EPS;
}
