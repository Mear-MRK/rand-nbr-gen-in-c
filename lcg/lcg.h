#ifndef LCG_H
#define LCG_H 1

/* Largest number that lcg_rand can generate: 32767 = 2^15 - 1 */
#define LCG_RAND_MAX 0x7FFF

/* Minimum distance between two randomly generated float32 by lcg_flt: 2^-24 */
#define LCG_FLT_EPS 5.960464478E-8F

#ifdef __cplusplus
extern "C"
{
#endif

/* Changes the internal states */
void lcg_seed(uint32_t);

/* Gives the internal state of lcg_rand */
uint32_t lcg_intern_stat(void);

/* Generates a random unsigned int32 up to 32767 */
uint32_t lcg_rand(void);

/* Generates uniformly distributed random float32 f such that 0 <= f < 1 */
float lcg_flt(void);

uint32_t lcg_rand_nbit(unsigned);

#ifdef __cplusplus
}
#endif

#endif /* LCG_H */
