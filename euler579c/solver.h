#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER
#include "mnpq.h"

using namespace std;

class solver
{
public:
	unsigned long long C, S;
	unsigned long long maxSide; //only for casting purposes (hopefully!)
	solver(long _maxSide) : maxSide(_maxSide), C(0), S(0) {}
	void solve();
	void process_mnpq(mnpq& mnpq);
};


#endif