#pragma once

#ifndef CLASS_VERTEX
#define CLASS_VERTEX

#include "macros.h"

using namespace std;
class vertex
{
public:
	long long x, y, z;
	vertex(long long X, long long Y, long long Z) : x(X), y(Y), z(Z) {}
	virtual ~vertex() {}
};

bool operator<(const vertex&, const vertex&);
bool operator !=(const vertex& lhs, const vertex& rhs);
vertex operator-(const vertex&, const vertex&);
vertex operator*(const vertex&, long);
ostream& operator <<(ostream& os, const vertex& vertex);

#endif