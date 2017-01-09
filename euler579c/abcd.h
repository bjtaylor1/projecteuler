#pragma once
#ifndef CLASS_ABCD
#include "vector3d.h"

using namespace std;

class abcd
{
public:
	biglong a, b, c, d;
	abcd(biglong A, biglong B, biglong C, biglong D) : a(A), b(B), c(C), d(D) {}
	set<vector3d> get_vectors() const;
};

ostream& operator<<(ostream&, const abcd&);
#endif