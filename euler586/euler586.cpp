// euler586.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mpir.h>

using namespace std;

#define N 100000
#define R 4
long long K[N + 1];

long long As[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };

long long factor2(long long nminus1, long long& twotother, long long& r, long long& d)
{
	return 0;
}

bool is_prime(long long n)
{

	if (n <= 3 || n % 2 == 0) return true;
	long long nminus1 = n - 1, twotother = 2, r = 1, d;
	while ((d = nminus1 / twotother) % 2 == 0) {
		r++;
		twotother << 1;
	}
	long long mina = 2 * (long long)ceil(log(n) * log(n));
	for (long long a : As)
	{

		if (a <= mina)
		{
			long long t1 = (long long)pow(a, d);
			if (t1 % n != 1)
			{
				
			}
		}
	}
}

int main()
{
	mpz_t bign1, bign2;
	mpz_init_set_ui(bign1, 42);
	mpz_init_set_ui(bign2, 5);
	mpz_t res;
	mpz_init(res);
	mpz_mul(res, bign1, bign2);
	gmp_printf("%Zd\n", res);

	memset(K, 0, N * sizeof(long long));

	for (long long a = 1; a <= N; a++)
	{
		for (long long b = a+1; b <= N; b++)
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

