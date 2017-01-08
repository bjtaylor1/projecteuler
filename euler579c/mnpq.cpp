#include "stdafx.h"
#include "mnpq.h"

void mnpq::add_cube(const cube& cube)
{
	cubes.insert(cube);
}

abcd mnpq::get_abcd() const
{
	long a = 2 * m*p + 2 * n*q;
	long b = 2 * n*p - 2 * m*q;
	long c = (p*p) + (q*q) - (n*n + m*m);
	long d = (p*p) + (q*q) + (n*n + m*m);
	return abcd(a, b, c, d);
}

vector<long> mnpq::as_vector() const
{
	return vector<long>({ m,n,p,q });
}
