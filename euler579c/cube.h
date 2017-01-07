#pragma once
#ifndef CLASS_CUBE
#define CLASS_CUBE

#include "vector3d.h"
using namespace std;

class cube
{
public:
	set<vector3d> uvn;
	cube(vector3d U, vector3d V, vector3d N) : uvn({ U,V,N}) {}
};

bool operator <(const cube& lhs, const cube& rhs);

#endif