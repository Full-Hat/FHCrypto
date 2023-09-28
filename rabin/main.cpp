#include <iostream>

#include "gmpxx.h"

int main()
{
    mpz_class a("12345678901234567890");
    mpz_class b("98765432109876543210");
    mpz_class gcd, x, y;
    mpz_gcdext(gcd.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    std::cout << "The solutions are (" << x << ", " << y << ")" << std::endl;
    return 0;
}
