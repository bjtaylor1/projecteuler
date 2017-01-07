#pragma once
#ifndef CLASS_SOLVER
#define CLASS_SOLVER

class solver
{
public:
	long maxSide;
	solver(long _maxSide) : maxSide(_maxSide) {}
	void solve();
};


#endif