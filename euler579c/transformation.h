#pragma once

#ifndef CLASS_TRANSFORMATION
#define CLASS_TRANSFORMATION
#include "vector3d.h"

using namespace std;

class transformation
{
public:
	vector<long long> numbers;
	transformation(const vector<long long>& Numbers);
	long long operator[](size_t) const;
};

vector3d operator*(const vector3d&, const transformation&);


#endif
