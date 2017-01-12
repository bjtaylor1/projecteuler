#pragma once
#ifndef CLASS_CUBE
#define CLASS_CUBE

#include "vector3d.h"
using namespace std;

class cube
{
public:
	static long long maxSize;
	bool oversize;
	long long width, height, depth;
	set<vector3d> uvn;
	set<vertex> vertices;
	cube(const vector3d& U, const vector3d& V, const vector3d& N, bool flipX, bool flipY, bool flipZ, int* order);

	
};

bool operator <(const cube& lhs, const cube& rhs);
ostream& operator<<(ostream& os, const cube& cube);

#endif