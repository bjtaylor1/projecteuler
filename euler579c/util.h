#pragma once

#ifndef CLASS_UTIL
#define CLASS_UTIL

using namespace std;
template<class TInt>
class util
{
public:
	static TInt gcd_l(TInt a, TInt b)
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

	static TInt gcd(TInt x, TInt y, TInt z)
	{
		return gcd_l(x, gcd_l(y, z));
	}

	template<class TIterator>
	static TInt gcd(TIterator first, TIterator last)
	{
		TInt l1 = *first;
		if (++first == last) return l1;
		else return gcd_l(abs(l1), abs(gcd(first, last)));
	}

	static TInt gcd(const set<TInt>& s)
	{
		return gcd(s.begin(), s.end());
	}


};

#endif