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
    cache<int, long> tcache;

    void makefit()
    {
        for(int pc = n; pc >= 2; pc--)
        {
            if((pc % 1000) == 0) std::cout << pc << "..." << std::endl;

            if((pc & 1) == 0)
            {
                // it's even, 2 is the only one that'll fit into it
                f[pc] = (pc & 2) != 0 ? 1 : 0;
            }
            else
            {                
                for(std::set<int>::const_iterator it = primes.begin(); it != primes.end() && (*it)<=pc; it++)
                {
                    int p = *it;
                    if((pc|p) == pc)
                    {
                        f[pc]++;
                    }
                }
            }
        }
        std::cout << std::endl;
    }

    long get_t(const int& p)
    {
        return tcache.get_or_add(p, [this](const int& _p){return this->calc_t(_p);} );
    }
    long calc_t(const int& p)
    {
        mpz_class res;
        mpz_ui_pow_ui(res.get_mpz_t(), f[p], k);
        for(int l = 2; l < p; l++)
        {
            if( ((p|l)  == p ) && f[l] > 0)
            {
                long lt= get_t(l);
                res -= lt;
            }
        }
        res %= BILL7;
        return res.get_ui();
    }

    void solve()
    {
        primes = makeprimes(n+1, prime);
        std::cout << "made primes" << std::endl;
        makefit();
        std::cout << "made fit" << std::endl;
        mpz_class total(0);
        for(std::set<int>::const_iterator it = primes.begin(); it != primes.end(); it++)
        {
            mpz_class t_this = get_t(*it);
            total = (total + t_this) % BILL7;
            std::cout << "p=" << (*it) << ", total = " << total << std::endl;
        }

        std::cout << std::endl << total << std::endl;
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