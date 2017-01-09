#pragma once

#ifndef CLASS_UTIL
#define CLASS_UTIL

#include "macros.h"

using namespace std;
class util
{
public:
	static biglong gcd_l(biglong, biglong);
	static biglong gcd(const set<biglong>);
	static biglong gcd(set<biglong>::iterator begin, set<biglong>::iterator end);
};

#endif