#include "gmpxx.h"

const auto public_key_size = 1024;
const auto block_size = public_key_size / 8;

const auto key_factor_size = public_key_size / 2;

const auto checksum_size = 16;

using pqn_t = std::tuple<mpz_class, mpz_class, mpz_class>;

inline bool IsPrime(mpz_class value)
{
    int reps = 25;  // Number of Miller-Rabin iterations
    int result = mpz_probab_prime_p(value.get_mpz_t(), reps);
    return result != 0;
}
inline mpz_class GetRandomValue()
{
    mpz_class value;
    unsigned long bit_length = 512;  // Specify the desired bit length here
    gmp_randstate_t state;
    gmp_randinit_default(state);
    mpz_urandomb(value.get_mpz_t(), state, bit_length);
    gmp_randclear(state);

    return value;
}

inline mpz_class GetPrime_3()
{
    mpz_class value;
    do
    {
        value = GetRandomValue();

        mpz_setbit(value.get_mpz_t(), 512 - 2);
        mpz_setbit(value.get_mpz_t(), 512 - 1);
        mpz_setbit(value.get_mpz_t(), 0);
        mpz_setbit(value.get_mpz_t(), 1);


    } while (!IsPrime(value));

    return value;
}

inline pqn_t GenerateKeys()
{
    auto p = GetPrime_3();
    auto q = GetPrime_3();

    auto n = p * q;

    return { p, q, n };
}


