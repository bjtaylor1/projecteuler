#include "stdafx.h"
#include "cube.h"
#include "util.h"

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

	long long minX = min_element(tempvertices.begin(), tempvertices.end(), compare_x)->x;
	long long minY = min_element(tempvertices.begin(), tempvertices.end(), compare_y)->y;
	long long minZ = min_element(tempvertices.begin(), tempvertices.end(), compare_z)->z;
	for (vector<vertex>::iterator vert = tempvertices.begin(); vert != tempvertices.end(); vert++)
	{
		vert->x -= minX;
		vert->y -= minY;
		vert->z -= minZ;
	}
	long long maxX = max_element(tempvertices.begin(), tempvertices.end(), compare_x)->x;
	long long maxY = max_element(tempvertices.begin(), tempvertices.end(), compare_y)->y;
	long long maxZ = max_element(tempvertices.begin(), tempvertices.end(), compare_z)->z;

	set<vertex> initVertices;
	for (vector<vertex>::const_iterator it = tempvertices.begin(); it != tempvertices.end(); it++)
	{
		long long xyz[] =
		{
			flipX ? maxX - it->x : it->x,
			flipY ? maxY - it->y : it->y,
			flipZ ? maxZ - it->z : it->z
		};

		initVertices.insert(vertex(xyz[order[0]], xyz[order[1]], xyz[order[2]]));
	}
	return initVertices;
}

cube::cube(const set<vertex>& initVertices, long long Sumgcd) : vertices(initVertices), sumgcd(Sumgcd)
{
	long long finalMinX = min_element(initVertices.begin(), initVertices.end(), compare_x)->x;
	long long finalMinY = min_element(initVertices.begin(), initVertices.end(), compare_y)->y;
	long long finalMinZ = min_element(initVertices.begin(), initVertices.end(), compare_z)->z;
	if (finalMinX != 0 || finalMinY != 0 || finalMinZ != 0) throw runtime_error("Cube has been transformed wrongly!");

	width = max_element(initVertices.begin(), initVertices.end(), compare_x)->x;
	height = max_element(initVertices.begin(), initVertices.end(), compare_y)->y;
	depth = max_element(initVertices.begin(), initVertices.end(), compare_z)->z;

	if (initVertices.size() != 8) throw runtime_error("A cube has less than 8 (distinct) initVertices");
}

template<class T1, class T2>
bool sort_pair_first(const pair<T1, T2>& p1, const pair<T1, T2>& p2)
{
	return p1.first < p2.first;
}

vectortriple cube::get_triple() const
{
	//look at all the vertices distance from the first:
	int examined = 0;
	vector<pair<long long, vertex>> verticesByDistanceFromFirst;
	for (set<vertex>::const_iterator v = ++vertices.begin(); v != vertices.end(); v++)
	{
		long long distsq = v->dist_squared_from(*vertices.begin());
		pair<long long, vertex> p(distsq, *v);
		verticesByDistanceFromFirst.push_back(p);
		examined++;
	}
	if (examined != 7) throw runtime_error("Should have examined 7 vertices!");
	sort(verticesByDistanceFromFirst.begin(), verticesByDistanceFromFirst.end(), sort_pair_first<long long, vertex>);
	if (verticesByDistanceFromFirst[0].first != verticesByDistanceFromFirst[1].first ||
		verticesByDistanceFromFirst[1].first != verticesByDistanceFromFirst[2].first) throw runtime_error("Failed to get 3 nearest vertices 1");

	if (verticesByDistanceFromFirst[2].first == verticesByDistanceFromFirst[3].first) throw runtime_error("Failed to get 3 nearest vertices 2");

	vertex u1 = verticesByDistanceFromFirst[0].second - *vertices.begin();
	vertex v1 = verticesByDistanceFromFirst[1].second - *vertices.begin();
	vertex n1 = verticesByDistanceFromFirst[2].second - *vertices.begin();
	
	vector3d u(u1.x, u1.y, u1.z);
	vector3d v(v1.x, v1.y, v1.z);
	vector3d n(n1.x, n1.y, n1.z);
	return vectortriple(u, v, n);

}


bool cube::is_oversize() const
{
	return (width > maxSize) || (height > maxSize) || (depth > maxSize);

}

ostream& operator<<(ostream& os, const cube& c)
{
	for (set<vertex>::const_iterator v = c.vertices.begin(); v != c.vertices.end(); v++)
	{
		cout << *v << "  ";
	}
	return os;
}