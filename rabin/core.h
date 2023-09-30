#include "gmpxx.h"
#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
#include <cstddef>
#include <gmp.h>
#include <iostream>

#include "big_rand.h"

using pqn_t = std::tuple<mpz_class, mpz_class, mpz_class>;

inline pqn_t GenerateKeys()
{
    big_rand_with_utils rand;
    rand.set_default_value_size(64);

    auto p = rand.GetRandomPrime_3_mod_4();
    auto q = rand.GetRandomPrime_3_mod_4();

    // mpz_class p {"13210729996893311803250560543885063660354761034075953601739315032701180045331188672961524373211325196207565324855386875786338412814740626960334964631926519"};
    // mpz_class q { "12988667901345057445081953465189537197696752315974534791535433550448866032334410079706329613456650922962512077226009253308851835749729581758857981739152863" };

    mpz_class n = p * q;

    return { p, q, n };
}

// inline uint32_t convert(mpz_class value)
// {
//     uint32_t result;
//     mpz_export(&result, nullptr, 1, sizeof(result), 0, 0, value.get_mpz_t());

//     return result;
// }

inline uint32_t encrypt(uint32_t value, pqn_t pqn)
{
    auto [p, q, n] = pqn;

    mpz_class multiplied = value * value;
    mpz_class crypted = multiplied % n;

    return convert(crypted);
}

inline bool decrypt(uint32_t value, pqn_t pqn)
{
    auto [p, q, n] = pqn;

    mpz_class c = value;

    mpz_class exp1; // (p + 1) / 4
    mpz_add_ui(exp1.get_mpz_t(), p.get_mpz_t(), 1);
    mpz_div_ui(exp1.get_mpz_t(), exp1.get_mpz_t(), 4);

    mpz_class exp2; // (q + 1) / 4
    mpz_add_ui(exp2.get_mpz_t(), q.get_mpz_t(), 1);
    mpz_div_ui(exp2.get_mpz_t(), exp2.get_mpz_t(), 4);

    mpz_class mp, mq; // c ^ exp<12> mod <pq>
    mpz_powm(mp.get_mpz_t(), c.get_mpz_t(), exp1.get_mpz_t(), p.get_mpz_t());
    mpz_powm(mq.get_mpz_t(), c.get_mpz_t(), exp2.get_mpz_t(), q.get_mpz_t());

    mpz_class temp, yp, yq; // yp * p + yq * q == 1
    mpz_gcdext(temp.get_mpz_t(), yp.get_mpz_t(), yq.get_mpz_t(), p.get_mpz_t(), q.get_mpz_t());
    
    mpz_class r[4];
    mpz_mod(r[0].get_mpz_t(), ((mpz_class)(yp * p * mq + yq * q * mp)).get_mpz_t(), n.get_mpz_t());
    r[1] = n - r[0];
    mpz_mod(r[2].get_mpz_t(), ((mpz_class)(yp * p * mq - yq * q * mp)).get_mpz_t(), n.get_mpz_t());
    r[3] = n - r[2];

    for (auto el : r)
    {
        if (el.get_str().size() <= 100)
        {
            return convert(el);
        }
    }
    
    return false;
}


