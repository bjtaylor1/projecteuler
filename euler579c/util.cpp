#include "stdafx.h"
#include "util.h"

using namespace std;

biglong util::gcd_l(biglong a, biglong b)
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

biglong util::gcd(set<biglong>::iterator first, set<biglong>::iterator last)
{
	biglong l1 = *first;
	if (++first == last) return l1;
	else return gcd_l(abs(l1), abs(gcd(first, last)));
}

biglong util::gcd(const set<biglong> s)
{
	return gcd(s.begin(), s.end());
}