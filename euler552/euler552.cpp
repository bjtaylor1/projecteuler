// euler552.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//const long long MAX = 0x7fffffff;
//const long long MAXN = MAX * 64;
const long long MAXN = 67108863;
const long MAX = MAXN / 64;

#define NUMPRIMES 300001

using namespace std;

long long primes[NUMPRIMES];
long long a__seq[NUMPRIMES];
long long s__seq[NUMPRIMES];

void set_bit(long long* b, long long bitOne)
{
	long long bit = bitOne;
	long long index = bit / 64;
	long long rem = bit % 64;
	if (index >= MAXN) throw exception();
	b[index] |= ((long long)1) << rem;
}

bool get_bit(long long* b, long long bitOne)
{
	long long bit = bitOne;
	long long index = bit / 64;
	long long rem = bit % 64;
	bool isset = ((b[index] >> rem) & 1) > 0;
	return isset;
}

bool create_primes(long long* b, int numRequired)
{
	memset(b, 0x5555555555555555, sizeof(long long) * MAX);
	b[0] = 0x5555555555555550;
	memset(primes, 0, sizeof(long long) * NUMPRIMES);
	primes[0] = 1;
	//return true;
	int numFound = 0;
	for (int n = 2; n < MAXN; n++)
	{
		if (!get_bit(b, n))
		{
			primes[++numFound] = n;
			if (numFound == numRequired) return true;

			for (int m = 2 * n; m < MAXN; m += n)
			{
				set_bit(b, m);
			}

		}
	}
	return false;

}

long long get_n(const long long n)
{
	long long x = -1;
	for (long long i = 1; i <= n; i++)
	{
		if (n % primes[i] == i) x = i;
		else break;
	}
	return x;
}

int main()
{
	memset(primes, 0, sizeof(long long) * NUMPRIMES);
	memset(a__seq, 0, sizeof(long long) * NUMPRIMES);
	memset(s__seq, 0, sizeof(long long) * NUMPRIMES);
	long long* b = new long long[MAXN];
	
	bool success = create_primes(b, NUMPRIMES);
	delete[] b;

	for ( long long i = 0; a__seq[NUMPRIMES] == 0; i++)
	{
		long long n = get_n(i);
		if (n != -1)
		{
			if (a__seq[n] == 0)
			{
				a__seq[n] = i;
			}
		}
	}

	cout << "success = " << success << endl;
	cout << (1) << ": " << primes[1] << endl;
	cout << "primes " << (NUMPRIMES - 1) << ": " << primes[NUMPRIMES - 1] << endl;
	cout << "a__seq " << (NUMPRIMES - 1) << ": " << a__seq[NUMPRIMES - 1] << endl;


	return 0;
}

