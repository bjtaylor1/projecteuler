#pragma once
#ifndef CLASS_BIGULONG
#define CLASS_BIGULONG

using namespace std;

class overlong
{
public:
	static long long MAX;

	long long val;
	overlong(long long Val) : val(Val) {}


	overlong operator++(int)
	{
		return overlong(val++);
	}

	static long long safe_add(long long a, long long b);
	static long long safe_multiply(long long n, long long f);
	static bool multiplication_is_safe(long long n, long long f);
	static bool addition_is_safe(long long a, long long b);
	static size_t highestOneBitPosition(long long a);

};

overlong operator*(const overlong& a, const overlong& b);
overlong operator/(const overlong& a, const overlong& b);
overlong operator +(const overlong& a, const overlong& b);
overlong operator -(const overlong& a, const overlong& b);
overlong operator += (overlong& a, const overlong& b);
bool operator <=(const overlong& a, const overlong& b);
bool operator <(const overlong& a, const overlong& b);
ostream& operator <<(ostream& s, const overlong& l);

#endif
