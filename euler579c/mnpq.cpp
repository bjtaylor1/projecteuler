#include "stdafx.h"
#include "mnpq.h"

pair<set<cube>::iterator, bool> mnpq::add_cube(const cube& cube)
{
	return cubes.insert(cube);
}

abcd mnpq::get_abcd() const
{
	biglong a = 2 * m*p + 2 * n*q;
	biglong b = 2 * n*p - 2 * m*q;
	biglong c = (p*p) + (q*q) - (n*n + m*m);
	biglong d = (p*p) + (q*q) + (n*n + m*m);
	return abcd(a, b, c, d);
}

vector<biglong> mnpq::as_vector() const
{
	return vector<biglong>({ m,n,p,q });
}
