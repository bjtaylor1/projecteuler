#pragma once
#ifndef CLASS_ABCD
#include "vector3d.h"

using namespace std;

class abcd
{
public:
	long a, b, c, d;
	abcd(long A, long B, long C, long D) : a(A), b(B), c(C), d(D) {}
	vector<vector3d> get_vectors() const;
};

#endif