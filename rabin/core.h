#include "gmpxx.h"
#include <cstddef>
#include <gmp.h>
#include <iostream>
#include <array>
#include <sstream>

#include "big_rand.h"
#include "utils.h"

using pqn_t = std::tuple<mpz_class, mpz_class, mpz_class>;

class Rabin
{
public:
    static std::pair<mpz_class, mpz_class> GenerateKeys()
    {
        big_rand_with_utils rand;
        rand.set_default_value_size(64);

        auto p = rand.GetRandomPrime_3_mod_4();
        auto q = rand.GetRandomPrime_3_mod_4();

        mpz_class n = p * q;

        return { p, q };
    }

    Rabin()
    {
        auto [p, q] = GenerateKeys();
        m_p = p;
        m_q = q;
        m_n = p * q;
    }

    Rabin(mpz_class public_key)
    {
        m_n = public_key;
        m_p = 0;
        m_q = 0;
    }

    Rabin(mpz_class p, mpz_class q)
    {
        m_p = p;
        m_q = q;
        m_n = p * q;
    }

    mpz_class encrypt(std::vector<unsigned int> value)
    {
        if (value.size() + 1 > 32)
        {
            throw std::invalid_argument("value must have size <= 128 bytes, it's algorithm requirements, real size is [" + value.size() * sizeof(value[0]) + std::string("]"));
        }

        value.push_back(calculate_check_sum(value));

        mpz_class value_mpz = to_mpz_class(value);

        mpz_class multiplied = value_mpz * value_mpz;
        mpz_class crypted;
        mpz_mod(crypted.get_mpz_t(), multiplied.get_mpz_t(), m_n.get_mpz_t());

        return crypted;
        //return to_vec(crypted, 32);
    }

    std::vector<unsigned int> decrypt(mpz_class a/*std::vector<unsigned int> value*/)
    {
        /*if (value.size() > 32)
        {
            throw std::invalid_argument("value must have size <= 128 bytes, it's algorithm requirements, real size is [" + value.size() * sizeof(value[0]) + std::string("], check if you get correct message"));
        }*/

        mpz_class c = a;// to_mpz_class(value);

        mpz_class exp1; // (p + 1) / 4
        mpz_add_ui(exp1.get_mpz_t(), m_p.get_mpz_t(), 1);
        mpz_div_ui(exp1.get_mpz_t(), exp1.get_mpz_t(), 4);

        mpz_class exp2; // (q + 1) / 4
        mpz_add_ui(exp2.get_mpz_t(), m_q.get_mpz_t(), 1);
        mpz_div_ui(exp2.get_mpz_t(), exp2.get_mpz_t(), 4);

        mpz_class mp, mq; // c ^ exp<12> mod <pq>
        mpz_powm(mp.get_mpz_t(), c.get_mpz_t(), exp1.get_mpz_t(), m_p.get_mpz_t());
        mpz_powm(mq.get_mpz_t(), c.get_mpz_t(), exp2.get_mpz_t(), m_q.get_mpz_t());

        mpz_class temp, yp, yq; // yp * p + yq * q == 1
        mpz_gcdext(temp.get_mpz_t(), yp.get_mpz_t(), yq.get_mpz_t(), m_p.get_mpz_t(), m_q.get_mpz_t());

        mpz_class r[4];
        mpz_mod(r[0].get_mpz_t(), ((mpz_class)(yp * m_p * mq + yq * m_q * mp)).get_mpz_t(), m_n.get_mpz_t());
        r[1] = m_n - r[0];
        mpz_mod(r[2].get_mpz_t(), ((mpz_class)(yp * m_p * mq - yq * m_q * mp)).get_mpz_t(), m_n.get_mpz_t());
        r[3] = m_n - r[2];

        for (auto el : r)
        {
            auto data = to_vec(el, 31/*value.size()*/);
            std::span<unsigned int> data_span{ data.begin(), --data.end() };

            if (*data.rbegin() == calculate_check_sum(data_span))
            {
                data.pop_back();
                return data;
            }
        }

        throw std::invalid_argument("Can't decrypt message, check for keys and data correctness");
    }

protected:
    static unsigned int calculate_check_sum(const std::span<unsigned int> value)
    {
        unsigned int checksum = 0;

        for (auto el : value)
        {
            checksum += el;
        }

        return checksum;
    }
private:
    mpz_class m_p, m_q, m_n;
};
