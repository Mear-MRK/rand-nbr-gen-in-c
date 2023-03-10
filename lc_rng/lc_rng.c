#include <stdint.h>
#include "lc_rng.h"

static uint32_t g_stat_15 = 7;
static uint32_t g_stat_9 = 13;

/* Gives the internal state of lc_rng_uint */
uint32_t lc_rng_intern_stat(void) {
    return g_stat_15;
}

/* Seeds random number generators */
void lc_rng_seed(uint32_t s)
{
    g_stat_15 = 7 + s;
    g_stat_9 = 13 + s;
}

/* Gives 15bit random unsigned integer i such that 0 <= i < 2^15 */
uint32_t lc_rng_uint(void)
{
    g_stat_15 = (214013 * g_stat_15 + 2531011) & 0x7FFFFFFF;
    return g_stat_15 >> 16;
}

/* Gives 9bit random unsigned integer i : 0 <= i < 2^9 */
static uint32_t lc_rng_9bit_uint(void)
{
    g_stat_9 = (214013 * g_stat_9 + 2531011) & 0x7FFFFFFF;
    return g_stat_9 >> 22;
}

/* 
 * Gives uniformly distributed random float32 f such that 0.0F <= f < 1.0F.
 * It can generate 2^24 possible values; This number has been chosen carefully because
 * the largest float32 smaller that 1 is 1 - 2^(-24).
 */
float lc_rng_flt(void)
{
    return (lc_rng_uint() << 9 | lc_rng_9bit_uint()) * LC_RNG_FLT_EPS;
}
