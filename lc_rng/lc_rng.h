#ifndef LC_RNG_H
#define LC_RNG_H 1

/* Largest number that lc_rng_uint can generate: 32767 = 2^15 - 1 */
#define LC_RNG_UINT_MAX 0x7FFF

/* Minimum distance between two randomly generated float32 by lc_rng_flt */
#define LC_RNG_FLT_EPS 5.960464478E-8F

#ifdef __cplusplus
extern "C"
{
#endif

/* Changes the internal states */
void lc_rng_seed(uint32_t);

/* Gives the internal state of lc_rng_uint */
uint32_t lc_rng_intern_stat(void);

/* Generates a random unsigned int32 up to 32767 */
uint32_t lc_rng_uint(void);

/* Generates a random float32 in [0, 1) */
float lc_rng_flt(void);

#ifdef __cplusplus
}
#endif

#endif /* LC_RNG_H */
