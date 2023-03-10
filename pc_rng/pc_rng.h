#ifndef PC_RNG_H
#define PC_RNG_H 1
#include <stdint.h>

#define PC_RNG_UINT_MAX UINT32_MAX

#ifdef __cplusplus
extern "C"
{
#endif

    void pc_rng_seed(uint64_t);
    uint32_t pc_rng_uint(void);
    double pc_rng_dbl(void);

#ifdef __cplusplus
}
#endif

#endif /* PC_RNG_H */
