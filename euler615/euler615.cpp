// euler615.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

class factor
{
public:
	mpz_class const value;
	unsigned long const count;
	factor(): count(0) {}
	factor(const mpz_class& _value, const unsigned long& _count) : value(_value), count(_count) {}
};

class composite_factor
{
private:
	gmp_randstate_t randstate;

public:
	typedef pair<mpz_class, factor> factormember;
	composite_factor()
	{
		gmp_randinit_default(randstate);
	}
	~composite_factor()
	{
		gmp_randclear(randstate);
	}

	map<mpz_class, factor> factors;
	void add(const factor& f)
	{
		factors.insert(factormember(f.value, f));
	}

	void remove(const mpz_class& value)
	{
		auto f = factors[value];
		factors.erase(value);
		if (f.count > 1)
		{
			add(factor(value, f.count - 1));
		}
	}

	void increase()
	{
		mpz_class largestFactor = factors.rbegin()->first;
		mpz_class nextprime;
		mpz_next_prime_candidate(nextprime.get_mpz_t(), largestFactor.get_mpz_t(), randstate);
		remove(largestFactor);
		add(factor(nextprime, 1));
	}

	mpz_class get_total() const
	{
		mpz_class total;
		for (auto f : factors)
		{
			mpz_class component;
			mpz_pow_ui(component.get_mpz_t(), f.second.value.get_mpz_t(), f.second.count);
			total += component;
		}
		return total;
	}

};

bool operator<(const composite_factor& cf1, const composite_factor& cf2)
{
	auto cf1total = cf1.get_total();
	auto cf2total = cf2.get_total();
	return cf1total < cf2total;
}

set<composite_factor> progress(const vector<composite_factor>& cfs)
{
	set<composite_factor> retval;
	for (auto cf : cfs)
	{
		cf.increase();
		retval.insert(cf);
	}
	return retval;
}

int main()
{
	long N;
	std::cin >> N;
	composite_factor cf;
	cf.add(factor(2, N));
	vector<composite_factor> cfs;
	cfs.push_back(cf);
#if _DEBUG
	cout << cf.get_total() << endl;
#endif
	for (int found = 1; found < N; found++)
	{
		auto nextset = progress(cfs);
		cf = *nextset.begin();
		cfs.push_back(cf);
#if _DEBUG
		cout << cf.get_total() << endl;
#endif
	}

	cout << (cf.get_total() % 123454321) << endl;
	cin.get();
    return 0;
}

