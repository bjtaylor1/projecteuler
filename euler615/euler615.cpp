// euler615.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

class factor
{
public:
	mpz_class const value;
	unsigned long const count;
	factor() : count(0) {}
	factor(const mpz_class& _value, const unsigned long& _count) : value(_value), count(_count) {}

};

bool operator<(const factor& f1, const factor& f2)
{
	if (f1.value != f2.value) return f1.value < f2.value;
	if (f1.count != f2.count) return f1.count < f2.count;
	return false;
}

bool operator==(const factor& f1, const factor& f2)
{
	return f1.value == f2.value && f1.count == f2.count;
}

typedef pair<mpz_class, factor> factormember;
gmp_randstate_t randstate;

class composite_factor
{
public:

	composite_factor(const factor& f)
	{
		add(f);
	}
	composite_factor() {}
	composite_factor(const composite_factor& cf) : factors(cf.factors) {}
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

	composite_factor expand() const
	{
		composite_factor retval(*this);
		mpz_class largestFactor = factors.rbegin()->first;
		mpz_class nextprime;
		mpz_next_prime_candidate(nextprime.get_mpz_t(), largestFactor.get_mpz_t(), randstate);
		retval.remove(largestFactor);
		retval.add(factor(nextprime, 1));
		return retval;
	}

	set<composite_factor> get_expansions() const
	{
		set<composite_factor> expansions;
		for (auto f : factors)
		{
			mpz_class nextprime;
			mpz_next_prime_candidate(nextprime.get_mpz_t(), f.second.value.get_mpz_t(), randstate);
			composite_factor expansion(*this);
			expansion.remove(f.second.value);
			expansion.add(factor(nextprime, 1));
			expansions.insert(expansion);
		}


		return expansions;
	}

	mpz_class get_total() const
	{
		mpz_class total(1);
		for (auto f : factors)
		{
			mpz_class component;
			mpz_pow_ui(component.get_mpz_t(), f.second.value.get_mpz_t(), f.second.count);
			total *= component;
		}
		return total;
	}
};

ostream& operator<<(ostream& os, const composite_factor& cf)
{
	cout << cf.get_total() << "=";
	for (auto f : cf.factors)
	{
		for (unsigned long i = 0; i < f.second.count; i++)
		{
			cout << f.second.value << ".";
		}
	}
	cout << "\b ";
	return os;
}

bool operator<(const composite_factor& cf1, const composite_factor& cf2)
{
	if (cf1.factors.size() != cf2.factors.size())
	{
		return cf1.factors.size() < cf2.factors.size();
	}
	for (auto it1 = cf1.factors.begin(), it2 = cf2.factors.begin();
		it1 != cf1.factors.end(); it1++, it2++)
	{
		if (*it1 != *it2) return *it1 < *it2;
	}
	return false;
}

class test
{
public:
	test(int _n) : n(_n) {}
	int n;
};

int main()
{
	long N;
	std::cin >> N;
	gmp_randinit_default(randstate);
	set<composite_factor> cfs;
	composite_factor current(factor(2, N));
	cfs.insert(current);
	mpz_class currentvalue = current.get_total();

	for (int found = 1; found < N; found++)
	{
		set<composite_factor> allexpansions;
		for (auto cf : cfs)
		{
			auto expansions = cf.get_expansions();
			allexpansions.insert(expansions.begin(), expansions.end());
		}
		map<mpz_class, composite_factor> expansionsbyvalue;
		for (auto exp : allexpansions)
		{
			expansionsbyvalue.insert(pair<mpz_class, composite_factor>(exp.get_total(), exp));
		}

		int t = 0;
		mpz_class highest = expansionsbyvalue.rbegin()->first;
		do
		{
			composite_factor twos(factor(2, N + t++));
			auto total = twos.get_total();
			expansionsbyvalue.insert(pair<mpz_class, composite_factor>(total, twos));
			if (total > highest) break;
		} while (true);

		auto nextCurrent = expansionsbyvalue.lower_bound(currentvalue);
		while (nextCurrent->first <= currentvalue) nextCurrent++;

		current = nextCurrent->second;
		currentvalue = nextCurrent->first;
		cfs.insert(current);
#if _DEBUG
		cout << currentvalue << endl;
#endif

	}

	cout << (currentvalue % 123454321) << endl;
	cin.get();
	return 0;
}

