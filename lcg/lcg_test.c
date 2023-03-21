#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "lcg.h"

    
void test_lcg_uint16(size_t size) { 
    uint32_t r;
    double sum = 0., sum2 = 0.;
    uint32_t max = 0, min = UINT32_MAX;
    for(size_t i=0; i < size; i++){
        r = lcg_uint16();
        if (r < min)
            min = r;
        if (r > max)
            max = r;
        sum += r;
        sum2 += r*r;
    }
    double avg = sum / size;
    double std = sqrt(sum2/size - (sum/size)*(sum/size));
    
    uint32_t ideal_max = LCG_UINT16_MAX;
    double ideal_avg = ideal_max * 0.5;
    double ideal_std = ideal_max * sqrt(1./3 - 1./4 + 1./6/ideal_max);
    
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
    double ideal_std = ideal_max * sqrt(1./3 - 1./4 + 1./6/ideal_max);
    
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
        printf("Nbr of anomalies: %zu\n", count_anomaly);
    
    float ideal_max = 1.0F - LCG_FLT_EPS;
    float ideal_avg = ideal_max * 0.5F;
    float ideal_std = ideal_max * sqrt(1./3 - 1./4 + LCG_FLT_EPS/6/ideal_max);
    
    printf("      min: %.8e, max: %.8e, avg: %.8e\n", min, max, avg);
    printf("IDEAL min: %.8e, max: %.8e, avg: %.8e\n", 0.F, ideal_max, ideal_avg);
    
    printf("      std: %.8e, std/avg: %.8e\n", std, std/avg);
    printf("IDEAL std: %.8e, std/avg: %.8e\n", ideal_std, ideal_std/ideal_avg);
}

size_t write_rng_bytes_to_file(const char *filename, size_t nbr_rnds, uint32_t (*rng)(void), unsigned nbr_bytes_rng) {
    assert(nbr_bytes_rng <= 4);
    FILE *sm_file = NULL;
    if (!(sm_file = fopen(filename, "wb"))) {
        perror("ERR: ");
        return (size_t)0;
    }
    
    size_t written = 0;

    for (size_t i = 0; i < nbr_rnds; i++) {
        uint32_t rnd = rng();
        written += fwrite(&rnd, 1, nbr_bytes_rng, sm_file);
    }

    fclose(sm_file);
    return written;
}

size_t write_float_rng_to_txtfile(const char *filename, size_t nbr_rnds, float (*rng)(void)) {
    
    FILE *sm_file = NULL;
    if (!(sm_file = fopen(filename, "w"))) {
        perror("ERR: ");
        return (size_t)0;
    }
    
    size_t written = 0;

    for (size_t i = 0; i < nbr_rnds; i++) {
        float rnd = rng();
        written += fprintf(sm_file, "%.8e\n", rnd);
    }

    fclose(sm_file);
    return written;
}

size_t lcg_uint16_intern_stat_period(uint32_t st) {
    size_t period = 0;
    int flag = 0;
    
    for (size_t i = 0; i <= (2ull * UINT32_MAX); i++) {
        if ( lcg_uint16_intern_stat() == st ) {
            period = i - period;
            if (flag)
                break;
            else
                flag = 1;
        }
        lcg_uint16();
    }
    return period;
}

size_t lcg_uint16_reaching_intern_stat(uint32_t st) {
    for (size_t i = 0; i <= UINT32_MAX; i++) {
        lcg_uint16();
        if (lcg_uint16_intern_stat() == st)
            return i+1;
    }
    return 0;
}


int main(int argc, char **argv) {
    
    uint64_t ex_sd = 0ull;
    char *end=NULL;
    if (argc > 1)
        ex_sd = (uint64_t) strtoull(argv[1], &end, 0);
    
    lcg_seed(((uint64_t)time(NULL)) ^ ex_sd);
    
    
    puts("lcg_uint16 test:");
    test_lcg_uint16((size_t)UINT32_MAX+1ull);
    
    puts("lcg_uint32 test:");
    test_lcg_uint32((size_t)UINT32_MAX);
    
    puts("lcg_flt test:");
    test_lcg_flt((size_t)UINT32_MAX);
    
    
    uint32_t st = 12345;
    printf("Period of lcg_uint16 state %u is %zu. It should be %zu for any state.\n", 
            st, lcg_uint16_intern_stat_period(st), (size_t)UINT32_MAX+1);
    
    
    char filename[32]; *filename = 0;
    
    sprintf(filename, "LCG%d.bin", 16);
    size_t nbr_rnds = 512*1024;
    size_t written = write_rng_bytes_to_file(filename, nbr_rnds, lcg_uint16, 2);
    printf("%zu 16-bit random numbers generated. %zu bytes written to %s.\n", nbr_rnds, written, filename);
    
    sprintf(filename, "LCG%d.bin", 32);
    nbr_rnds = 256*1024;
    written = write_rng_bytes_to_file(filename, nbr_rnds, lcg_uint32, 4);
    printf("%zu 32-bit random numbers generated. %zu bytes written to %s.\n", nbr_rnds, written, filename);
    /*
    strcpy(filename, "LCG_FLT32.txt");
    nbr_rnds = 51320000;
    written = write_float_rng_to_txtfile(filename, nbr_rnds, lcg_flt);
    printf("%zu float32 random numbers generated. %zu bytes written to %s.\n", nbr_rnds, written, filename);
    */
    return 0;
}
