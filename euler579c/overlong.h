#pragma once
#ifndef CLASS_BIGULONG
#define CLASS_BIGULONG

class overlong
{
public:
	static unsigned long MAX;

	unsigned long long val;
	overlong(unsigned long long Val) : val(Val) {}


	overlong& operator++(int)
	{
		val++;
		return *this;
	}

	static unsigned long safe_add(unsigned long a, unsigned long b);
	static unsigned long safe_multiply(unsigned long n, unsigned long f);
	static bool multiplication_is_safe(unsigned long n, unsigned long f);
	static bool addition_is_safe(unsigned long a, unsigned long b);
	static size_t highestOneBitPosition(unsigned long a);

};

overlong operator*(const overlong& a, const overlong& b);
overlong operator/(const overlong& a, const overlong& b);
overlong operator +(const overlong& a, const overlong& b);
overlong operator -(const overlong& a, const overlong& b);
overlong operator += (const overlong& a, const overlong& b);
bool operator <=(const overlong& a, const overlong& b);
bool operator <(const overlong& a, const overlong& b);

#endif
