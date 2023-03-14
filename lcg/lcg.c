#include <stdint.h>
#include "lcg.h"

static uint32_t g_stat_15 = 7U;
static uint32_t g_stat_9 = 13U;

/* Gives the internal state of lcg_rand */
uint32_t lcg_intern_stat(void) {
    return g_stat_15;
}

/* Seeds random number generators */
void lcg_seed(uint32_t s)
{
    g_stat_15 = 7U + s;
    g_stat_9 = 13U + s;
}

/* Gives 15bit random unsigned integer i such that 0 <= i < 2^15 */
uint32_t lcg_rand(void)
{
    g_stat_15 = (214013U * g_stat_15 + 2531011U) & 0x7FFFFFFFU;
    return g_stat_15 >> 16;
}

uint32_t lcg_rand_nbit(unsigned nbr_outbits)
{
    g_stat_15 = (214013U * g_stat_15 + 2531011U) & 0x7FFFFFFFU;
    return g_stat_15 >> (31-nbr_outbits);
}

/* Gives 9bit random unsigned integer i such that 0 <= i < 2^9 */
static uint32_t lcg_rand_9(void)
{
    g_stat_9 = (214013U * g_stat_9 + 2531011U) & 0x7FFFFFFFU;
    return g_stat_9 >> 22;
}

/* 
 * Gives uniformly distributed random float32 f such that 0.0F <= f < 1.0F.
 * It can generate 2^24 possible values; This number has been chosen intentionally because
 * minimum possible difference between two float32 in the range of [0.5, 1] is 2^-24.
 */
float lcg_flt(void)
{
    return (lcg_rand_9() << 25 | lcg_rand()) * LCG_FLT_EPS;
}
