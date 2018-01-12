// euler146.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

gmp_randstate_t randstate;
using namespace std;

bool matches(mpz_class& p, const mpz_class& n)
{
	for (auto f : { 3,7,9,13,27 })
	{
		mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());
		if (n*n + f != p) return false;
	}
	return true;
}

bool is_pattern(int ui_n)
{
	mpz_class n(ui_n), p(n*n + 1);
	if (mpz_probable_prime_p(p.get_mpz_t(), randstate, 10, 0))
	{
		return matches(p, n);
	}
	return false;
}

int main()
{
	gmp_randinit_default(randstate);
	mpz_class p;
	int N;
	cin >> N;
	mpz_class n, r, t;
	bool gotnext = false;
	int count = 0;
	for (int n = 1; n < N; n++)
	{
		if (is_pattern(n))
		{
			t += n;
		}
	}
	cout << t << endl;
    return 0;
}

