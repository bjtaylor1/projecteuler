// euler41.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

int main()
{
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	char p[] = "987654321";
	for (int len = 9; len > 0; len--)
	{
		char* begin = p + 9 - len;
		do {
			mpz_class tot(begin);
			if (mpz_probable_prime_p(tot.get_mpz_t(), randstate, 10, 0))
			{
				cout << tot << endl;
				return 0;
			}
		} while (prev_permutation(begin, begin + len));
	}
	return 0;
}

