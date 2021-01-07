#include <iostream>
#include <mpirxx.h>
#include <set>
#include <vector>
#include "primes.h"
#include "outputhelpers.h"
#include "counter.h"
#include "constants.h"

inline int twotothe(int n) { return n == 0 ? 1 : 2<<(n-1); }

class fit //list of primes that 'fit into' each prime
{
public:
    int p;
    std::set<int> primes;

    void init(const int& _p){p = _p; }
};

template<typename T>
std::set<std::set<T> > combinations(const std::set<T>& items)
{
    if(items.size() >=63)
    {
        std::cerr << "Cannot call this method with 63 items or more!" << std::endl;
        throw std::runtime_error("Failed.");
    }

    T* array = new T[items.size()];
    int i = 0;
    for(typename std::set<T>::const_iterator it = items.begin(); it != items.end(); it++)
    {
        array[i++] = *it;
    }

    std::set<std::set<T> > retval;
    for(int i = 1; i < (1<<items.size()); i++)
    {
        std::set<T> item;
        for(int b = 0; (1<<b)<=i; b++)
        {
            if((i&(1<<b)) != 0) item.insert(array[b]);
        }
        retval.insert(item);
    }
    delete[] array;

    return retval;
}

class distinct // list of ways of making a number other numbers, not including the number itself
{
public:
    int p;
    void init(const int& _p) { p = _p;}
    std::set<std::set<int> > sets;

    void add_combination(const fit& f1, const fit& f2)
    {

    }
};



inline int distinct_combinations_calc(const int& a, const int& b)
{
    if(a == 0 || b == 0) return 1;
    if(a == 1) return b;
    if(b == 1) return a;
    return twotothe(a) + twotothe(b) - 3;
    //i.e.
    //(2^a - 1) + (2^b - 1) - 1
    //= all the combinations of each one, with ALL of the other
    //minus 1 for counting them all twice
}

inline int distinct_combinations(const int& a, const int& b)
{
    int res = distinct_combinations_calc(a, b);
    //std::cout << std::endl << "  dc(" << a << "," << b << ") = " << res << std::endl;
    return res;
}

class solver
{ 
public:
    int n, k;
    bool* isprime;
    int* d;
    int* f; // number of primes that 'fit into' each higher prime (not including itself)
    fit* fD;
    distinct* dD;

    std::set<int> primes;
    solver(int n, int k) :n(n), k(k)
    {
        isprime = new bool[n+1];
        d = new int[n+1]; // simply the number of ways of making each prime, regardless of whether they include the prime itself, e.g. 2|7=7, 2|5=7.
        
        //dD = new distinct[n+1];

        f = new int[n+1]; // number of primes that 'fit into' each prime. NOT including the prime itself. (for reasons associated with avoiding double counting)
        memset(d, 0, (n+1)*sizeof(int));
        memset(f, 0, (n+1)*sizeof(int));
        primes = makeprimes(n+1, isprime);

        fD = new fit[n+1];
        dD = new distinct[n+1];
        for(int p = 2; p<=n; p++)
        {
            f[p]=0;
            fD[p].init(p);
            dD[p].init(p);
        }
    }
    ~solver()
    {
        delete[] isprime;
        delete[] d;
        delete[] f;
        delete[] fD;
        delete[] dD;
    }

    mpz_class solve()
    {
        mpz_class retval = 0;

        // calculate the number of primes that 'fit into' each prime
        for(int x = 2; x <= n; x++) //if(isprime[x]) // for now - may need to 'relax' this to find all composites?
        {
            for(int y = x+1; y <= n; y++) //if(isprime[y])
            {
                int pc = x|y;
                if(pc <= n)
                {
                    if(pc == y)
                    {
                        if(isprime[x] && isprime[y])
                        {
                            // not distinct - found another one (x) that 'fits into' y
                            f[pc]++;
                            fD[pc].primes.insert(x);

                            if(fD[pc].primes.size() != f[pc])
                            {
                                std::cerr << "fD != f - consistency check failed!" << std::endl;
                                throw std::runtime_error("Failed");
                            }

                            std::cout << x<<"|"<<y<<"="<<pc << ": " << "f[" << pc << "]++ = " << f[y] << std::endl;
                        }
                    }
                    else
                    {
                        // distinct
                        if( (isprime[x]||d[x]>0) && (isprime[y]||d[y] > 0))
                        {
                            int toadd = distinct_combinations(f[x]+1, f[y]+1);

                            d[pc] += toadd;

                            std::cout << x<<"|"<<y<<"="<<pc << ": " << "d[" << pc << "]+= " << (f[x]+1) << "x" << (f[y]+1) << "=" << toadd << " = " << d[pc] << std::endl;
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
    std::set<int> test { 1,2,3,4,5};
    std::set<std::set<int> > combs = combinations<int>(test);
    for(auto comb : combs)
    {
        for(auto i : comb) std::cout << " " << i << " ";
        std::cout << std::endl;
    }

    // fit* sets = new fit[2];
    // sets[0].primes.insert(1);
    // sets[1].primes.insert(2);
    // for(int i = 0; i < 2; i++)
    // {
    //     for(std::set<int>::const_iterator it = sets[i].primes.begin(); it != sets[i].primes.end(); it++) std::cout << (*it);

    //     std::cout << std::endl;
    // }
    // delete[] sets;

    if(argc < 3) throw std::runtime_error("Usage: 734 n k");
    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    solver s(n, k);
    std::cout << s.solve() << std::endl;

    //
    // int n = std::stoi(argv[1]);
    // bool* isprime = new bool[n+1];
    // std::set<int> primes = makeprimes(n+1, isprime);
    // std::cout << primes.size() << std::endl;
    // mpz_class t(0);

    return 0;
}