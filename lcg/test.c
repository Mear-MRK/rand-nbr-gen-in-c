#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "lcg.h"

#define SZ UINT_MAX

void bytes_to_bits_str_nb(const void *bytes, char *str, size_t nbr_bits) {
    unsigned char *ch_bytes = (unsigned char*)bytes;
    size_t n_bytes = nbr_bits/8, r_bits = nbr_bits % 8;
    for (size_t i = 0; i < n_bytes; i++) {
        for(unsigned char hot_1 = 0x80U; hot_1 > 0; hot_1 >>= 1) {
            *str++ = (ch_bytes[i] & hot_1) ? '1' : '0';
        }
    }
    for(unsigned char hot_1 = 1U << (r_bits-1); hot_1 > 0; hot_1 >>= 1) {
            *str++ = (ch_bytes[n_bytes] & hot_1) ? '1' : '0';
    }
    
    *str = '\0';
}
        

void insert_delimiters(const char *inp_str, char *out_str, unsigned int n, char dlm) {
    unsigned int i = 0;
    char r;
    while( (r = *inp_str++) ) {
        *out_str++ = r;
        if ( ++i == n  ) {
            *out_str++ = dlm;
            i = 0;
        }
    }
    *out_str = '\0';
}
    
    
void test_lcg_rand(unsigned size) { 
    uint32_t r;
    double sum = 0., sum2 = 0.;
    uint32_t max = 0, min = UINT32_MAX;
    for(unsigned i=0; i!=size; i++){
        r = lcg_rand();
        if (r < min)
            min = r;
        if (r > max)
            max = r;
        sum += r;
        sum2 += r*r;
    }
    double avg = sum / size;
    double std = sqrt(sum2/size - avg*avg);
    printf("min: %u, max: %u, avg: %g, std: %g, std/avg: %g\n",
            min, max, avg, std, std/avg);  
}

void test_lcg_flt(unsigned size) {
    double sum = 0., sum2 = 0.;
    float max = FLT_MIN, min = FLT_MAX;
    float f;
    unsigned count_anomaly = 0;
    for(unsigned i=0; i!=size; i++){
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
    double avg = sum / size;
    double std = sqrt(sum2/size - avg*avg);
    printf("min: %.8e, max: %.8e, avg: %g, std: %g, std/avg: %g, nbr anomalies: %u\n",
            min, max, avg, std, std/avg, count_anomaly);
}


size_t write_rng_bits_to_file(const char *filename, size_t nbr_rnds, uint32_t (*rng_rand)(void), int nbit) {

    FILE *sm_file = NULL;
    if (!(sm_file = fopen(filename, "wb")))
    {
        perror("ERR: ");
        return (size_t)-1;
    }
    #define N_P 9
    char str_out[32*N_P+1];
    memset(str_out, 0, 32*N_P+1);
    size_t written = 0;
    unsigned char *buff = (unsigned char *)malloc(nbit);
    unsigned count = 0;
    for (size_t i = 0; i < nbr_rnds / 8; i++) {
        memset(buff, 0, nbit);
        unsigned off = 0;
        unsigned char *pb = buff;
        for (int j = 0; j < 8; j++) {
            uint32_t rnd = rng_rand();
            if (count++ < N_P) {
                // printf("%08x,", rnd);
                char bin[33];
                bytes_to_bits_str_nb((char*)&rnd, bin, nbit);
                strcat(str_out, bin);
            }
                
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
    // puts(str_out);
    char sp_out[36*N_P+1];
    insert_delimiters(str_out, sp_out, 8, ',');
    puts(sp_out);
    
    free(buff);
    fclose(sm_file);
    return written;
}

size_t test_period(uint32_t v) {
    size_t period = 0;
    int flag = 0;
    uint32_t internal = 0;
    
    for (size_t i = 0; i <= 2 * UINT_MAX; i++) {
        if ( lcg_rand() == v ) {
            if (! flag ) {
                internal = lcg_intern_stat();
                flag = 1;
                period = i;
                continue;
            }
            if (internal == lcg_intern_stat()) {
                period = i - period;
                break;
            }
        }
    }
    return period;

}

size_t test_intern_stat(uint32_t st) {
    for (size_t i = 0; i <= UINT_MAX; i++) {
        if (lcg_intern_stat() == st)
            return i;
        lcg_rand();
    }
    return (size_t) -1;
}
    

static int nbit = 15;

uint32_t rnd(void) {
    return lcg_rand_nbit(nbit);
}

uint32_t ones(void) {
    return ((~0U) >> (32-nbit));
}

uint32_t ozoz(void) {
    uint32_t out = 0U;
    for(int i = 0; i < nbit; i+=2 )
        out |= (1U << i);
    return out;
}

int main(int argc, char **argv) {
    // test_lcg_rand(SZ);
    // test_lcg_flt(SZ);
    // unsigned v = 12345;
    if (argc > 1)
        nbit = (unsigned) atoi(argv[1]);
    assert(nbit <= 32);
    size_t nbr_rnds = ceil(1024*1024.0/nbit/8.0)*8;
    if (argc > 2)
        nbr_rnds = (size_t) atol(argv[2]);  
    
    // printf("Period of %u is %llu.\n", v, test_period(v));
    // printf("After %llu steps reaching state %u.\n", test_intern_stat(v), v);
    char filename[32];
    sprintf(filename, "LCG%d.bin", nbit);
    size_t written = write_rng_bits_to_file(filename, nbr_rnds, rnd, nbit);
    printf("\n%llu %ubit random numbers generated. %llu bytes written to %s.\n", nbr_rnds, nbit, written, filename);
    return 0;
}
