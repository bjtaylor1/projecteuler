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
	static long long gcd(set<long long>::iterator begin, set<long long>::iterator end);



};

#endif