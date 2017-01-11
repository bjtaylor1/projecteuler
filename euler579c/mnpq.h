#pragma once
#ifndef MNPQ_CLASS
#define MNPQ_CLASS

#include "cube.h"
#include "abcd.h"

using namespace std;

class mnpq
{
public:
	long long m, n, p, q;
	mnpq(long long M, long long N, long long P, long long Q) : m(M), n(N), p(P), q(Q) {}

	abcd get_abcd() const;
	vector<long long> as_vector() const;
};

ostream& operator <<(ostream& os, const mnpq& item);

#endif
