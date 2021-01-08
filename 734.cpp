#include <iostream>
#include <mpirxx.h>
#include <set>
#include <vector>
#include "primes.h"
#include "outputhelpers.h"
#include "counter.h"
#include "constants.h"

inline int twotothe(int n) { return n == 0 ? 1 : 2<<(n-1); }

std::ostream& operator<<(std::ostream& os, const std::set<int>& s)
{
    int i = 0;
    for(std::set<int>::const_iterator it = s.begin(); it != s.end(); it++)
    {
        os << " " << (*it) << " ";
    }
    return os;
}

class fit //list of primes that 'fit into' each prime
{
public:
    int p;
    std::set<int> primes; //that fit into p. But not including p itself

    void init(const int& _p){p = _p; }
};

template<typename T>
std::set<std::set<T> > setcombinations(const std::set<T>& items)
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
};

void add_combinations_with(std::set<std::set<int> >& combinations, const fit& f, const std::set<int>& defaultcombination)
{
    if(f.primes.size() > 0)
    {
        std::set<std::set<int> > combs = setcombinations(f.primes);
        for(std::set<std::set<int> >::const_iterator it = combs.begin(); it != combs.end(); it++)
        {
            std::set<int> comb = *it;
            comb.insert(defaultcombination.begin(), defaultcombination.end());
            combinations.insert(comb);
        }
    }
}

std::set<std::set<int> > combine(const fit& f1, const fit& f2)
{
    std::set<std::set<int> > combinations;
    std::set<int> defaultcombination;
    defaultcombination.insert(f1.p);
    defaultcombination.insert(f2.p);
    combinations.insert(defaultcombination);

    add_combinations_with(combinations, f1, defaultcombination);
    add_combinations_with(combinations, f2, defaultcombination);
    return combinations;
}

inline int distinct_combinations_calc(const int& a, const int& b)
{
    return twotothe(a) + twotothe(b) - 1;
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
                            int toadd = distinct_combinations(f[x], f[y]);

                            d[pc] += toadd;
                            auto newdistincts = combine(fD[x], fD[y]);
                            if(toadd != newdistincts.size())
                            {
                                std::cerr << "Discrepancy with " << x << "|" << y << " = " << pc << ": " << toadd << " vs " << newdistincts.size() << 
                                    " f[x]=" << f[x] << ", f[y]=" << f[y] << std::endl;
                                for(auto nd : newdistincts)
                                {
                                    std::cout << "  " << nd << std::endl;
                                }
                                throw std::runtime_error("Failed");
                            }
                            dD[pc].sets.insert(newdistincts.begin(), newdistincts.end());
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

        std::cout << std::endl;
        for(auto s : dD[n].sets)
        {
            std::cout << "D: " << s << std::endl;
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
    s.solve();
    return 0;
}