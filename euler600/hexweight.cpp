#include "stdafx.h"
#include "hexweight.h"

using namespace std;
hexweight::hexweight(long _s0, long _s1, long _s2, long _s3, long _s4, long _s5)
	: s0(_s0), s1(_s1), s2(_s2), s3(_s3), s4(_s4), s5(_s5)
{
}

bool hexweight::operator<(const hexweight& rhs) const
{
	if (s0 != rhs.s0) return s0 < rhs.s0;
	if (s1 != rhs.s1) return s1 < rhs.s1;
	if (s2 != rhs.s2) return s2 < rhs.s2;
	if (s3 != rhs.s3) return s3 < rhs.s3;
	if (s4 != rhs.s4) return s4 < rhs.s4;
	if (s5 != rhs.s5) return s5 < rhs.s5;

	return false;
}

vector<long> hexweight::get_sides() const
{
	return vector<long>({s0, s1, s2, s3, s4, s5});
}
