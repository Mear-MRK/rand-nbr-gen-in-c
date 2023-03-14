#include "pc_rng.h"
#include <stdint.h>

static uint64_t g_state = 0x4d595df4d0f33173;         // Or 0x853c49e6748fea9bULL
static uint64_t const g_mult = 6364136223846793005ull;
static uint64_t const g_inc = 1442695040888963407ull; // Or an arbitrary odd constant - 0xda3e39cb94b95bdbULL

static inline uint32_t g_rotr32(uint32_t x, unsigned r)
{
    return x >> r | x << (-r & 31);
}

uint32_t pc_rng_uint(void)
{
    uint64_t x = g_state;
    unsigned count = (unsigned)(x >> 59); // 59 = 64 - 5

    g_state = x * g_mult + g_inc;
    x ^= x >> 18;                                  // 18 = (64 - 27)/2
    return g_rotr32((uint32_t)(x >> 27), count); // 27 = 32 - 5
}

void pc_rng_seed(uint64_t seed)
{
    g_state = seed + g_inc;
    (void)pc_rng_uint();
}

double pc_rng_dbl(void)
{
    return (double)pc_rng_uint() / (PC_RNG_UINT_MAX + 1.0);
}
