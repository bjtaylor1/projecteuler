#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER
#include "mnpq.h"

using namespace std;

class solver
{
public:
	long maxSide;
	solver(long _maxSide) : maxSide(_maxSide) {}
	void solve();
	void process_mnpq(const mnpq& mnpq);
};


#endif