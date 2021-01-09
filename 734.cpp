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
    cache<int, mpz_class> tcache;

    void makefit()
    {
        // for(int small = 2; small < n; small++) if(prime[small])
        // {
        //     for(int big = small+1; big <= n; big++) if(prime[big])
        //     {
        //         if((big|small)==big) f[big]++;
        //     }
        // }

        for(int pc = 2; pc <= n; pc++)
        {
            std::cout << pc << ": ";
            for(std::set<int>::const_iterator it = primes.begin(); (*it)<=pc; it++)
            {
                int p = *it;
                if((pc|p) == pc)
                {
                    std::cout << " " << p;
                    f[pc]++;
                }
            }
            std::cout << " = " << f[pc] << std::endl;
        }
        std::cout << std::endl;
    }

    mpz_class get_t(const int& p)
    {
        return tcache.get_or_add(p, [this](const int& _p){return this->calc_t(_p);} );
    }
    mpz_class calc_t(const int& p)
    {
        mpz_class res;
        if(p==23) std::cout << "p=" << p << ":" << std::endl;
        mpz_ui_pow_ui(res.get_mpz_t(), f[p], k);
        if(p==23) std::cout << "  f[p]=" << f[p] << ", k=" << k << std::endl;
        for(int l = 2; l < p; l++)
        {
            if( ((p|l)  == p ) && f[l] > 0)
            {
                mpz_class lt= get_t(l);
                if(p == 23) std::cout << "  -= " << lt << " (from " << l << ")" << std::endl;
                res -= lt;
            }
        }
        // std::cout << std::endl;
        return res;
    }

    void solve()
    {
        primes = makeprimes(n+1, prime);
        makefit();
        mpz_class total(0);
        for(std::set<int>::const_iterator it = primes.begin(); it != primes.end(); it++)
        {
            mpz_class t_this = get_t(*it);
            std::cout << (*it) << ": " << t_this << std::endl;
            total = (total + t_this) % BILL7;
        }
        std::cout << total << std::endl;
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

int main(int argc, char** argv)
{
    if(argc < 3) throw std::runtime_error("Usage: 734 n k");
    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    solver s(n, k);
    s.solve();

    return 0;
}