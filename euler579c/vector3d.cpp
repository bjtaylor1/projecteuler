#include "stdafx.h"
#include "vector3d.h"

vertex operator+(const vertex& lhs, const vector3d& rhs)
{
	return vertex(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

vector3d operator/(const vector3d& lhs, long f)
{
	long newx = lhs.x / f;
	long newy = lhs.y / f;
	long newz = lhs.z / f;
	return vector3d(newx, newy, newz);
}

vector3d::vector3d(long X, long Y, long Z) : vertex(X, Y, Z)
{
	long sumSquares = (X*X + Y*Y + Z*Z);
	length = my_round(sqrt(sumSquares));
	if (length*length != sumSquares) throw exception("Vector length not integral.");
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

vector3d vector3d::cross_product(const vector3d& rhs) const
{
	long newx = y * rhs.z - z * rhs.y;
	long newy = z * rhs.x - x * rhs.z;
	long newz = x * rhs.y - y * rhs.x;

	vector3d n(newx, newy, newz);
	n = n / (n.length / rhs.length);
	return  n;
}
