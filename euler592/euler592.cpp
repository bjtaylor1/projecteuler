// euler592.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
int main(int argc, char** argv)
{
	mpz_w f(1);
	mpz_w max(0xffffffffffff);
	long long tot = stoll(argv[1]);
	double progdiv = ((double)tot) / 100;

	for (long long l = 1; l <= tot; l++)
	{
		if ((l % 10000000) == 0) cout << "\r" << setprecision(1) << (((double)l) / progdiv) << "% : " << f.val;
		f *= l;

		auto z = mpz_scan1(f.val, 0);
		if (z >= 4)
		{
			z -= (z % 4);
			mpz_tdiv_q_2exp(f.val, f.val, z);
		}

	}
	mpz_and(f.val, f.val, max.val);
	cout << hex << uppercase << f.val << endl;

	return 0;
}

