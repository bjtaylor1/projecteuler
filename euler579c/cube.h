#pragma once
#ifndef CLASS_CUBE
#define CLASS_CUBE

#include "vector3d.h"
#include "vectortriple.h"

using namespace std;

class cube
{
public:
	cube(const vectortriple&);
	cube(const set<vertex>& Vertices, long long Sumgcd);

	static long long maxSize;
	long long width, height, depth;
	long long sumgcd;
	set<vertex> vertices;
	static set<vertex> align_vertices(const vectortriple&);
	bool is_oversize() const;
	vectortriple get_triple() const;
};

bool operator <(const cube& lhs, const cube& rhs);
ostream& operator<<(ostream& os, const cube& c);

#endif