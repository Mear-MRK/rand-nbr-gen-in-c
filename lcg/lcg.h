#ifndef LCG_H
#define LCG_H 1

/* Largest number that lcg_uint16 can generate: 65535 = 2^16 - 1 */
#define LCG_UINT16_MAX 0xFFFF

/* Minimum distance between two randomly generated float32 by lcg_flt: 2^-24 */
#define LCG_FLT_EPS 5.960464478E-8F


#ifdef __cplusplus
extern "C"
{
#endif

/* Changes the internal states */
void lcg_seed(uint64_t);

/* Gives the internal state of lcg_uint16 */
uint32_t lcg_uint16_intern_stat(void);

/* Gives the internal state of lcg_uint32 */
uint64_t lcg_uint32_intern_stat(void);

/* Generates a random 16-bit unsigned integer (up to 65535) */
uint32_t lcg_uint16(void);

/* Generates a random 32-bit unsigned integer */
uint32_t lcg_uint32(void);

/* Generates uniformly distributed random float32 f such that 0 <= f < 1 */
float lcg_flt(void);


#ifdef __cplusplus
}
#endif

#endif /* LCG_H */
