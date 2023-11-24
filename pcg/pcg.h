#ifndef PCG_H
#define PCG_H 1

#include <stdint.h>

/* Largest number that pcg_uint32 can generate: 2^32 - 1 */
#define PCG_UINT32_MAX UINT32_MAX

/* Minimum distance between two randomly generated float64 by pcg_dbl: 2^-53 */
#define PCG_DBL_EPS 1.11022302462515654E-16

/* Minimum distance between two randomly generated float32 by pcg_flt: 2^-24 */
#define PCG_FLT_EPS 5.960464478E-8F

#ifdef __cplusplus
extern "C"
{
#endif

/* Changes the internal states */
void pcg_seed(uint64_t);

/* Gives the internal state of pcg */
uint64_t pcg_intern_stat(void);

/* Generates a random 32-bit unsigned integer */
uint32_t pcg_uint32(void);

/* Generates uniformly distributed random float64 f such that 0 <= f < 1 */
double pcg_dbl(void);

/* Generates uniformly distributed random float32 f such that 0 <= f < 1 */
float pcg_flt(void);

#ifdef __cplusplus
}
#endif

#endif /* PCG_H */
