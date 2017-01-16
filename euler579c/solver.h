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
	static long long maxSide, numThreads;
	static size_t maxResultDigits;

	static void solve();
	
	static set<cube> cubes_done;
	static set<set<vector3d>> triples;
	static void process_mnpq(const mnpq& mnpq);
};




#endif