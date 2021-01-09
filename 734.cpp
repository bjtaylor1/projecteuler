#include <iostream>
#include <mpirxx.h>
#include <set>
#include <vector>
#include "primes.h"
#include "outputhelpers.h"
#include "counter.h"
#include "constants.h"
#include "cache.h"

class solver
{
public:
    int n, k;
    bool* prime;
    int* f; // number that 'fit into' each
    std::set<int> primes;
    mpz_class ans;

    void makefit()
    {
        for(int small = 2; small < n; small++) if(prime[small])
        {
            for(int big = small+1; big <= n; big++) if(prime[big])
            {
                if((big|small)==big) f[big]++;
            }
        }
    }

    void solve()
    {
        primes = makeprimes(n+1, prime);
        makefit();

    }

    solver(int _n, int _k) : n(_n), k(_k), ans(0)
    {
        prime = new bool[n+1];
        f = new int[n+1];
        memset(prime, 0, sizeof(bool)*(n+1));
        memset(f, 0, sizeof(int)*(n+1));
    }

    ~solver()
    {
        delete[] prime;
        delete[] f;
    }
};

int testcache(const int& i)
{
    return i*2;
}

int main(int argc, char** argv)
{
    if(argc < 3) throw std::runtime_error("Usage: 734 n k");
    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    solver s(n, k);
    s.solve();

    std::cout << std::endl;
    for(std::set<int>::const_iterator it = s.primes.begin(); it != s.primes.end(); it++)
    {
        int p = *it;
        std::cout << p << ", f=" << s.f[p] << std::endl;
    }

    return 0;
}