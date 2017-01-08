#pragma once

#ifndef CLASS_VERTEX
#define CLASS_VERTEX
using namespace std;
class vertex
{
public:
	long x, y, z;
	vertex(long X, long Y, long Z) : x(X), y(Y), z(Z) {}
	virtual ~vertex() {}
};

bool operator<(const vertex&, const vertex&);
bool operator !=(const vertex& lhs, const vertex& rhs);
vertex operator-(const vertex&, const vertex&);

ostream& operator <<(ostream& os, const vertex& vertex);

#endif