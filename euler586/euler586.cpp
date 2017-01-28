// euler586.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mpz_w.h"
#include "euler586.h"

using namespace std;

#define N 100000
#define R 4
long long K[N + 1];

mpz_w As[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };

long long factor2(long long nminus1, long long& twotother, long long& r, long long& d)
{
	return 0;
}


bool is_prime(mpz_w n)
{
	if (n <= 3 || n.is_even()) return true;
	mpz_w d, nminus1 = n - mpz_w::one;
	long long s = mpz_scan1(nminus1.val, 0); //n = 2^s.d
	mpz_tdiv_q_2exp(d.val, nminus1.val, s);
	for (mpz_w a : As)
	{
		mpz_w atothedmodn;
		mpz_powm(atothedmodn.val, a.val, d.val, n.val);
		if (atothedmodn != mpz_w::one)
		{
			for (long long r = 0; r < s; r++)
			{
				mpz_w dtwotother;
				mpz_mul_2exp(dtwotother.val, d.val, r);
				mpz_w atothedtwotother;
				mpz_powm(atothedtwotother.val, a.val, dtwotother.val, n.val);
				if (++atothedtwotother != n)
				{
					return false;
				}
			}
		}
	}
	return true;
}

int main()
{
	mpz_w t1 = 42, t2 = 5;
	mpz_w tplus1 = ++t1;

	gmp_printf("%Zd", t1.val);
	return 0;
	memset(K, 0, N * sizeof(long long));

	for (long long a = 1; a <= N; a++)
	{
		for (long long b = a + 1; b <= N; b++)
		{
			long long res = a*a + 3 * a*b + b*b;
			if (res == 9889) cout << a << "," << b << endl;
			if (res > N)
				break;
			else
				K[res]++;
		}
	}
	long long tot = 0;
	for (long long ki = 0; ki <= N; ki++)
	{
		if (K[ki] == R)
		{
			cout << ki << ",";
			tot++;
		}
	}
	cout << endl << endl << tot << endl;
	return 0;
}
