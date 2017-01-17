#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER
#include "mnpq.h"
#include "macros.h"
#include "transformation.h"

using namespace std;

class solver
{
public:
	static BIGINT /*C, */ S;
	static long long maxSide, numThreads;
	static size_t maxResultDigits;
	static long long maxCount;

	static void solve();
	
	static set<cube> cubes_done;
	static void process_mnpq(const mnpq& mnpq);

	static transformation t_x[], t_y[], t_z[];

	static transformation make_x_transformation(double angleDegrees);
	static transformation make_y_transformation(double angleDegrees);
	static transformation make_z_transformation(double angleDegrees);
};




#endif