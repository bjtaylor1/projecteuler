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
	unsigned long C, S, M;
	overlong maxSide;
	solver(unsigned long _maxSide) : maxSide(_maxSide), C(0), S(0)
	{
		if (_maxSide >= 5000)
		{
			overlong::MAX = 1e9;
		}
		else overlong::MAX = 0;
	}
	void solve();
	void process_mnpq(mnpq& mnpq);
};


#endif