#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "pcg.h"

    
void test_pcg_uint32(size_t size) { 
    uint32_t r;
    double sum = 0., sum2 = 0.;
    uint32_t max = 0, min = UINT32_MAX;
    for(size_t i=0; i < size; i++){
        r = pcg_uint32();
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


void test_pcg_dbl(size_t size) {
    long double sum = 0., sum2 = 0.;
    // printf("size of long double %llu\n", sizeof(long double));
    double max = DBL_MIN, min = DBL_MAX;
    double d;
    size_t count_anomaly = 0;
    for(size_t i = 0; i < size; i++){
        d = pcg_dbl();
        if (d < min)
            min = d;
        if (d > max)
            max = d;
        if (d >= 1.0 || d < 0.0)
            count_anomaly++;
        sum += d;
        sum2 += d*d;
    }
    double avg = sum / size;
    double std = sqrt(sum2/size - (sum/size)*(sum/size));
    if (count_anomaly)
        printf("Nbr of anomalies: %zu\n", count_anomaly);
    
    double ideal_max = 1.0 - PCG_DBL_EPS;
    double ideal_avg = ideal_max * 0.5;
    double ideal_std = ideal_max * 0.288675134594812914; // ideal_max * sqrt(1./3 - 1./4 + PCG_DBL_EPS/6/ideal_max);
    
    printf("      min: %.16e, max: %.16e, avg: %.16e\n", min, max, avg);
    printf("IDEAL min: %.16e, max: %.16e, avg: %.16e\n", 0.0, ideal_max, ideal_avg);
    
    printf("      std: %.16e, std/avg: %.16e\n", std, std/avg);
    printf("IDEAL std: %.16e, std/avg: %.16e\n", ideal_std, ideal_std/ideal_avg);
}

size_t write_u32_rng_to_binfile(const char *filename, size_t nbr_rnds, uint32_t (*rng)(void)) {
    
    FILE *sm_file = NULL;
    if (!(sm_file = fopen(filename, "wb"))) {
        perror("ERR: ");
        return (size_t)0;
    }
    
    size_t written = 0;

    for (size_t i = 0; i < nbr_rnds; i++) {
        uint32_t rnd = rng();
        written += fwrite(&rnd, 1, 4, sm_file);
    }

    fclose(sm_file);
    return written;
}

double time_u32_rng(size_t nbr_rnd, uint32_t (*rng)(void)) {
    clock_t tik;
    tik = clock();
    for(size_t i = 0; i < nbr_rnd; i++)
        (void)rng();
    tik = clock() - tik;
    return (double)tik / CLOCKS_PER_SEC;
}

double time_dbl_rng(size_t nbr_rnd, double (*rng)(void)) {
    clock_t tik;
    tik = clock();
    for(size_t i = 0; i < nbr_rnd; i++)
        (void)rng();
    tik = clock() - tik;
    return (double)tik / CLOCKS_PER_SEC;
}

size_t write_double_rng_to_txtfile(const char *filename, size_t nbr_rnds, double (*rng)(void)) {
    
    FILE *sm_file = NULL;
    if (!(sm_file = fopen(filename, "w"))) {
        perror("ERR: ");
        return (size_t)0;
    }
    
    size_t written = 0;

    for (size_t i = 0; i < nbr_rnds; i++) {
        written += fprintf(sm_file, "%.16e\n", rng());
    }

    fclose(sm_file);
    return written;
}


int main(int argc, char **argv) {
    
    uint64_t ex_sd = 0ull;
    char *end=NULL;
    if (argc > 1)
        ex_sd = (uint64_t) strtoull(argv[1], &end, 0);
    
    pcg_seed(((uint64_t)time(NULL)) ^ ex_sd);
    
    puts("pcg_uint32 test:");
    test_pcg_uint32((size_t)UINT32_MAX);   
    
    puts("pcg_dbl test:");
    test_pcg_dbl((size_t)UINT32_MAX);
    
    size_t size = 1000000000;    
    printf("It took %.3fs to generate %zu uint32 random numbers.\n", time_u32_rng(size, pcg_uint32), size);
    printf("It took %.3fs to generate %zu double random numbers.\n", time_dbl_rng(size, pcg_dbl), size);
    
    char filename[32]; *filename = 0;
        
    sprintf(filename, "PCG%d.bin", 32);
    size_t nbr_rnds = 256*1024;
    size_t written = write_u32_rng_to_binfile(filename, nbr_rnds, pcg_uint32);
    printf("%zu 32-bit random numbers generated. %zu bytes written to %s.\n", nbr_rnds, written, filename);
    
    /*
    strcpy(filename, "PCG_FLT64.txt");
    nbr_rnds = 51320000;
    written = write_double_rng_to_txtfile(filename, nbr_rnds, pcg_dbl);
    printf("%zu float64 random numbers generated. %zu bytes written to %s.\n", nbr_rnds, written, filename);
    */
    return 0;
}
