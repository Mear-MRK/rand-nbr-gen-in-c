#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include "pcg.h"



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
        printf("Nbr of anomalies: %llu\n", count_anomaly);
    
    double ideal_max = 1.0 - PCG_DBL_EPS;
    double ideal_avg = ideal_max * 0.5;
    double ideal_std = ideal_max * 0.288675134594812914; // ideal_max * sqrt(1./3 - 1./4 + PCG_DBL_EPS/6/ideal_max);
    
    printf("      min: %.16e, max: %.16e, avg: %.16e\n", min, max, avg);
    printf("IDEAL min: %.16e, max: %.16e, avg: %.16e\n", 0.0, ideal_max, ideal_avg);
    
    printf("      std: %.16e, std/avg: %.16e\n", std, std/avg);
    printf("IDEAL std: %.16e, std/avg: %.16e\n", ideal_std, ideal_std/ideal_avg);
}

int main()
{
    uint64_t size = 11ULL * (uint64_t)UINT32_MAX;
    test_pcg_dbl(size);   

    return 0;
}