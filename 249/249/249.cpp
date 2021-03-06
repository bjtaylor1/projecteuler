// 249.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
#define MIN(x,y) (x < y ? x : y)
#define LIMIT 5000
int main()
{
	unsigned long long mod = 1e16;
	set<long> primes;
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	long tot = 0;
	mpz_class p;
	for (p = 2; p < LIMIT; mpz_next_prime_candidate(p.get_mpz_t(), p.get_mpz_t(), randstate))
	{
		long pui = p.get_ui();
		primes.insert(pui);
		tot += pui;
	}
	while (p <= tot)
	{
		primes.insert(p.get_ui());
		mpz_next_prime_candidate(p.get_mpz_t(), p.get_mpz_t(), randstate);
	}
	mpz_class** w = new mpz_class*[tot+1];
	memset(w, 0, (tot+1) * sizeof(mpz_class*));
	w[0] = new mpz_class(1);
	long max = 0;
	cout << *primes.rbegin() << endl;
	int i = 0;
	for (auto p : primes)
	{
		if (p > LIMIT) break;
		if(i++ % 100 == 0) cout << p << endl;
		for (long j = p+max; j >= p; j--)
		{
			if (w[j] == NULL) w[j] = new mpz_class(0);
			if (w[j - p] == NULL) w[j - p] = new mpz_class(0);
			*w[j] += (*w[j - p]);
		}
		max += p;
		//cout << "p = " << p << ", max is now " << max << endl;
	}
	mpz_class result = 0;
	for (auto p : primes)
	{
		if (w[p] != NULL)
		{
			auto wt = *w[p];
			//cout << "w[" << p << "] = " << *w[p] << endl;
			result = (result + wt) % mod;
		}
	}
	delete[] w;
	cout << (result % 1e16) << endl;
    return 0;
}

