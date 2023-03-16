#ifndef PCG_H
#define PCG_H 1
#include <stdint.h>

#define PCG_UINT_MAX UINT32_MAX
#define PCG_DBL_EPS 1.11022302462515654E-16  /* 2^-53 */

#ifdef __cplusplus
extern "C"
{
#endif

    void pcg_seed(uint64_t);
    uint32_t pcg_uint32(void);
    double pcg_dbl(void);

#ifdef __cplusplus
}
#endif

#endif /* PCG_H */
