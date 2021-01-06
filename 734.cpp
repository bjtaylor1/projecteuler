#include <iostream>
#include <mpirxx.h>
#include <set>
#include <vector>
#include "primes.h"
#include "outputhelpers.h"
#include "counter.h"
#include "constants.h"

inline int twotothe(int n) { return n == 0 ? 1 : 2<<(n-1); }

class solver
{ 
public:
    int n, k;
    bool* isprime;
    int* d;
    int* f; // number of primes that 'fit into' each higher prime (not including itself)
    
    std::set<int> primes;
    solver(int n, int k) :n(n), k(k)
    {
        isprime = new bool[n+1];
        d = new int[n+1]; // simply the number of ways of making each prime, regardless of whether they include the prime itself, e.g. 2|7=7, 2|5=7.
        f = new int[n+1];
        memset(d, 0, (n+1)*sizeof(int));
        memset(f, 0, (n+1)*sizeof(int));
        primes = makeprimes(n+1, isprime);
    }
    ~solver() { delete[] isprime; delete[] d; delete[] f; }

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
                for(auto t : tuple) std::cout << " " << t;
                std::cout << " ==" << newbits << std::endl;
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
        for(int x = 2; x <= n; x++) //if(isprime[x]) // for now - may need to 'relax' this to find all composites?
        {
            for(int y = x+1; y <= n; y++) //if(isprime[y])
            {
                int pc = x|y;
                if(pc <= n)
                {
                    std::cout << x<<"|"<<y<<"="<<pc << ": ";
                    if(pc == y)
                    {
                        if(isprime[x] && isprime[y])
                        {
                            // not distinct - found another one (x) that 'fits into' y
                            f[pc]++;
                            std::cout << "f[" << pc << "]++ = " << f[y] << std::endl;
                        }
                    }
                    else
                    {
                        // distinct
                        if( (isprime[x]||d[x]>0) && (isprime[y]||d[y] > 0))
                        {
                            int toadd = (f[x]+1)*(f[y]+1);
                            d[pc] += toadd;
                            std::cout << "d[" << pc << "]+= " << (f[x]+1) << "x" << (f[y]+1) << "=" << toadd << " = " << d[pc] << std::endl;
                        }
                    }
                    
                }
            }
        }

        std::cout << std::endl;
        for(std::set<int>::const_iterator it = primes.begin(); it != primes.end(); it++)
        {
            int p = *it;
            int sets = twotothe(f[p]) - 1; // i.e. 2^f[p] - 1  https://math.stackexchange.com/questions/161565/what-is-the-total-number-of-combinations-of-5-items-together-when-there-are-no-d
            int total = d[p] + sets + 1;
            std::cout << p << ": f=" << f[p] << ", sets=" << sets << ", d=" << d[p] << ", total = " << total << std::endl;
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