#include "stdafx.h"
#include "vertex.h"

using namespace std;

bool operator<(const vertex & lhs, const vertex & rhs)
{
	if (lhs.x != rhs.x) return lhs.x < rhs.x;
	if (lhs.y != rhs.y) return lhs.y < rhs.y;
	if (lhs.z != rhs.z) return lhs.z < rhs.z;

	return false;
}

bool operator !=(const vertex& lhs, const vertex& rhs)
{
	return lhs.x != rhs.x ||
		lhs.y != rhs.y ||
		lhs.z != rhs.z;
}


vertex operator-(const vertex& lhs, const vertex& rhs)
{
	return vertex(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

vertex operator*(const vertex& v, long n)
{
	return vertex(v.x * n, v.y * n, v.z * n);
}

ostream& operator<<(ostream& os, const vertex & vertex)
{
	os << vertex.x << "," << vertex.y << "," << vertex.z;
	return os;
}
