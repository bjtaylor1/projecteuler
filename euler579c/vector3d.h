#pragma once
#ifndef CLASS_VECTOR3D
#define CLASS_VECTOR3D

#include "macros.h"
#include "vertex.h"

using namespace std;

class vector3d : public vertex
{
public:
	long long length;
	vector3d(long long X, long long Y, long long Z);

	bool is_orthogonal_to(const vector3d&) const;
	long long dot_product(const vector3d&) const;
	vector3d cross_product(const vector3d&) const;
	long long gcd() const;

	vector3d to_abs() const;

};

vertex operator+(const vertex&, const vector3d&);
vector3d operator/(const vector3d&, long);

#endif