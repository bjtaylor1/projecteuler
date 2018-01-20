// euler97.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
int main()
{
	mpz_class op(28433);
	mpz_class res, resmod;
	mpz_mul_2exp(res.get_mpz_t(), op.get_mpz_t(), 7830457);
	mpz_mod_ui(resmod.get_mpz_t(), res.get_mpz_t(), 10000000000);
	resmod += 1;
	cout << resmod << endl;
    return 0;
}

