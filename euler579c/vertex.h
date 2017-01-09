#pragma once

#ifndef CLASS_VERTEX
#define CLASS_VERTEX

#include "macros.h"

using namespace std;
class vertex
{
public:
	biglong x, y, z;
	vertex(biglong X, biglong Y, biglong Z) : x(X), y(Y), z(Z) {}
	virtual ~vertex() {}
};

bool operator<(const vertex&, const vertex&);
bool operator !=(const vertex& lhs, const vertex& rhs);
vertex operator-(const vertex&, const vertex&);

ostream& operator <<(ostream& os, const vertex& vertex);

#endif