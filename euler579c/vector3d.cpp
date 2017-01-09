#include "stdafx.h"
#include "vector3d.h"
#include "util.h"

biglong vector3d::gcd() const
{
	return util::gcd(set<biglong>({ x, y, z }));
}


vertex operator+(const vertex& lhs, const vector3d& rhs)
{
	return vertex(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

vector3d operator/(const vector3d& lhs, biglong f)
{
	biglong newx = lhs.x / f;
	biglong newy = lhs.y / f;
	biglong newz = lhs.z / f;
	return vector3d(newx, newy, newz);
}

vector3d::vector3d(biglong X, biglong Y, biglong Z) : vertex(X, Y, Z)
{
	bigulong sumSquares = (X*X + Y*Y + Z*Z);
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

biglong vector3d::dot_product(const vector3d& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}

vector3d vector3d::cross_product(const vector3d& rhs) const
{
	biglong newx = y * rhs.z - z * rhs.y;
	biglong newy = z * rhs.x - x * rhs.z;
	biglong newz = x * rhs.y - y * rhs.x;

	vector3d n(newx, newy, newz);
	n = n / (n.length / rhs.length);
	return  n;
}
