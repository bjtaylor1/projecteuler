// 78.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

int main()
{
	long limit = 60000;
	long best = limit;
	long m = 1e6;
	mpz_class* w = new mpz_class[limit + 1];
	for (long i = 0; i <= limit; i++)
	{
		w[i] = mpz_class(0);
	}
	w[0] = 1;
	cout << "Created memory" << endl;
	for (long i = 1; i <= limit; i++)
	{
		if (i % 10 == 0) cout << i << endl;
		for (long j = i; j <= limit; j++)
			w[j] += w[j - i];

		if (w[i] % m == 0)
		{
			cout << "w[" << i << "]=" << w[i] << endl;
			break;
		}
	}
	delete[] w;
    return 0;
}

