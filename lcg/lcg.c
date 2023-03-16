#include <stdint.h>
#include <assert.h>
#include "lcg.h"

static uint32_t intern_stat_0 = 5U;
static uint32_t intern_stat_1 = 13U;

/* Gives the internal state of lcg_rand */
uint32_t lcg_rand_intern_stat(void) 
{
    return intern_stat_0;
}

/* Seeds random number generator */
void lcg_seed(uint32_t s)
{
    intern_stat_0 = (5U + s) & 0x7FFFFFFFU;
    intern_stat_1 = (13U + s) & 0x7FFFFFFFU;
}

/* Gives 15bit random unsigned integer i such that 0 <= i < 2^15 */
inline uint32_t lcg_rand(void)
{
    intern_stat_0 = (214013U * intern_stat_0 + 2531011U) & 0x7FFFFFFFU;
    return intern_stat_0 >> 16;
}

/* Gives (32bit) uint32_t random integer i: 0 <= i < 2^32 */
inline uint32_t lcg_uint32(void)
{
    intern_stat_0 = (214013U * intern_stat_0 + 2531011U) & 0x7FFFFFFFU;
    intern_stat_1 = (214013U * intern_stat_1 + 2531011U) & 0x7FFFFFFFU;
    return intern_stat_0 >> 15 | intern_stat_1 >> 15 << 16;
}

/* 
 * Gives uniformly distributed random float32 (single) f such that 0.0F <= f < 1.0F.
 * It can generate 2^24 possible values; This number has been chosen intentionally because
 * minimum possible difference between two float32 in the range of [0.5, 1] is 2^-24.
 */
inline float lcg_flt(void)
{
    intern_stat_0 = (214013U * intern_stat_0 + 2531011U) & 0x7FFFFFFFU;
    intern_stat_1 = (214013U * intern_stat_1 + 2531011U) & 0x7FFFFFFFU;
    return (intern_stat_0 >> 19 | intern_stat_1 >> 19 << 12) * LCG_FLT_EPS;
}

