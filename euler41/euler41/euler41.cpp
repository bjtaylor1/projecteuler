// euler41.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

int main()
{
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);

	int p[] = { 9,8,7,6,5,4,3,2,1 };
	mpz_class allexp[] = { 1e8, 1e7, 1e6, 1e5, 1e4, 1e3, 1e2, 1e1, 1 };

	for (int len = 9; len > 0; len--)
	{
		int* begin = p + 9 - len;
		mpz_class* exp = allexp + 9 - len;
		do {
			auto last = begin[len - 1];

			if (last != 2 && last != 4 && last != 5 && last != 6 && last != 8)
			{
				mpz_class tot = accumulate(begin, begin + len, mpz_class(0),
					[begin, exp, i = 0](const mpz_class& st, int a) mutable->mpz_class
				{
					auto stnew = st + begin[i] * exp[i];
					i++;
					return stnew;
				});

				if (mpz_probable_prime_p(tot.get_mpz_t(), randstate, 10, 0))
				{
					cout << tot << endl;
					return 0;
				}
			}
		} while (prev_permutation(begin, begin + len));
	}
	return 0;
}

