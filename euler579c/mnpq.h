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

	set<cube> cubes;
	//it doesn't really need to store them, only to count them - but 
	//useful for debugging, unique-izing the count, and it will be thrown away anyway
	pair<set<cube>::iterator, bool> add_cube(const cube& cube);
	abcd get_abcd() const;
	vector<long long> as_vector() const;
};

ostream& operator <<(ostream& os, const mnpq& item);

#endif