#pragma once

#ifndef CLASS_VECTORTRIPLE
#define CLASS_VECTORTRIPLE
#include "vector3d.h"

class vectortriple
{
public:
	vector3d u, v, n;
	vectortriple(const vector3d U, const vector3d V, const vector3d N) : u(U), v(V), n(N) {}
};

#endif