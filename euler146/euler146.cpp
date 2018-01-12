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

int main()
{
	mpz_class p;
	int N;
	cin >> N;
	mpz_class n, r, t;
	bool gotnext = false;
	int count = 0;
	do
	{
		if(!gotnext) mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());
		gotnext = false;
		mpz_class s = p - 1;
		mpz_sqrtrem(n.get_mpz_t(), r.get_mpz_t(), s.get_mpz_t());
		//if (r == 0)
		//{
		//	if (matches(p, n)) t += n;

		//	gotnext = true;
		//}
		if (count++ % 1000 == 0)
		{
			cout << n << "," << p << endl;
		}
	} while (n <= N);
	cout << t << endl;
    return 0;
}

