#pragma once

#ifndef CLASS_TRANSFORMATION
#define CLASS_TRANSFORMATION
#include "vector3d.h"

using namespace std;

class transformation
{
public:
	string desc;

	vector<long long> numbers;
	transformation(const vector<long long>& Numbers, char axis, double angle);
	transformation(const vector<long long>& Numbers, const string& Desc);
	long long operator[](size_t) const;
};

vector3d operator*(const transformation&, const vector3d&);


#endif
