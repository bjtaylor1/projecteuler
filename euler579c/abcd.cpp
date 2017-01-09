#include "stdafx.h"
#include "abcd.h"

set<vector3d> abcd::get_vectors() const
{
	vector<biglong> numbers({ a,b,c });
	sort(numbers.begin(), numbers.end());
	set<vector3d> vectors;
	do
	{
		vector3d v(*numbers.begin(), *(numbers.begin() + 1), *(numbers.begin() + 2));
		vectors.insert(v);
	} while (next_permutation(numbers.begin(), numbers.end()));

	return vectors;
}

ostream& operator<<(ostream& os, const abcd& item)
{
	os << item.a << "," << item.b << "," << item.c << ", side = " << item.d;
	return os;
}