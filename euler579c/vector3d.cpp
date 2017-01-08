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

bool vector3d::is_orthogonal_to(const vector3d& rhs) const
{
	double angleRads = acos(((double)dot_product(rhs)) / (length * rhs.length));
	double angleDegs = angleRads * M_PI / 180;
	bool is_orth = abs(angleDegs - 90) < 1e-9;
	return is_orth;
}

long vector3d::dot_product(const vector3d& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}
