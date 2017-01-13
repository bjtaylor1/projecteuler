#pragma once
#ifndef CLASS_ABCD
#include "vector3d.h"
#include "vectortriple.h"

using namespace std;

class abcd
{
public:
	long long a, b, c, d;
	abcd(long long A, long long B, long long C, long long D) : a(A), b(B), c(C), d(D) {}
	set<vector3d> get_vectors() const;
	long long get_count_zero() const;
};

ostream& operator<<(ostream&, const abcd&);
set<abcd> get_permutations(const abcd& item);
bool operator<(const abcd&, const abcd&);

#endif