#pragma once
#ifndef CLASS_VECTOR3D
#define CLASS_VECTOR3D

#include "macros.h"
#include "vertex.h"

using namespace std;

class vector3d : public vertex
{
public:
	biglong length;
	vector3d(biglong X, biglong Y, biglong Z);

	bool is_orthogonal_to(const vector3d&) const;
	biglong dot_product(const vector3d&) const;
	vector3d cross_product(const vector3d&) const;
	biglong gcd() const;

};

vertex operator+(const vertex&, const vector3d&);
vector3d operator/(const vector3d&, biglong);

#endif