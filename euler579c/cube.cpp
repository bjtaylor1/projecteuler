#include "stdafx.h"
#include "cube.h"

using namespace std;

long long cube::maxSize = 0;

bool is_less(const cube& lhs, const cube& rhs)
{
	if (lhs.width != rhs.width) return lhs.width < rhs.width;
	if (lhs.depth != rhs.depth) return lhs.depth < rhs.depth;
	if (lhs.height != rhs.height) return lhs.height < rhs.height;

	set<vertex>::const_iterator it_l = lhs.vertices.begin(), it_r = rhs.vertices.begin();
	for (long long l = 0; l < 8; l++)
	{
		if (it_l == lhs.vertices.end() || it_r == rhs.vertices.end()) throw runtime_error("A cube has less than 8 (distinct) vertices");
		if ((*it_l) != (*it_r)) return (*it_l) < (*it_r);
		++it_l; ++it_r;
	}
	if (it_l != lhs.vertices.end() || it_r != rhs.vertices.end()) throw runtime_error("A cube has more than 8 vertices");
	return false;
}

bool operator<(const cube & lhs, const cube & rhs)
{
	bool isless = is_less(lhs, rhs);
	return isless;
}

cube operator*(const cube& c, long n)
{
	set<vertex> newVertices;
	for (set<vertex>::const_iterator vertex = c.vertices.begin(); vertex != c.vertices.end(); vertex++)
	{
		newVertices.insert((*vertex) * n);
	}
	return cube(newVertices);
}

bool compare_x(const vertex& v1, const vertex& v2) { return v1.x < v2.x; }
bool compare_y(const vertex& v1, const vertex& v2) { return v1.y < v2.y; }
bool compare_z(const vertex& v1, const vertex& v2) { return v1.z < v2.z; }

set<vertex> cube::get_vertices(const vector3d & U, const vector3d & V, const vector3d & N, bool flipX, bool flipY, bool flipZ, int* order)
{
	if (
		(!U.is_orthogonal_to(V)) ||
		(!V.is_orthogonal_to(N)) ||
		(!N.is_orthogonal_to(U))
		) throw runtime_error("Cube initialized with bad vectors!");


	long minx = 0, miny = 0, minz = 0;
	vertex O(0, 0, 0);
	vector<vertex> tempvertices
	{
		O,
		U,
		V,
		N,
		U+V,
		U+N,
		V+N,
		U+V+N
	};

	long minX = min_element(tempvertices.begin(), tempvertices.end(), compare_x)->x;
	long minY = min_element(tempvertices.begin(), tempvertices.end(), compare_y)->y;
	long minZ = min_element(tempvertices.begin(), tempvertices.end(), compare_z)->z;
	for (vector<vertex>::iterator vert = tempvertices.begin(); vert != tempvertices.end(); vert++)
	{
		vert->x -= minX;
		vert->y -= minY;
		vert->z -= minZ;
	}
	long maxX = max_element(tempvertices.begin(), tempvertices.end(), compare_x)->x;
	long maxY = max_element(tempvertices.begin(), tempvertices.end(), compare_y)->y;
	long maxZ = max_element(tempvertices.begin(), tempvertices.end(), compare_z)->z;

	set<vertex> initVertices;
	for (vector<vertex>::const_iterator it = tempvertices.begin(); it != tempvertices.end(); it++)
	{
		long xyz[] =
		{
			flipX ? maxX - it->x : it->x,
			flipY ? maxY - it->y : it->y,
			flipZ ? maxZ - it->z : it->z
		};

		initVertices.insert(vertex(xyz[order[0]], xyz[order[1]], xyz[order[2]]));
	}
	return initVertices;
}

cube::cube(const set<vertex>& initVertices) : vertices(initVertices)
{
	long finalMinX = min_element(initVertices.begin(), initVertices.end(), compare_x)->x;
	long finalMinY = min_element(initVertices.begin(), initVertices.end(), compare_y)->y;
	long finalMinZ = min_element(initVertices.begin(), initVertices.end(), compare_z)->z;
	if (finalMinX != 0 || finalMinY != 0 || finalMinZ != 0) throw runtime_error("Cube has been transformed wrongly!");

	width = max_element(initVertices.begin(), initVertices.end(), compare_x)->x;
	height = max_element(initVertices.begin(), initVertices.end(), compare_y)->y;
	depth = max_element(initVertices.begin(), initVertices.end(), compare_z)->z;

	if (initVertices.size() != 8) throw runtime_error("A cube has less than 8 (distinct) initVertices");
}


bool cube::is_oversize() const
{
	return (width > maxSize) || (height > maxSize) || (depth > maxSize);

}
