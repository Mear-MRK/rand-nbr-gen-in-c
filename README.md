# Random number generators in C

To ensure reliable simulation, it is important to have a trusted and controlled pseudo-random number generator (PRNG).
To this end, I have implemented two PRNGs, namely the Linear Congruential generator (LCG) and the Permuted Congruential generator (PCG).

To test the efficacy of these generators, I have subjected them to SP800-22 tests, and the results have been included.
Furthermore, I have tested the PCG against TestU01 Crush.

The `*cg_uintXX` functions generate XX-bit unsigned integers, while the `lcg_flt` and `pcg_dbl` functions 
generate single precision (float32) and double precision (float64), respectively, in the range of `[0, 1)`.

I have followed recommended parameters for LCG as suggested in this paper: https://doi.org/10.1090/S0025-5718-99-00996-5.
For PCG, I have used the parameters suggested in the original paper: https://www.pcg-random.org/paper.html.

To perform the SP800-22 test, I used the codes available in this repository: https://github.com/dj-on-github/sp800_22_tests.
Similarly, for the TestU01, I used the codes available on this webpage: http://simul.iro.umontreal.ca/testu01/tu01.html.
