#include "stdafx.h"
#include "util.h"

using namespace std;

long long util::gcd_l(long long a, long long b)
{
	for (;;)
	{
		if (a == 0)
			return b;
		b %= a;
		if (b == 0)
			return a;
		a %= b;
	}
	throw exception("gcd not found");
}

long long util::gcd(set<long long>::iterator first, set<long long>::iterator last)
{
	long l1 = *first;
	if (++first == last) return l1;
	else return gcd_l(abs(l1), abs(gcd(first, last)));
}

long long util::gcd(const set<long long> s)
{
	return gcd(s.begin(), s.end());
}

