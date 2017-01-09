#pragma once
#ifndef CLASS_CUBE
#define CLASS_CUBE

#include "vector3d.h"
using namespace std;

class cube
{
public:
	unsigned long width, height, depth;
	set<vector3d> uvn;
	set<vertex> vertices;
	cube(const vector3d& U, const vector3d& V, const vector3d& N);

private:
	void add_vertex(vector<vertex>& vertices, long& minx, long& miny, long& minz, const vertex& vertex);
};

bool operator <(const cube& lhs, const cube& rhs);
ostream& operator<<(ostream& os, const cube& cube);

#endif