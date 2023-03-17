#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "lcg.h"

    
void test_lcg_rand(size_t size) { 
    uint32_t r;
    double sum = 0., sum2 = 0.;
    uint32_t max = 0, min = UINT32_MAX;
    for(size_t i=0; i < size; i++){
        r = lcg_rand();
        if (r < min)
            min = r;
        if (r > max)
            max = r;
        sum += r;
        sum2 += r*r;
    }
    double avg = sum / size;
    double std = sqrt(sum2/size - (sum/size)*(sum/size));
    
    uint32_t ideal_max = LCG_RAND_MAX;
    double ideal_avg = ideal_max * 0.5;
    double ideal_std = ideal_max * 0.28868394439521456; // ideal_max * sqrt(1./3 - 1./4 + 1./6/ideal_max);
    
    printf("      min: %u, max: %u, avg: %.16e\n", min, max, avg);
    printf("IDEAL min: %u, max: %u, avg: %.16e\n", 0, ideal_max, ideal_avg);
    
    printf("      std: %.16e, std/avg: %.16e\n", std, std/avg);
    printf("IDEAL std: %.16e, std/avg: %.16e\n", ideal_std, ideal_std/ideal_avg);
}

void test_lcg_uint32(size_t size) { 
    uint32_t r;
    double sum = 0., sum2 = 0.;
    uint32_t max = 0, min = UINT32_MAX;
    for(size_t i=0; i < size; i++){
        r = lcg_uint32();
        if (r < min)
            min = r;
        if (r > max)
            max = r;
        sum += r;
        sum2 += ((double)r)*r;
    }
    double avg = sum/size;
    double std = sqrt(sum2/size - avg*avg);
    
    uint32_t ideal_max = UINT32_MAX;
    double ideal_avg = ideal_max * 0.5;
    double ideal_std = ideal_max * 0.2886751346620252997; // ideal_max * sqrt(1./3 - 1./4 + 1./6/ideal_max);
    
    printf("      min: %u, max: %u, avg: %.16e\n", min, max, avg);
    printf("IDEAL min: %u, max: %u, avg: %.16e\n", 0, ideal_max, ideal_avg);
    
    printf("      std: %.16e, std/avg: %.16e\n", std, std/avg);
    printf("IDEAL std: %.16e, std/avg: %.16e\n", ideal_std, ideal_std/ideal_avg);
}

void test_lcg_flt(size_t size) {
    double sum = 0., sum2 = 0.;
    float max = FLT_MIN, min = FLT_MAX;
    float f;
    size_t count_anomaly = 0;
    for(size_t i = 0; i < size; i++){
        f = lcg_flt();
        if (f < min)
            min = f;
        if (f > max)
            max = f;
        if (f >= 1.0F || f < 0.0F)
            count_anomaly++;
        sum += f;
        sum2 += f*f;
    }
    float avg = sum / size;
    float std = sqrt(sum2/size - (sum/size)*(sum/size));
    if (count_anomaly)
        printf("Nbr of anomalies: %llu\n", count_anomaly);
    
    float ideal_max = 1.0F - LCG_FLT_EPS;
    float ideal_avg = ideal_max * 0.5F;
    float ideal_std = ideal_max * 0.2886751518F; // ideal_max * sqrt(1./3 - 1./4 + LCG_FLT_EPS/6/ideal_max);
    
    printf("      min: %.8e, max: %.8e, avg: %.8e\n", min, max, avg);
    printf("IDEAL min: %.8e, max: %.8e, avg: %.8e\n", 0.F, ideal_max, ideal_avg);
    
    printf("      std: %.8e, std/avg: %.8e\n", std, std/avg);
    printf("IDEAL std: %.8e, std/avg: %.8e\n", ideal_std, ideal_std/ideal_avg);
}

size_t write_rng_bits_to_file(const char *filename, size_t nbr_rnds, uint32_t (*rng_rand)(void), int nbit) {

    FILE *sm_file = NULL;
    if (!(sm_file = fopen(filename, "wb"))) {
        perror("ERR: ");
        return (size_t)-1;
    }

    size_t written = 0;
    unsigned char *buff = (unsigned char *)malloc(nbit);
    
    for (size_t i = 0; i < nbr_rnds / 8; i++) {
        memset(buff, 0, nbit);
        unsigned off = 0;
        unsigned char *pb = buff;
        for (int j = 0; j < 8; j++) {
            uint32_t rnd = rng_rand();               
            unsigned char *pr = (unsigned char *)&rnd;
            int rem;
            for(rem = nbit; rem > 0; rem -= 8) {
                if (rem < 8) {
                    off += rem;
                    if ( off < 8 ) {
                        *pb |= *pr << (8 - off) ;                       
                    } else {
                        off -= 8;
                        *pb |= *pr >> off;
                        pb++;
                        *pb |= *pr << (8 - off);
                    }
                    break;                    
                }
                *pb |= *pr >> off;
                pb++;
                *pb |= *pr << (8 - off);
                pr++;
            }
        }
        written += fwrite(buff, 1, nbit, sm_file);
    }
    
    free(buff);
    fclose(sm_file);
    return written;
}

size_t write_rng_bytes_to_file(const char *filename, size_t nbr_rnds, uint32_t (*rng_uint32)(void)) {

    FILE *sm_file = NULL;
    if (!(sm_file = fopen(filename, "wb"))) {
        perror("ERR: ");
        return (size_t)-1;
    }
    
    size_t written = 0;

    for (size_t i = 0; i < nbr_rnds; i++) {
        uint32_t rnd = rng_uint32();
        written += fwrite(&rnd, 1, 4, sm_file);
    }

    fclose(sm_file);
    return written;
}

size_t intern_stat_period(uint32_t st) {
    size_t period = 0;
    int flag = 0;
    
    for (size_t i = 0; i <= 2 * UINT32_MAX; i++) {
        if ( lcg_rand_intern_stat() == st ) {
            period = i - period;
            if (flag)
                break;
            else
                flag = 1;
        }
        lcg_rand();
    }
    return period;
}

size_t reaching_intern_stat(uint32_t st) {
    for (size_t i = 0; i <= UINT32_MAX; i++) {
        lcg_rand();
        if (lcg_rand_intern_stat() == st)
            return i+1;
    }
    return 0;
}


int main() {
    puts("lcg_rand test:");
    test_lcg_rand((size_t)UINT32_MAX+1ull);
    
    puts("lcg_uint32 test:");
    test_lcg_uint32(2*(size_t)UINT32_MAX);
    
    puts("lcg_flt test:");
    test_lcg_flt(2*(size_t)UINT32_MAX);
    
    uint32_t st = 12345;
    printf("Period of state %u is %llu.\n", st, intern_stat_period(st));
    
    char filename[32];
    sprintf(filename, "LCG%d.bin", 15);
    size_t nbr_rnds = 256000;
    size_t written = write_rng_bits_to_file(filename, nbr_rnds, lcg_rand, 15);
    printf("%llu 15-bit random numbers generated. %llu bytes written to %s.\n", nbr_rnds, written, filename);
    sprintf(filename, "LCG%d.bin", 32);
    nbr_rnds = 128000;
    written = write_rng_bytes_to_file(filename, nbr_rnds, lcg_uint32);
    printf("%llu 32-bit random numbers generated. %llu bytes written to %s.\n", nbr_rnds, written, filename);
    
    return 0;
}
