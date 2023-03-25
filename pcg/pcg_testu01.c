#include "unif01.h"
#include "bbattery.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "pcg.h"


double GetU01(void *param, void* state) {
    return pcg_dbl();
}

unsigned long GetBits(void *param, void *state) {
    return (unsigned long)pcg_uint32();
}

void Write(void *state) {
    printf("PCG intern_stat: %lu\n", pcg_intern_stat());
}

int main () 
{
   unif01_Gen *gen = calloc(1, sizeof(unif01_Gen));
   gen->name = "PCG";
   gen->GetU01 = GetU01;
   gen->GetBits = GetBits;
   gen->Write = Write;
   
   pcg_seed(time(NULL));

   bbattery_Crush(gen);
    
   free(gen);
   return 0;
}
