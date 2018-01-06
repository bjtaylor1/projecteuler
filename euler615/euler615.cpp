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
	composite_factor()	{}
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
	composite_factor cf1;
	composite_factor cf2 = cf1;

	mpz_class current(0);
	for (int found = 1; found < N; found++)
	{
		cfs.insert(composite_factor(factor(2, N + found-1)));

		cout << ".";
		set<composite_factor> expanded;
		for (auto cf : cfs) expanded.insert(cf.expand());
		cout << ".";

		cfs.insert(expanded.begin(), expanded.end());
		cout << ".";

		set<mpz_class> vals;
		mpz_class new_current(0);
		for (auto cf : cfs)
		{
			auto n = cf.get_total();
			if (n > current && (new_current == 0 || n < new_current))
			{
				new_current = n;
			}
		}
		current = new_current;

		cout << "." << found << endl;

	}

	cout << (current % 123454321) << endl;
	cin.get();
    return 0;
}

