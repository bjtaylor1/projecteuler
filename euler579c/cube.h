#pragma once
#ifndef CLASS_CUBE
#define CLASS_CUBE

#include "vector3d.h"
using namespace std;

class cube
{
public:
	static long long maxSize;
	int width, height, depth;
	set<vertex> vertices;
	static set<vertex> get_vertices(const vector3d& U, const vector3d& V, const vector3d& N, bool flipX, bool flipY, bool flipZ, int* order);
	cube(const set<vertex>& Vertices);
	bool is_oversize() const;
	
};

bool operator <(const cube& lhs, const cube& rhs);
cube operator*(const cube&, long);

#endif