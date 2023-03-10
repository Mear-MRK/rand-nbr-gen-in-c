#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pc_rng.h"

#define SZ 30000000

int main()
{
    uint32_t i, r;
    double avg = 0., avg2 = 0., std, di = 1. / SZ;
    uint32_t max = 0, min = UINT32_MAX;

    FILE *sm_file = NULL;
    const char *sm_filename = "rnd_uint_sample.txt";
    if (!(sm_file = fopen(sm_filename, "w")))
    {
        fprintf(stderr, "Can't open %s to write!", sm_filename);
        exit(-1);
    }

    for (i = 0; i != SZ; i++)
    {
        r = pc_rng_uint();
        fprintf(sm_file, "%u\n", r);
        if (r < min)
            min = r;
        if (r > max)
            max = r;
        avg += r * di;
        avg2 += r * di * r;
    }

    fclose(sm_file);
    std = sqrt(avg2 - avg * avg) * sqrt((double)SZ / (SZ - 1));

    double idealAvg = PC_RNG_UINT_MAX / 2.;
    double idealStd = sqrt(PC_RNG_UINT_MAX) * sqrt(PC_RNG_UINT_MAX + 2.) / 2. / sqrt(3.);
    printf("avg: %g, std: %g\navg/idealAvg: %g, std/idealStd: %g\n",
           avg, std, avg / idealAvg, std / idealStd);
    printf("max: %u, min: %u, max_uint_rnd: %u\nmin/idealStd: %g, max/idealMax: %g\n",
           max, min, PC_RNG_UINT_MAX, min / idealStd, (double)max / PC_RNG_UINT_MAX);

    return 0;
}