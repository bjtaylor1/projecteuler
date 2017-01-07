#include "stdafx.h"
#include "abcd.h"

vector<vector3d> abcd::get_vectors() const
{
	vector<vector3d> vectors
	{
		vector3d(a,b,c),
		vector3d(a,c,b),
		vector3d(c,a,b),
		vector3d(c,b,a)
	};
	return vectors;
}

