#pragma once

#ifndef CLASS_UTIL
#define CLASS_UTIL

#include "macros.h"

using namespace std;
class util
{
public:
	static long long gcd_l(long long, long long);
	static long long gcd(const set<long long>);

	template<class TIterator>
	static long long gcd(TIterator first, TIterator last)
	{
		long l1 = *first;
		if (++first == last) return l1;
		else return gcd_l(abs(l1), abs(gcd(first, last)));
	}
	



};

#endif