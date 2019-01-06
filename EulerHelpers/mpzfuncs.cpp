#include "stdafx.h"
#include "mpzfuncs.h"

bool mpzfuncs::randinit = false;
gmp_randstate_t mpzfuncs::randstate;

mpz_class mpzfuncs::sqrt(const mpz_class& c)
{
    mpz_class res;
    mpz_sqrt(res.get_mpz_t(), c.get_mpz_t());
    return res;
}

mpz_class mpzfuncs::nextprime(const mpz_class& c)
{
    mpz_class res;
    if (!(randinit++))
    {
        gmp_randinit_default(randstate);
    }
    mpz_next_prime_candidate(res.get_mpz_t(), c.get_mpz_t(), randstate);
    return res;
}
