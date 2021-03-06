// euler49.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
typedef tuple<mpz_class, mpz_class, mpz_class> tresult;

int main()
{
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	set<tresult> found;
	for (mpz_class p(1009); p < 10000; mpz_nextprime(p.get_mpz_t(), p.get_mpz_t()))
	{
		stringstream ss;
		ss << p;
		string ps = ss.str();
		sort(ps.begin(), ps.end());
		if (ps[0] == '0') continue;
		
		set<mpz_class> sequence;
		do
		{
			mpz_class r(ps);
			if (mpz_probable_prime_p(r.get_mpz_t(), randstate, 10, 0))
			{
				sequence.insert(r);
			}
		} while (next_permutation(ps.begin(), ps.end()));

		set<mpz_class> seq;
		for (auto it1 = sequence.begin(); it1 != sequence.end(); it1++)
		{
			for (auto it2 = it1; ++it2 != sequence.end();)
			{
				auto diff = *it2 - *it1;
				auto third = *it2 + diff;
				if (sequence.find(third) != sequence.end())
				{
					if(found.insert(tresult(*it1, *it2, third)).second)
						cout << *it1 << "," << *it2 << "," << (*it2 + diff) << endl;
				}
			}
		}
	}
    return 0;
}

