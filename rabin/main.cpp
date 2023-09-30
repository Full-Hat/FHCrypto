#include <__chrono/duration.h>
#include <__ranges/concepts.h>
#include <chrono>
#include <ios>
#include <iostream>
#include <ratio>
#include <stdexcept>
#include <string>
#include <thread>
#include <ctime>

#include "core.h"
#include "big_rand.h"

int main()
{
    // big_rand_with_utils r;
    // r.set_default_value_size(64);

    // r.GetRandomPrime_3_mod_4();

    // auto value = r.Rand();
    // do {
    //     value = r.Rand();
    //     mpz_setbit(value.get_mpz_t(), 512 - 2);
    //     mpz_setbit(value.get_mpz_t(), 512 - 1);
    //     mpz_setbit(value.get_mpz_t(), 0);
    //     mpz_setbit(value.get_mpz_t(), 1);
    // }while (!big_rand_with_utils::IsPrime(value));
    
    // std::cout << value.get_str() << std::endl;
    // std::cout << std::boolalpha << big_rand_with_utils::IsPrime(value) << std::endl;
    //exit(0);
    for (int i = 0; i < 1000; i++)
    {   
        //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
        auto keys = GenerateKeys();
        auto encrypted = encrypt(10, keys);
        auto decrypted = decrypt(encrypted, keys);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (decrypted == false)
        {
            throw std::runtime_error(std::to_string(i));
        }
    }
}
