#include "stdafx.h"
#include "vectortriple.h"

vectortriple::vectortriple(const vector3d U, const vector3d V, const vector3d N) : u(U), v(V), n(N)
{
	if (
		(!U.is_orthogonal_to(V)) ||
		(!V.is_orthogonal_to(N)) ||
		(!N.is_orthogonal_to(U))
		) throw runtime_error("Vector triple initialized with bad vectors!");

}

set<vector3d> vectortriple::get_key() const
{
	set<vector3d> absVectors({ u.to_key(), v.to_key(), n.to_key() });
	return absVectors;
}

long long vectortriple::sumgcd() const
{
	return u.gcd() + v.gcd() + n.gcd();
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
