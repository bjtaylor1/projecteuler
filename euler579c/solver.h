#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER
#include "mnpq.h"
#include "macros.h"

using namespace std;

class solver
{
public:
	bigulong C, S, M;
	bigulong maxSide; //only for casting purposes (hopefully!)
	solver(biglong _maxSide) : maxSide(_maxSide), C(0), S(0), M(_maxSide == 5000 ? 1e9 : 0) {}
	void solve();
	void process_mnpq(mnpq& mnpq);
};


#endif