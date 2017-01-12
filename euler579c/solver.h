#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER
#include "mnpq.h"
#include "macros.h"

using namespace std;

class solver
{
public:
	static BIGINT C, S;
	static long long M;
	static long long maxSide, maxResultDigits;

	static void solve();
	
	static set<vector<int>> keys;
	static set<cube> cubes_done;
	static set<int> cubeCounts;

	static void process_mnpq(const mnpq& mnpq);
};




#endif