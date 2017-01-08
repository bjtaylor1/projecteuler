#pragma once
#ifndef CLASS_CUBE
#define CLASS_CUBE

#include "vector3d.h"
using namespace std;

class cube
{
public:
	set<vector3d> uvn;
	set<vertex> vertices;
	cube(vector3d& U, vector3d& V, vector3d& N);

private:
	void add_vertex(vector<vertex>& vertices, long& minx, long& miny, long& minz, const vertex& vertex);
};

bool operator <(const cube& lhs, const cube& rhs);

#endif