#include "big_rand.h"

inline bool IsPrime(mpz_class value)
{
    const int reps = 25;  // Number of Miller-Rabin iterations
    const int result = mpz_probab_prime_p(value.get_mpz_t(), reps);
    return result != 0;
}

