#pragma once

#include "big_rand.h"

inline bool IsPrime(mpz_class value)
{
    const int reps = 25;  // Number of Miller-Rabin iterations
    const int result = mpz_probab_prime_p(value.get_mpz_t(), reps);
    return result != 0;
}

template <size_t size>
inline std::array<unsigned int, size> to_arr(mpz_class value)
{
    std::array<unsigned int, size> result;

    for (auto& el : result)
    {
        el = value.get_ui();
        value >>= 32;
    }

    return result;
}

inline std::vector<unsigned int> to_vec(mpz_class value, size_t size)
{
    std::vector<unsigned int> result(size);

    for (auto& el : result)
    {
        el = value.get_ui();
        value >>= 32;
    }

    return result;
}

template <size_t size>
mpz_class to_mpz_class(std::array<unsigned int, size> value)
{
    mpz_class result{ 0 };

    for (auto it = value.rbegin(); it != value.rend(); ++it)
    {
        result += *it;
        result <<= 32;
    }

    result >>= 32;

    return result;
}

mpz_class to_mpz_class(std::vector<unsigned int> value)
{
    mpz_class result{ 0 };

    for (auto it = value.rbegin(); it != value.rend(); ++it)
    {
        result += *it;
        result <<= 32;
    }

    result >>= 32;

    return result;
}
