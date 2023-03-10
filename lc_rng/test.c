#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

#include "lc_rng.h"

#define SZ UINT_MAX
    
    
void test_lc_rng_uint(unsigned size) { 
    uint32_t r;
    double sum = 0., sum2 = 0.;
    uint32_t max = 0, min = UINT_MAX;
    for(unsigned i=0; i!=size; i++){
        r = lc_rng_uint();
        if (r < min)
            min = r;
        if (r > max)
            max = r;
        sum += r;
        sum2 += r*r;
    }
    printf("avg: %g, std: %g, max: %u, min: %u\n", 
        sum/size, sqrt((sum2/size-sum/size*sum/size)), max, min);  
}

void test_lc_rng_flt(unsigned size) {
     double   sum = 0., sum2 = 0.;
    float max_f = FLT_MIN, min_f = FLT_MAX;
    float f;
    int count_anomaly = 0;
    for(unsigned i=0; i!=size; i++){
        f = lc_rng_flt();
        if (f < min_f)
            min_f = f;
        if (f > max_f)
            max_f = f;
        if (f >= 1.0F || f < 0.0F)
            count_anomaly++;
        sum += f;
        sum2 += f*f;
    }
    printf("avg: %g, std: %g, max: %.8e, min: %.8e, anomalies: %d\n", 
        sum/size, sqrt((sum2/size-sum/size*sum/size)), max_f, min_f, count_anomaly);
}

size_t test_period(uint32_t v) {
    size_t period = 0;
    int flag = 0;
    uint32_t internal = 0;
    
    for (size_t i = 0; i <= 2 * UINT_MAX; i++) {
        if ( lc_rng_uint() == v ) {
            if (! flag ) {
                internal = lc_rng_intern_stat();
                flag = 1;
                period = i;
                continue;
            }
            if (internal == lc_rng_intern_stat()) {
                period = i - period;
                break;
            }
        }
    }
    return period;

}
    



int main(int argc, char **argv) {
    test_lc_rng_uint(SZ);
    test_lc_rng_flt(SZ);
    unsigned v = 12345;
    if (argc > 1)
        v = atoi(argv[1]);
    printf("Period of %u is %u.\n", v, test_period(v));
}
