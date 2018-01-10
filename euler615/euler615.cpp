// euler615.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
extern int primes[];

class fraction
{
public:
	mpz_class top, bottom;
	mpz_class q, r;
	fraction(const mpz_class& _top, const mpz_class& _bottom) : top(_top), bottom(_bottom)
	{
		mpz_tdiv_qr(q.get_mpz_t(), r.get_mpz_t(), top.get_mpz_t(), bottom.get_mpz_t());
	}
};

bool operator<(const fraction& f1, const fraction& f2)
{
	if (f1.q != f2.q) return f1.q < f2.q;
	if (f1.r != f2.r) return f1.r < f2.r;
	//if they have the same quotient and remainder, the one with the *highest* top is smaller
	//e.g. 9/4 < 5/2 (both have q=2, r=1)
	return f2.top < f1.top;
}



int main()
{
	long N;
	std::cin >> N;
	set<fraction> fractions;
	bool s0end = false, s1end = false;
	int s0 = 0, s1 = 0;
	int stage = 0;

	for (int i = 0; i < N / 2 + N % 2; i++)
	{
		fractions.insert(fraction(primes[i], 1));
		fractions.insert(fraction(primes[i], 2));
	}
	for (int powerb = 2, insertedb = true; insertedb; powerb++)
	{
		insertedb = false;
		for (int powert = powerb, insertedt = true; insertedt; powert++)
		{
			//if even the first one of this power is too big, don't bother doing any higher powers.
			insertedt = false;
			for (int i = 1; i < N; i++)
			{
				mpz_class top, bottom(2);
				mpz_ui_pow_ui(top.get_mpz_t(), primes[i], powert);
				mpz_ui_pow_ui(bottom.get_mpz_t(), 2, powerb);
				fraction f(top, bottom);
				auto nth = next(fractions.begin(), N - 1);
				if (f < *nth) //if it's too big, don't bother carrying on.
				{
					fractions.insert(f);
					insertedt = true; //this one first into the first N, so at least try some higher powers.
					insertedb = true;
				}
				else
				{
					break;
				}
			}
		}
	}
	auto last = next(fractions.begin(), N - 1);
	mpz_class first(2);
	first <<= (N - 1);
	mpz_class result;
	mpz_mul(result.get_mpz_t(), first.get_mpz_t(), last->top.get_mpz_t());
	mpz_class resultq;
	mpz_tdiv_qr(result.get_mpz_t(), resultq.get_mpz_t(), result.get_mpz_t(), last->bottom.get_mpz_t());
	if (resultq != 0) cout << "Warning: non-exact division, q = " << resultq << endl;
	cout << (result % 123454321) << endl;

	cin.get();
	return 0;
}

