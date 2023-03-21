#include <stdint.h>
#include <assert.h>
#include "lcg.h"

#define INIT_INTERN_STAT_16 931126291u
#define INIT_INTERN_STAT_32 3771874948816592237ull

static uint32_t intern_stat_16 = INIT_INTERN_STAT_16;
static uint64_t intern_stat_32 = INIT_INTERN_STAT_32;


/* Gives the internal state of lcg_uint16 */
uint32_t lcg_uint16_intern_stat(void) 
{
    return intern_stat_16;
}

/* Gives the internal state of lcg_uint32 */
uint64_t lcg_uint32_intern_stat(void) 
{
    return intern_stat_32;
}

/* Seeds random number generator */
void lcg_seed(uint64_t s)
{
    intern_stat_16 = INIT_INTERN_STAT_16 ^ (uint32_t)s ^ (uint32_t)(s >> 16);
    intern_stat_32 = INIT_INTERN_STAT_32 ^ s;
}

/* Gives 16-bit random unsigned integer i such that 0 <= i < 2^16 */
inline uint32_t lcg_uint16(void)
{
    intern_stat_16 = 32310901u * intern_stat_16 + 425894797u;
    return intern_stat_16 >> 16;
}


/* Gives 32-bit random unsigned integer i: 0 <= i < 2^32 */
inline uint32_t lcg_uint32(void) {
    intern_stat_32 = 3935559000370003845ull * intern_stat_32 + 3210482851ull;
    return (uint32_t)(intern_stat_32 >> 32);
}


/* 
 * Gives uniformly distributed random float32 (single) f such that 0.0F <= f < 1.0F.
 * It can generate 2^24 possible values; This number has been chosen intentionally because
 * minimum possible difference between two float32 in the range of [0.5, 1] is 2^-24.
 */
inline float lcg_flt(void)
{
    return (lcg_uint32() >> 8) * LCG_FLT_EPS;
}
