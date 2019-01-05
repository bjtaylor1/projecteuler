// euler642.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

mpz_class sum_of_primes(const mpz_class& n)
{
    mpz_class r = mpzfuncs::sqrt(n);
    std::vector<mpz_class> V;
    for (mpz_class i(1); i <= r; i++)
    {
        V.push_back(n / i);
    }
    for (mpz_class i(*V.rbegin() - 1); i > 0; i--)
    {
        V.push_back(i);
    }
    std::map<mpz_class, mpz_class> S;
    for (auto i : V)
    {
        S.insert(std::pair<mpz_class, mpz_class>(i, i*(i + 1) / 2 - 1));
    }

    for (mpz_class p = 2; p <= r + 1; p++)
    {
        if (S[p] > S[p - 1])
        {
            mpz_class sp = S[p - 1];
            mpz_class p2 = p * p;
            for (auto v : V)
            {
                if (v < p2) break;
                S[v] -= p * (S[v / p] - sp);
            }
        }
    }
    return S[n];
}

void count_psmooth_r(const mpz_class& n, const mpz_class& p, const mpz_class& prod, mpz_class& tot, int f)
{
    //e.g.                          100                   2                   2                50       -1
    auto amount = n / prod;
    if (amount > 0)
    {
        
        tot += f * amount;
        for (mpz_class np = mpzfuncs::nextprime(p); np < n; np = mpzfuncs::nextprime(np))
        {
            mpz_class nprod = np * p;
            count_psmooth_r(n, np, np * prod, tot, f * -1);

        }
    }
}

std::map<mpz_class, mpz_class> psmoothcache;
mpz_class count_psmooth(const mpz_class& p, const mpz_class n)
{
    auto existing = psmoothcache.find(p);
    if (existing != psmoothcache.end())
    {
        return existing->second;
    }
    else
    {
        mpz_class val;
        count_psmooth_r(n, p, p, val, 1);
        return val;
    }
}

int main()
{
    long N(100);
    mpz_class r = mpzfuncs::sqrt(N);
    mpz_class tot(0);
    for (mpz_class small = 2; small <= r; small = mpzfuncs::nextprime(small))
    {
        auto occurrences = count_psmooth(small, N);
        tot += small * occurrences;
    }

    long lbound = mpzfuncs::nextprime(r).get_ui();
    while(lbound <= N)
    {
        auto occurrences = N / lbound; // it's rounded off
        auto ubound = mpzfuncs::nextprime(N / occurrences).get_ui();  // it's rounded off - so it's not the same as lbound
        auto totprimes = sum_of_primes(ubound).get_ui() - sum_of_primes(lbound).get_ui() + lbound - ubound;
        auto subtot = totprimes * occurrences;
        tot += subtot;
        lbound = ubound;
    }
    
    std::cout << tot << std::endl;
    //
    //gmp_randinit_default(randstate);

    //mpz_class p(2);
    //long long i = 0;
    //while(p*p < N)
    //{
    //    if(i++ % 1000 == 0) std::cout << p << std::endl;
    //    mpz_class np;
    //    mpz_next_prime_candidate(np.get_mpz_t(), p.get_mpz_t(), randstate);
    //    p = np;
    //}

    
    //auto ans = sum_of_primes(100);
    //std::cout << ans << std::endl;
}
