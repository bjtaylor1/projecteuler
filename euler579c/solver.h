#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER
#include "mnpq.h"
#include "macros.h"
#include "massiveinteger.h"

using namespace std;

class solver
{
public:
	massiveinteger C, S;
	long long M;
	long maxSide, maxResultDigits;
	solver(long _maxSide, long _maxResultDigits) : maxSide(_maxSide), maxResultDigits(_maxResultDigits), C(0), S(0) { }
	void solve();
	void process_mnpq(mnpq& mnpq);
};


#endif