#pragma once
#ifndef MNPQ_CLASS
#define MNPQ_CLASS

#include "cube.h"
#include "abcd.h"

using namespace std;

class mnpq
{
public:
	unsigned long m, n, p, q;
	mnpq(unsigned long M, unsigned long N, unsigned long P, unsigned long Q) : m(M), n(N), p(P), q(Q) {}

	set<cube> cubes;
	//it doesn't really need to store them, only to count them - but 
	//useful for debugging, unique-izing the count, and it will be thrown away anyway
	pair<set<cube>::iterator, bool> add_cube(const cube& cube);
	abcd get_abcd() const;
	vector<unsigned long> as_vector() const;
};

#endif
