#pragma once

#ifndef CLASS_VECTORTRIPLE
#define CLASS_VECTORTRIPLE
#include "vector3d.h"

class vectortriple
{
public:
	vector3d u, v, n;
	vectortriple(const vector3d U, const vector3d V, const vector3d N) : u(U), v(V), n(N)
	{
		if (
			(!U.is_orthogonal_to(V)) ||
			(!V.is_orthogonal_to(N)) ||
			(!N.is_orthogonal_to(U))
			) throw runtime_error("Vector triple initialized with bad vectors!");
	}

	set<vector3d> get_key() const
	{
		set<vector3d> absVectors({ u.to_key(), v.to_key(), n.to_key() });
		return absVectors;
	}
};

#endif