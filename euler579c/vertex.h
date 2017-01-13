#pragma once

#ifndef CLASS_VERTEX
#define CLASS_VERTEX

#include "macros.h"

using namespace std;
class vertex
{
public:
	int x, y, z;
	vertex(int X, int Y, int Z) : x(X), y(Y), z(Z) {}
	virtual ~vertex() {}
};

bool operator<(const vertex&, const vertex&);
bool operator !=(const vertex& lhs, const vertex& rhs);
vertex operator-(const vertex&, const vertex&);
vertex operator*(const vertex&, long);
ostream& operator <<(ostream& os, const vertex& vertex);

#endif