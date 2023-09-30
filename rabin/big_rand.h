#pragma once

#include "gmpxx.h"
#include <cstddef>
#include <ctime>
#include <gmp.h>
#include <utility>
#include <chrono>
#include <iostream>

class big_rand
{
public:
    big_rand()
    {
        gmp_randinit_default(m_state);
        gmp_randseed_ui(m_state, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    }

    ~big_rand()
    {
        gmp_randclear(m_state);
    }

    mpz_class Rand()
    {
        return Rand(m_default_values_size);
    }

    mpz_class Rand(size_t value_size)
    {
        mpz_class rand_value;
        mpz_urandomb(rand_value.get_mpz_t(), m_state, value_size * 8);
        return rand_value;
    }

    void set_default_value_size(size_t value_size)
    {
        m_default_values_size = value_size;
    }
    
protected:
    gmp_randstate_t m_state;
    size_t m_default_values_size;
};

inline uint32_t convert(mpz_class value)
{
    std::string strValue = value.get_str();

    uint32_t result = std::stoul(strValue);

    return result;
}

class big_rand_with_utils : public big_rand
{
public:
    big_rand_with_utils() : big_rand()
    {
    }

    ~big_rand_with_utils() = default;

    static bool IsPrime(const mpz_class &value)
    {
        const int reps = 25;  // Number of Miller-Rabin iterations
        const int result = mpz_probab_prime_p(value.get_mpz_t(), reps);
        return result != 0;
    }

    mpz_class GetRandomPrime_3_mod_4()
    {
        mpz_class value;
        do
        {
            value = this->Rand();

            mpz_setbit(value.get_mpz_t(), 512 - 2);
            mpz_setbit(value.get_mpz_t(), 512 - 1);
            mpz_setbit(value.get_mpz_t(), 0);
            mpz_setbit(value.get_mpz_t(), 1);

        } while (!big_rand_with_utils::IsPrime(value));

        return value;
    }
};
