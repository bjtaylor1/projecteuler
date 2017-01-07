#pragma once
#ifndef CLASS_VECTOR3D
#define CLASS_VECTOR3D

class vector3d
{
public:
	long x, y, z;
	vector3d(long X, long Y, long Z) : x(X), y(Y), z(Z) {}
};


bool operator<(const vector3d&, const vector3d&);
bool operator!=(const vector3d&, const vector3d&);

#endif