#include "stdafx.h"
#include "abcd.h"

set<vector3d> abcd::get_vectors() const
{
	vector<long long> numbers({ a,b,c });
	sort(numbers.begin(), numbers.end());
	set<vector3d> vectors;
	do
	{
		vector3d v(*numbers.begin(), *(numbers.begin() + 1), *(numbers.begin() + 2));
		vectors.insert(v);
	} while (next_permutation(numbers.begin(), numbers.end()));

	return vectors;
}

long long abcd::get_count_zero() const
{
	long long countzero = 0;
	if (a == 0) countzero++;
	if (b == 0) countzero++;
	if (c == 0) countzero++;
	if (d == 0) countzero++;
	return countzero;
}

vector<int> abcd::get_key() const
{
	vector<int> key({ (int)abs(a), (int)abs(b), (int)abs(c) });
	sort(key.begin(), key.end());
	return key;
}

bool abcd::all_positive() const
{
	return a >= 0 && b >= 0 && c >= 0 && d >= 0;
}

ostream& operator<<(ostream& os, const abcd& item)
{
	os << item.a << "," << item.b << "," << item.c << ", side = " << item.d;
	return os;
}