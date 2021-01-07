#include <iostream>
#include <mpirxx.h>
#include <set>
#include <vector>
#include "primes.h"
#include "outputhelpers.h"
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

    void make(std::set<int>& tuple, const std::set<int>::const_iterator first, int bits)
    {
        for(std::set<int>::const_iterator it = first; it != primes.end(); it++)
        {
            int val = *it;
            int newbits = bits | val;

            tuple.insert(val);

            if(newbits == n)
            {
                for(auto t : tuple) 
                {
                    std::cout << " " << t << " ";
                }
                std::cout << std::endl;
            }
       
            if(tuple.size() < k)
            {
                make(tuple, std::next(it), newbits);
            }
            tuple.erase(val);
        }
    }

    mpz_class solve()
    {
        std::set<int> tuple;
        make(tuple, primes.begin(), 0);
        return mpz_class(0);
    }
};

int main(int argc, char** argv)
{
    if(argc < 2) throw std::runtime_error("Usage: primebits n");
    int n = std::stoi(argv[1]);
    solver s(n, n);
    s.solve();
    return 0;
}