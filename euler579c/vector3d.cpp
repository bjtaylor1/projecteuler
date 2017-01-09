#include "stdafx.h"
#include "vector3d.h"
#include "util.h"

long vector3d::gcd() const
{
	return util::gcd(set<long>({ x, y, z }));
}


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
	unsigned long sumSquares = (X*X + Y*Y + Z*Z);
	length = my_round(sqrt(sumSquares));
	
	if (length*length != sumSquares)
	{
		stringstream msg;
		msg << "Vector length not integral for ";
		msg << X << "," << Y << "," << Z;
		throw exception(msg.str().c_str());
	}
}

bool vector3d::is_orthogonal_to(const vector3d& rhs) const
{
	double angleRads = acos(((double)dot_product(rhs)) / (length * rhs.length));
	double angleDegs = angleRads * 180 / M_PI;
	bool is_orth = abs(angleDegs - 90) < 1e-9;
	return is_orth;
}

unsigned long vector3d::dot_product(const vector3d& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}

vector3d vector3d::cross_product(const vector3d& rhs) const
{
	unsigned long newx = y * rhs.z - z * rhs.y;
	unsigned long newy = z * rhs.x - x * rhs.z;
	unsigned long newz = x * rhs.y - y * rhs.x;

	vector3d n(newx, newy, newz);
	n = n / (n.length / rhs.length);
	return  n;
}
