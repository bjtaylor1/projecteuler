// euler615.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
extern int primes[];

void combine(int N, int numFactors, int depth, mpz_class& current, set<mpz_class>& found)
{
	if (depth > N) return;
	for (int i = 0; ; i++)
	{
		auto prime = primes[N - depth];
		mpz_class primepower;
		mpz_ui_pow_ui(primepower.get_mpz_t(), prime, i);
		current *= primepower;
		if (found.size() > N && *found.rbegin() < current)
		{
			//definitely oversize - prune
			current /= primepower;
			return;
		}

		int newNumFactors = numFactors + i;
		if (newNumFactors >= N)
		{
			found.insert(current);
		}
		if (found.size() > N + 1)
		{
			found.erase(next(found.begin(), N + 1), found.end()); //prune it to N+1, so we only have one that is definitely oversize, i.e. the smallest definitely-oversize one will be rbegin
		}
		combine(N, newNumFactors, depth + 1, current, found);
		current /= primepower;
	}
}

int main()
{
	int N;
	cin >> N;
	set<mpz_class> found;
	mpz_class current(1);
	combine(N, 0, 1, current, found);
	auto result = next(found.begin(), N - 1);
	cout << (*result /*% 123454321*/) << endl;
	cin.get();
	return 0;
}

