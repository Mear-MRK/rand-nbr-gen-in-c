#include <stdint.h>
#include <assert.h>
#include "lcg.h"

static uint32_t intern_stat_16 = 5;
static uint64_t intern_stat_32 = 13;


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
    intern_stat_16 = (uint32_t)(5 + s);
    intern_stat_32 = 13 + s;
}

/* Gives 16-bit random unsigned integer i such that 0 <= i < 2^16 */
inline uint32_t lcg_uint16(void)
{
    intern_stat_16 = 32310901 * intern_stat_16 + 17;
    return intern_stat_16 >> 16;
}


/* Gives 32-bit random unsigned integer i: 0 <= i < 2^32 */
inline uint32_t lcg_uint32(void) {
    intern_stat_32 = 3935559000370003845ull * intern_stat_32 + 13;
    return (uint32_t)(intern_stat_32 >> 32);
}


/* 
 * Gives uniformly distributed random float32 (single) f such that 0.0F <= f < 1.0F.
 * It can generate 2^24 possible values; This number has been chosen intentionally because
 * minimum possible difference between two float32 in the range of [0.5, 1] is 2^-24.
 */
inline float lcg_flt(void)
{
    intern_stat_32 = 3935559000370003845ull * intern_stat_32 + 13;
    return (intern_stat_32 >> 40) * LCG_FLT_EPS;
}
