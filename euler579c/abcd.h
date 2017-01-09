#pragma once
#ifndef CLASS_ABCD
#include "vector3d.h"

using namespace std;

class abcd
{
public:
	long long a, b, c, d;
	abcd(long long A, long long B, long long C, long long D) : a(A), b(B), c(C), d(D) {}
	set<vector3d> get_vectors() const;
};

ostream& operator<<(ostream&, const abcd&);
#endif