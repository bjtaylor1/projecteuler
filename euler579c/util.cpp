#include "stdafx.h"
#include "util.h"

using namespace std;

long util::gcd_l(long a, long b)
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

long util::gcd(set<long>::iterator first, set<long>::iterator last)
{
	long l1 = *first;
	if (++first == last) return l1;
	else return gcd_l(l1, gcd(first, last));
}

long util::gcd(const set<long> s)
{
	return gcd(s.begin(), s.end());
}