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

mpz_class N(10000);
//bool count_psmooth_r(const mpz_class& p, const mpz_class& prod, mpz_class& tot, int f, int depth)
//{
//    //e.g.                          100                   2                   2                50       -1
//    auto amount = N / prod;
//    if (amount > 0)
//    {
//        tot += f * amount;
//        for (mpz_class np = mpzfuncs::nextprime(p); np < N; np = mpzfuncs::nextprime(np))
//        {
//            if (depth == 0) std::cout << p << std::endl;
//            if (depth == 1) std::cout << " " << p << std::endl;
//            if (depth == 2) std::cout << "  " << p << std::endl;
//            if (depth == 3) std::cout << "   " << p << std::endl;
//            if (depth == 4) std::cout << "    " << p << std::endl;
//            mpz_class nprod = np * p;
//            if (!count_psmooth_r(np, np * prod, tot, f * -1, depth+1)) break;
//        }
//
//        return true;
//    }
//    else return false;
//}
//
//std::map<mpz_class, mpz_class> psmoothcache;
std::vector<mpz_class> primes;

mpz_class hamming(const mpz_class& n, const int& primeindex)
{
    if (primeindex == 0)
    {
        mpz_class ret = mpz_sizeinbase(n.get_mpz_t(), 2); // should be -1, but count 1 as well
        return ret;
    }
    else if (n == 0)
    {
        return 0;
    }
    else
    {
        mpz_class l = hamming(n, primeindex - 1);
        mpz_class r = hamming(n / primes[primeindex], primeindex);
        mpz_class ret = l + r;
        return ret;
    }
}

mpz_class count_psmooth(int primeindex)
{
    mpz_class l = hamming(N, primeindex);
    mpz_class r = primeindex == 0 ? 1 : hamming(N, primeindex - 1);
    mpz_class retval = l - r;
    return retval;
}

//mpz_class count_psmooth(const mpz_class& p, const mpz_class n)
//{
//    auto existing = psmoothcache.find(p);
//    if (existing != psmoothcache.end())
//    {
//        return existing->second;
//    }
//    else
//    {
//        mpz_class val;
//        count_psmooth_r(p, p, val, 1, 0);
//        return val;
//    }
//}

int main()
{
    mpz_class r = mpzfuncs::sqrt(N);
    for (mpz_class prime(2); prime <= r; prime = mpzfuncs::nextprime(prime))
    {
        primes.push_back(prime);
    }

    std::cout << "hamming: " << hamming(1000000000, primes.size() - 1) << std::endl;

    mpz_class totsmall(0), totlarge(0);
    std::cout << "r = " << r << std::endl;
    std::cout << "small:" << std::endl;
    for (int primeindex = 0; primeindex < primes.size(); primeindex++)
    {
        mpz_class occurrences = count_psmooth(primeindex);
        mpz_class small = primes[primeindex];
        std::cout << small << "\r";
#if _DEBUG
        std::cout << small << ": " << occurrences << " = " << (small * occurrences) << std::endl;
#endif
        totsmall += primes[primeindex] * occurrences;
    }

    std::cout << "total small: " << totsmall << std::endl;
    std::cout << std::endl << "large:" << std::endl;

    auto lbound = mpzfuncs::nextprime(r);
    while (lbound <= N)
    {
        std::cout << lbound << "\r";
        mpz_class occurrences = N / lbound; // it's rounded off
        mpz_class ubound = mpzfuncs::nextprime(N / occurrences);  // it's rounded off - so it's not the same as lbound
        mpz_class totprimes = sum_of_primes(ubound) - sum_of_primes(lbound) + lbound - ubound;
        mpz_class subtot = totprimes * occurrences;
        totlarge += subtot;
        lbound = ubound;
    }

    std::cout << "total large: " << totlarge << std::endl;
    mpz_class tot = totlarge + totsmall;
    std::cout << std::endl << tot << std::endl;
}
