#include "stdafx.h"
#include "vector3d.h"
#include "util.h"

long long vector3d::gcd() const
{
	long long retval = util<long long>::gcd(x, y, z);
	return retval;
}

vector3d vector3d::to_key() const
{
	vector<long long> sides({ abs(x), abs(y), abs(z) });
	sort(sides.begin(), sides.end());
	return vector3d(sides[0], sides[1], sides[2]);
}


vertex operator+(const vertex& lhs, const vector3d& rhs)
{
	return vertex(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

vector3d operator/(const vector3d& lhs, long long f)
{
	long long newx = lhs.x / f;
	long long newy = lhs.y / f;
	long long newz = lhs.z / f;
	return vector3d(newx, newy, newz);
}

vector3d::vector3d(long long X, long long Y, long long Z) : vertex(X, Y, Z)
{
	long long sumSquares = (X*X + Y*Y + Z*Z);
	length = my_round(sqrt(sumSquares));
	
	if (length*length != sumSquares)
	{
		stringstream msg;
		msg << "Vector length not integral for ";
		msg << X << "," << Y << "," << Z;
		throw runtime_error(msg.str().c_str());
	}
}

bool vector3d::is_orthogonal_to(const vector3d& rhs) const
{
	double angleRads = abs(acos(((double)dot_product(rhs)) / (length * rhs.length)));
	double angleDegs = angleRads * 180 / M_PI;
	bool is_orth = abs(angleDegs - 90) < 1e-9;
	return is_orth;
}

long long vector3d::dot_product(const vector3d& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}

vector3d vector3d::cross_product(const vector3d& rhs) const
{
	long long newx = y * rhs.z - z * rhs.y;
	long long newy = z * rhs.x - x * rhs.z;
	long long newz = x * rhs.y - y * rhs.x;

	vector3d n(newx, newy, newz);
	n = n / (n.length / rhs.length);
	return  n;
}

vector3d operator*(const vector3d& v, long long n)
{
	return vector3d(v.x*n, v.y*n, v.z*n);
}