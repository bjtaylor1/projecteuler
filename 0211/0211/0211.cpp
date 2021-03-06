#include "stdafx.h"

#define N 10
using namespace std;
typedef unsigned long long ull;
set<mpz_class> primes;
set<mpz_class> primepowers;
mpz_class sum_factor_squares[N];


int* pf;
set<int> pfs[N];

template<class T, class TN>
class incrementing_set
{
public:
	map<T, TN> m;
	TN insert(const T& item)
	{
		if (m.find(item) == m.end())
			m[item] = 0;
		return ++m[item];
	}

	TN remove(const T& item)
	{
		auto existing = m.find(item);
		TN newcount = existing->second - 1;
		if (newcount == 0)
			m.erase(item);
		else m[item] = newcount;

		return newcount;
	}
};


bool is_perfect_square(const ull& n)
{
	auto root = (ull)sqrt(n);
	return root * root == n;
}

bool do_it(const mpz_class& pp1, const mpz_class& product, const mpz_class& spp, long depth, set<ull>& f)
{
	ull produi = product.get_ui();

	bool retval = false;

	if (produi <= N)
	{
		sum_factor_squares[produi] += spp;
		for (set<mpz_class>::const_iterator pp2 = primepowers.lower_bound(pp1 + 1); pp2 != primepowers.end(); pp2++)
		{
			f.insert(pp2->get_ui());
			auto res = do_it(*pp2, product * *pp2, spp + (*pp2 * *pp2), depth + 1, f);
			f.erase(pp2->get_ui());
			if (!res)
			{
				break;
			}
		}
		retval = true;
	}
	return retval;
}

gmp_randstate_t randstate;
void make_prime_powers()
{
	for (mpz_class p(2); p <= N;)
	{
		for (mpz_class pp = p; pp <= N; pp *= p)
		{
			primepowers.insert(pp);
		}
		mpz_next_prime_candidate(p.get_mpz_t(), p.get_mpz_t(), randstate);
	}
}


#define FL 10 //factor limit
#define PFSIZE ((N + 1)*FL)

class factors
{
public:
	int of;
	int count;
	int items[FL];
	factors() : of(0), count(0) {}
	factors(int _of) : of(_of), count(0){	}
};

struct CompareLess
{
	bool operator () (const int & a, const int & b) const
	{
		return a<b;
	}
	static int max_value()
	{
		return std::numeric_limits<int>::max();
	}
};

struct HashFunctor
{
	size_t operator () (int key) const
	{
		// a simple integer hash function
		return (size_t)(key * 2654435761u);
	}
};
#define SUB_BLOCK_SIZE 8192
#define SUB_BLOCKS_PER_BLOCK 256
typedef stxxl::unordered_map<
	int, factors, HashFunctor, CompareLess, SUB_BLOCK_SIZE, SUB_BLOCKS_PER_BLOCK
> unordered_map_type;

int main()
{
	gmp_randinit_default(randstate);

	cout << "Allocating...";
	unordered_map_type fs;
	cout << " done." << endl;
	for (mpz_class p(2); p <= N;)
	{
		cout << p << endl;
		int pui = p.get_ui();
		for (int pp = pui; pp <= N; pp *= pui)
		{
			for (int ppm = pp; ppm <= N; ppm += pp)
			{
				auto inserted = fs.insert(pair<int, factors>(ppm, factors(ppm)));
				factors& f = inserted.first->second;
				f.items[f.count++] = pp;
				
				cout << endl;
				for (auto item : fs)
				{
					cout << "Factors of " << item.first << ": ";
					for (int i = 0; i < item.second.count; i++)
					{
						cout << item.second.items[i] << " ";
					}
					cout << endl;
				}
				//f[ppm].items[f[ppm].count++] = pp;
				//auto pos = ppm * FL;
				//auto fi = ++pf[pos];//factor index (current count + 1)
				//if (fi >= FL) throw runtime_error("Breached factor limit!");
				//pf[pos + fi] = pp;
			}
		}
		mpz_next_prime_candidate(p.get_mpz_t(), p.get_mpz_t(), randstate);
	}
	cout << "Freeing...";
	//delete[] pf;
	cout << "done." << endl;
    return 0;
}

