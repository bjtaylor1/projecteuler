#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER
#include "mnpq.h"
#include "macros.h"

using namespace std;

class solver
{
public:
	static BIGINT /*C, */ S;
	static long long maxSide, maxResultDigits, numThreads;

	static void solve();
	
	static set<cube> cubes_done;

	static void process_mnpq(const mnpq& mnpq);
};




#endif