#pragma once

#ifndef CLASS_VECTORTRIPLE
#define CLASS_VECTORTRIPLE
#include "vector3d.h"
#include "transformation.h"

class vectortriple
{
public:
	vector3d u, v, n;
	vectortriple(const vector3d U, const vector3d V, const vector3d N);

	set<vector3d> get_key() const;	
	long long sumgcd() const;
};

vectortriple operator*(const transformation&, const vectortriple&);

#endif