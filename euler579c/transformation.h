#pragma once

#ifndef CLASS_TRANSFORMATION
#define CLASS_TRANSFORMATION
#include "vector3d.h"

using namespace std;

class transformation
{
public:
	char axis; double angle;

	vector<long long> numbers;
	transformation(const vector<long long>& Numbers, char axis, double angle);
	long long operator[](size_t) const;
};

vector3d operator*(const transformation&, const vector3d&);


#endif
