#pragma once
#ifndef CLASS_VECTOR3D
#define CLASS_VECTOR3D
#include "macros.h"
using namespace std;

class vector3d
{
public:
	long x, y, z, length;
	vector3d(long X, long Y, long Z) : x(X), y(Y), z(Z)
	{
		long sumSquares = (X*X + Y*Y + Z*Z);
		length = my_round(sqrt(sumSquares));
		if (length*length != sumSquares) throw exception("Vector length not integral.");
	}

	bool is_orthogonal_to(const vector3d&) const;
	long dot_product(const vector3d&) const;
};


bool operator<(const vector3d&, const vector3d&);
bool operator!=(const vector3d&, const vector3d&);

#endif