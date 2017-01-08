#include "stdafx.h"
#include "abcd.h"

set<vector3d> abcd::get_vectors() const
{
	vector<long> numbers({ a,b,c });
	sort(numbers.begin(), numbers.end());
	set<vector3d> vectors;
	do
	{
		vector3d v(*numbers.begin(), *(numbers.begin() + 1), *(numbers.begin() + 2));
		vectors.insert(v);
	} while (next_permutation(numbers.begin(), numbers.end()));

	return vectors;
}

vector3d abcd::get_root_vector() const
{
	vector<long> numbers({ a,b,c });
	sort(numbers.begin(), numbers.end());
	vector3d v(*numbers.begin(), *(numbers.begin() + 1), *(numbers.begin() + 2));
	return v;
}