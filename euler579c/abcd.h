#pragma once
#ifndef CLASS_ABCD
#include "vector3d.h"

using namespace std;

class abcd
{
public:
	unsigned long a, b, c, d;
	abcd(unsigned long A, unsigned long B, unsigned long C, unsigned long D) : a(A), b(B), c(C), d(D) {}
	set<vector3d> get_vectors() const;
};

ostream& operator<<(ostream&, const abcd&);
#endif