#include "stdafx.h"
#include "mnpq.h"

pair<set<cube>::iterator, bool> mnpq::add_cube(const cube& cube)
{
	return cubes.insert(cube);
}

abcd mnpq::get_abcd() const
{
	long long a = 2 * m*p + 2 * n*q;
	long long b = 2 * n*p - 2 * m*q;
	long long c = (p*p) + (q*q) - (n*n + m*m);
	long long d = (p*p) + (q*q) + (n*n + m*m);
	return abcd(a, b, c, d);
}

vector<long long> mnpq::as_vector() const
{
	return vector<long long>({ m,n,p,q });
}
