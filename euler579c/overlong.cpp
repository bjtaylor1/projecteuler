#include "stdafx.h"
#include "overlong.h"

using namespace std;

long long overlong::MAX = 0;

overlong operator*(const overlong& a, const overlong& b)
{
	return overlong::safe_multiply(a.val, b.val);
}
overlong operator/(const overlong& a, const overlong& b)
{
	return a.val / b.val;
}
overlong operator +(const overlong& a, const overlong& b)
{
	return overlong::safe_add(a.val, b.val);
}
overlong operator -(const overlong& a, const overlong& b)
{
	return a.val - b.val;
}
overlong operator += (overlong& a, const overlong& b)
{
	long long valA, valB;
	if (overlong::MAX > 0)
	{
		valA = a.val % overlong::MAX;
		valB = b.val % overlong::MAX;
	}
	else
	{
		valA = a.val;
		valB = b.val;
	}
	long long newval = valA + valB;
	if (overlong::MAX > 0)
	{
		newval %= overlong::MAX;
	}
	a.val = newval;
	return a;
}
bool operator<=(const overlong& a, const overlong& b)
{
	return a.val <= b.val;
}
bool operator<(const overlong& a, const overlong& b)
{
	return a.val < b.val;
}

ostream & operator<<(ostream& os, const overlong & l)
{
	os << l.val;
	return os;
}


long long overlong::safe_add(long long a, long long b)
{
	if (!overlong::addition_is_safe(a, b))
	{
		stringstream s;
		s << "Tried to unsafely add " << a << " + " << b;
		throw runtime_error(s.str().c_str());
	}
	return a+b;
}

long long overlong::safe_multiply(long long a, long long b)
{
	if (b == 1) return a;
	if (a == 1) return b;
	if (!overlong::multiplication_is_safe(a, b))
	{
		stringstream s;
		s << "Tried to unsafely multiply " << a << " x " << b;
		throw runtime_error(s.str().c_str());
	}
	return a*b;
}

bool overlong::addition_is_safe(long long a, long long b)
{
	size_t a_bits = highestOneBitPosition(a), b_bits = highestOneBitPosition(b);
	return (a_bits<64 && b_bits<64);
}

bool overlong::multiplication_is_safe(long long a, long long b)
{
	size_t a_bits = highestOneBitPosition(a), b_bits = highestOneBitPosition(b);
	return (a_bits + b_bits < 64);
}

size_t overlong::highestOneBitPosition(long long a)
{
	size_t bits = 0;
	while (a != 0)
	{
		++bits;
		a >>= 1;
	};
	return bits;
}
