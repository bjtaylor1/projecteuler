#include "stdafx.h"
#include "cube.h"

using namespace std;

bool is_less(const cube& lhs, const cube& rhs)
{
	set<vertex>::const_iterator it_l = lhs.vertices.begin(), it_r = rhs.vertices.begin();
	for (long l = 0; l < 8; l++)
	{
		if (it_l == lhs.vertices.end() || it_r == rhs.vertices.end()) throw exception("A cube has less than 8 (distinct) vertices");
		if ((*it_l) != (*it_r)) return (*it_l) < (*it_r);
		++it_l; ++it_r;
	}
	if (it_l != lhs.vertices.end() || it_r != rhs.vertices.end()) throw exception("A cube has more than 8 vertices");
	return false;
}

bool operator<(const cube & lhs, const cube & rhs)
{
	bool isless = is_less(lhs, rhs);
	return isless;
}

ostream & operator<<(ostream & os, const cube & cube)
{
	for (set<vertex>::const_iterator it = cube.vertices.begin();;)
	{
		cout << *it;
		if ((++it) == cube.vertices.end()) break; else cout << "   ";
	}
	return os;
}

cube::cube(const vector3d & U, const vector3d & V, const vector3d & N) : uvn({U,V,N})
{
	long minx = 0, miny = 0, minz = 0;
	vector<vertex> tempvertices;
	vertex O(0, 0, 0);
	add_vertex(tempvertices, minx, miny, minz, O); //origin
	add_vertex(tempvertices, minx, miny, minz, U);
	add_vertex(tempvertices, minx, miny, minz, V);
	add_vertex(tempvertices, minx, miny, minz, N);
	add_vertex(tempvertices, minx, miny, minz, U+V);
	add_vertex(tempvertices, minx, miny, minz, U+N);
	add_vertex(tempvertices, minx, miny, minz, V+N);
	add_vertex(tempvertices, minx, miny, minz, U+V+N);

	vertex minBounds(minx, miny, minz);
	for (vector<vertex>::const_iterator it = tempvertices.begin(); it != tempvertices.end(); it++)
	{
		vertices.insert((*it) - minBounds);
	}

	if (vertices.size() != 8) throw exception("A cube has less than 8 (distinct) vertices");
}

void cube::add_vertex(vector<vertex>& vertices, long& minx, long& miny, long& minz, const vertex& vertex)
{
	minx = min(minx, vertex.x);
	miny = min(miny, vertex.y);
	minz = min(minz, vertex.z);
	vertices.push_back(vertex);
}
