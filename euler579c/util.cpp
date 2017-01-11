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
	throw runtime_error("gcd not found");
}

long long util::gcd(const set<long long> s)
{
	return gcd(s.begin(), s.end());
}

