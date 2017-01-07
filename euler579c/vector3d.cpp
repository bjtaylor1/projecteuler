#include "stdafx.h"
#include "vector3d.h"

bool operator<(const vector3d & lhs, const vector3d & rhs)
{
	if (lhs.x != rhs.x) return lhs.x < rhs.x;
	if (lhs.y != rhs.y) return lhs.y < rhs.y;
	if (lhs.z != rhs.z) return lhs.z < rhs.z;

	return false;
}

bool operator !=(const vector3d& lhs, const vector3d& rhs)
{
	return lhs.x != rhs.x ||
		lhs.y != rhs.y ||
		lhs.z != rhs.z;
}