// 266c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
#define MAX(x,y) (x > y ? x : y);

int main()
{
	mpz_class n = 1;
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	long primes[42];
	int pi = 0;
	int i = 0;
	for (mpz_class p = 2; p < 190; mpz_next_prime_candidate(p.get_mpz_t(), p.get_mpz_t(), randstate), i++)
	{
		n = n * p;
		primes[i] = p.get_ui();
	}

	mpz_class lim;
	mpz_ui_pow_ui(lim.get_mpz_t(), 2, 21);
	long uilim = lim.get_ui();
	set<mpz_class>* low = new set<mpz_class>(), *high = new set<mpz_class>();

	for (long i = 0; i < uilim; i++)
	{
		mpz_class flow(1), fhigh(1);
		long c = i;
		for (int b = 0; c > 0; b++, c >>= 1)
		{
			if ((c & 1) == 1)
			{
				mpz_mul_ui(flow.get_mpz_t(), flow.get_mpz_t(), primes[b]);
				mpz_mul_ui(fhigh.get_mpz_t(), fhigh.get_mpz_t(), primes[b+21]);
			}
		}
		low->insert(flow);
		high->insert(fhigh);
	}

	mpz_class nr;
	mpz_class nrplus = nr + 1;
	mpz_sqrt(nr.get_mpz_t(), n.get_mpz_t());
	mpz_class best(2);
	mpz_class m;
	mpz_ui_pow_ui(m.get_mpz_t(), 10, 16);
	long count = 0;
	for (auto l : *low)
	{
		if (count++ % 1000 == 0) cout << count << endl;
		
		mpz_class ll1 = nr / (2*l);
		mpz_class ll2 = best / l;
		mpz_class ll = MAX(ll1, ll2);
		for (set<mpz_class>::const_iterator h = high->lower_bound(ll); h != high->end(); h++)
		{
			mpz_class c = l * *h;
			if (c <= nr)
			{
				if (c > best)
				{
					best = c;
					cout << "Best = " << best << endl
						<< "        " << (best % m) << endl << endl;
				}
			}
			else break;
		}
	}

	cout << "finished" << endl;
	cout << "Best = " << best << endl
		<< "        " << (best % m) << endl << endl;

	delete low;
	delete high;

	return 0;
}

