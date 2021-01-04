#include <iostream>
#include <mpirxx.h>
#include <set>
#include <vector>
#include "primes.h"
#include "outputhelpers.h"
#include "counter.h"
#include "constants.h"

class solver
{ 
public:
    int n, k;
    bool* isprime;
    int* composites;
    std::set<int> primes;
    solver(int n, int k) :n(n), k(k)
    {
        isprime = new bool[n+1];
        composites = new int[n+1];
        memset(composites, 0, (n+1)*sizeof(int));
        primes = makeprimes(n+1, isprime);
    }
    ~solver() { delete[] isprime; delete[] composites; }

    void make(std::set<int>& tuple, const std::set<int>::const_iterator first, int bits, mpz_class& count, int depth)
    {
        for(std::set<int>::const_iterator it = first; it != primes.end(); it++)
        {
            int val = *it;
            int newbits = bits | val;

            tuple.insert(val);

            // if(isprime[newbits])
            // {
            //     mpz_class permutations_given, permutations_remaining;
            //     mpz_class S(s);
            //     mpz_fac_ui(permutations_given.get_mpz_t(), s);
            //     mpz_pow_ui(permutations_remaining.get_mpz_t(), S.get_mpz_t(), k - s);
            //     count = (count + permutations_given * permutations_remaining) % 1000000007;
            // }
            if(newbits <= n && isprime[newbits])
            {
                writecsv(tuple, std::cout) << "   =" << newbits << std::endl;
                count++; // for now
            }
       
            if(tuple.size() < k)
            {
                make(tuple, std::next(it), newbits, count, depth+1);
            }
            tuple.erase(val);
        }
    }

    mpz_class solve()
    {
        mpz_class retval = 0;
        std::set<int> tuple;
        make(tuple, primes.begin(), 0, retval, 0);

        std::cout << std::endl;

        // calculate the number of primes that 'fit into' each prime
        for(std::set<int>::const_iterator it1 = primes.begin(); it1 != primes.end(); it1++)
        {
            int subprimes = 0;
            for(std::set<int>::const_iterator it2 = primes.begin(); *it2 < *it1; it2++)
            {
                int p1 = *it1, p2 = *it2;
                if(p1 == (p1 | p2)) 
                {
                    composites[p1|p2] += composites[p1] + composites[p2] + 1;
                    std::cout << p1 << " | " << p2 << " = " << (p1|p2) << ", c[" << (p1|p2) <<"] = " << composites[p1|p2] << std::endl;
                }
            }
        }

        std::cout << std::endl;
        for(std::set<int>::const_iterator it = primes.begin(); it != primes.end(); it++)
        {
            std::cout << "c[" << (*it) << "] = " << composites[(*it)] << std::endl;
        }

        return retval;
    }
};

mpz_class setcount(int n, int k);
mpz_class setcount(int n, int k, int m)
{
    if(m == 1) return k;
    return k * setcount(n, k-1);
}

mpz_class setcount(int n, int k)
{
    mpz_class res;
    if(k == 1)
    {
        res = 1;
    }
    else if(n==k)
    {
        mpz_fac_ui(res.get_mpz_t(), n);
    }
    else
    {
        res = (setcount(n-1, k) + setcount(n-1, k-1));
        res = res * k;
    }   
    return res;
}

void makesetcount(int n, int k)
{
    std::cout << setcount(n,k) << std::endl;
}



int main(int argc, char** argv)
{
    if(argc < 3) throw std::runtime_error("Usage: 734 n k");
    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    solver s(n, k);
    std::cout << s.solve() << std::endl;

    // int n = std::stoi(argv[1]);
    // bool* isprime = new bool[n+1];
    // std::set<int> primes = makeprimes(n+1, isprime);
    // std::cout << primes.size() << std::endl;
    // mpz_class t(0);

    return 0;
}