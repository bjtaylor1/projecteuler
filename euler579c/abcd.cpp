#include "stdafx.h"
#include "abcd.h"
#include "mnpq.h"
#include "util.h"

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

abcd abcd::to_key() const
{
	vector<long long> v({ abs(a), abs(b), abs(c) });
	sort(v.begin(), v.end());

	return abcd(v[0], v[1], v[2], d);
}

ostream& operator<<(ostream& os, const abcd& item)
{
	os << item.a << "," << item.b << "," << item.c << "," << item.d;
	return os;
}

set<abcd> get_permutations(const abcd & item)
{
	int order[] = { 0,1,2 };
	long long baseArray[] = { item.a, item.b, item.c };
	set<abcd> abcds;
	do
	{
		for (int neg = 0; neg < 8; neg++)
		{
			int aNeg = (neg & 1) == 0 ? 1 : -1;
			int bNeg = (neg & 2) == 0 ? 1 : -1;
			int cNeg = (neg & 4) == 0 ? 1 : -1;
			abcd i(aNeg * baseArray[order[0]], bNeg * baseArray[order[1]], cNeg * baseArray[order[2]], item.d);
			abcds.insert(i);
		}
	} while (next_permutation(order, order + 3));
	return abcds;
}

bool operator<(const abcd & l, const abcd & r)
{
	if (l.a != r.a) return l.a < r.a;
	if (l.b != r.b) return l.b < r.b;
	if (l.c != r.c) return l.c < r.c;
	if (l.d != r.d) return l.d < r.d;
	return false;
}
