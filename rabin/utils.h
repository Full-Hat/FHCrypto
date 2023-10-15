#pragma once

#include "big_rand.h"
#include <cstddef>
#include <vector>

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

inline std::vector<uint32_t> to_vec_32(std::vector<uint8_t> data)
{
    std::vector<uint32_t> result;
    
    for (int i = 0; i < data.size(); i += 4)
    {
        uint32_t value = 0;
        for (int k = i + 3; k >= i; --k)
        {
            if (k >= data.size())
            {
                continue;
            }

            value += data[k];

            if (k != i)
            {
                value <<= 8;
            }
        }

        result.push_back(value);
    }

    return result;
}

inline std::vector<uint8_t> to_vec_8(std::vector<uint32_t> data, size_t real_size)
{
    std::vector<uint8_t> result;
    unsigned int counter = 0;
    for (auto el : data)
    {
        for (int i = 0; i < 4; ++i)
        {
            result.push_back((uint8_t)el);
            el >>= 8;
            
            ++counter;

            if (counter == real_size)
            {
                return result;
            }
        }
    }

    return result;
}
