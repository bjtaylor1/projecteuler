#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER
#include "mnpq.h"
#include "macros.h"

#include "overlong.h"

using namespace std;

class solver
{
public:
	overlong C, S;
	long long M;
	overlong maxSide;
	solver(long long _maxSide) : maxSide(_maxSide), C(0), S(0) { }
	void solve();
	void process_mnpq(mnpq& mnpq);
};


#endif