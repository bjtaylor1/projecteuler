#pragma once

#ifndef CLASS_UTIL
#define CLASS_UTIL

#include "macros.h"

using namespace std;
class util
{
public:
	static long gcd_l(long, long);
	static long gcd(const set<long>);
	static long gcd(set<long>::iterator begin, set<long>::iterator end);



};

#endif