#include "stdafx.h"
#include "vectortriple.h"

vectortriple::vectortriple(const vector3d U, const vector3d V, const vector3d N) : u(U), v(V), n(N)
{
#ifdef CAUTIOUS
	if (
		(!U.is_orthogonal_to(V)) ||
		(!V.is_orthogonal_to(N)) ||
		(!N.is_orthogonal_to(U))
		) throw runtime_error("Vector triple initialized with bad vectors!");

	if (U.length != V.length || U.length != N.length) 
		throw runtime_error("Vector triple initialized with different length vectors!");
#endif
}

set<vector3d> vectortriple::get_key() const
{
	set<vector3d> absVectors({ u.to_key(), v.to_key(), n.to_key() });
	return absVectors;
}

long long vectortriple::sumgcd() const
{
	long long returnvalue = u.gcd() + v.gcd() + n.gcd();
	return returnvalue;
}

vectortriple operator*(const transformation& t, const vectortriple& vt)
{
	vector3d u = t*vt.u;
	vector3d v = t*vt.v;
	vector3d n = t*vt.n;
	return vectortriple(u,v,n);
}

ostream& operator<<(ostream& os, const vectortriple& v)
{
	os << v.u << "  " << v.v << "  " << v.n;
	return os;
}
