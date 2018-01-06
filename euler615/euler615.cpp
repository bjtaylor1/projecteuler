// euler615.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

class factor
{
public:
	mpz_class const value;
	mpz_class const count;
	factor() {}
	factor(const mpz_class& _value, const mpz_class& _count) : value(_value), count(_count) {}
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
		auto total = accumulate(factors.begin(), factors.end(), mpz_class(0),
			[](const mpz_class& tot, const factormember& f) {return tot + f.second.count * f.second.value; });
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

	for (int found = 0; found < N; found++)
	{

	}
		
    return 0;
}

