#pragma once

#ifndef MPZFUNCS
#define MPZFUNCS

#include <mpirxx.h>

class mpzfuncs
{
private:
    static bool randinit;
    static gmp_randstate_t randstate;

public:
    static mpz_class sqrt(const mpz_class& c);
    static mpz_class nextprime(const mpz_class& c);
};

#endif