#include <iostream>
#include <mpirxx.h>

#include "primes.h"
#include "outputhelpers.h"
#include "counter.h"
#include "constants.h"

class solver
{ 
public:
    int n, k;
    bool* isprime;
    std::set<int> primes;
    solver(int n, int k) :n(n), k(k)
    {
        isprime = new bool[n+1];
        primes = makeprimes(n+1, isprime);
    }
    ~solver() { delete[] isprime; }

    void make(counter<int>& tuple, const std::set<int>::const_iterator first, int bits, mpz_class& count, int depth)
    {
        for(std::set<int>::const_iterator it = first; it != primes.end(); it++)
        {
            int val = *it;
            int newbits = bits | val;

            tuple.add(val);

            // if(isprime[newbits])
            // {
            //     mpz_class permutations_given, permutations_remaining;
            //     mpz_class S(s);
            //     mpz_fac_ui(permutations_given.get_mpz_t(), s);
            //     mpz_pow_ui(permutations_remaining.get_mpz_t(), S.get_mpz_t(), k - s);
            //     count = (count + permutations_given * permutations_remaining) % 1000000007;
            // }
            if(tuple.total == k && newbits <= n && isprime[newbits])
            {
                //tuple.writeverbose(std::cout) << std::endl;
                count = (count + tuple.get_permutations()) % BILL7;
            }
       
            if(tuple.total < k)
            {
                make(tuple, std::next(it), newbits, count, depth+1);
            }
            tuple.remove(val);
        }
    }

    mpz_class solve()
    {
        mpz_class retval = 0;
        counter<int> tuple;
        make(tuple, primes.begin(), 0, retval, 0);
        return retval;
    }
};


int main(int argc, char** argv)
{
    if(argc < 3) throw std::runtime_error("Usage: 734 n k");
    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    solver s(n, k);
    std::cout << s.solve() << std::endl;
    return 0;
}