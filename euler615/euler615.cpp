// euler615.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	long N;
	std::cin >> N;
	mpz_class f;
	mpz_mul_2exp(f.get_mpz_t(), mpz_class(1).get_mpz_t(), (N - 2));
	std::cout << f << std::endl;

	mpz_class g(4);
	
	long valsRequired = N;
	long valsObtained = 0;
	mpz_class val;
	while (valsObtained < valsRequired)
	{
		gmp_randstate_t randstate;
		gmp_randinit_default(randstate);
		val = f * g;
		if (++valsObtained % 100 == 0) std::cout << "\r" << valsObtained;
		g++;
		if (mpz_likely_prime_p(g.get_mpz_t(), randstate, 0))
		{
			g++;
		}
	}

	std::cout << "\n" << (val % 123454321) << std::endl;
	std::cin.get();
    return 0;
}

