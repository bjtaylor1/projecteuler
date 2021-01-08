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
    bool* prime;
    bool* composite;
    int* f; // number that 'fit into' each

    inline int isporc(int i) { return prime[i] || composite[i];}

    void makecomposites()
    {
        for(int x = 2; x <= n; x++) if(isporc(x))
        {
            for (int y = x+1; y <= n; y++) if(isporc(y))
            {
                if(!prime[x|y])
                {
                    std::cout << x << "|" << y << "=" << (x|y) << std::endl;
                    composite[x|y] = true;
                }
            }
        }
    }

    solver(int _n, int _k) : n(_n), k(_k)
    {
        prime = new bool[n+1];
        composite = new bool[n+1];
        memset(prime, 0, sizeof(bool)*(n+1));
        memset(composite, 0, sizeof(bool)*(n+1));
        primeseive(n+1, prime);
        makecomposites();
    }

    ~solver()
    {
        delete[] prime;
        delete[] composite;
    }
};

int main(int argc, char** argv)
{
    if(argc < 3) throw std::runtime_error("Usage: 734 n k");
    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    solver s(n, k);
    //s.solve();

    std::cout << std::endl;
    for(int i = 2; i <= n; i++)
    {
        if(s.prime[i]) std::cout << "P: " << i << std::endl;
        else if(s.composite[i]) std::cout << "C: " << i << std::endl;
    }

    return 0;
}