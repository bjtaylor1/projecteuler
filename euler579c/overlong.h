#pragma once
#ifndef CLASS_BIGULONG
#define CLASS_BIGULONG

using namespace std;

class overlong
{
public:
	static unsigned long MAX;

	unsigned long long val;
	overlong(unsigned long long Val) : val(Val) {}


	overlong& operator++(int)
	{
		return overlong(val++);
	}

	static unsigned long safe_add(unsigned long long a, unsigned long long b);
	static unsigned long safe_multiply(unsigned long long n, unsigned long long f);
	static bool multiplication_is_safe(unsigned long long n, unsigned long long f);
	static bool addition_is_safe(unsigned long long a, unsigned long long b);
	static size_t highestOneBitPosition(unsigned long long a);

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
