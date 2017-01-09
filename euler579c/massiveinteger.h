#pragma once

#ifndef CLASS_MASSIVEINTEGER
#define CLASS_MASSIVEINTEGER

using namespace std;

class massiveinteger
{
public:
	vector<int> digits;
	massiveinteger() : massiveinteger(vector<int>({ 0 })) {}
	massiveinteger(const vector<int>& Digits) : digits(Digits) {}
	massiveinteger(unsigned long long n)
	{
		do {
			long rem = n % 10;
			digits.push_back(rem);
			n = (n - rem) / 10;
		} while (n > 0);
	}
};

ostream& operator<<(ostream& os, const massiveinteger& m1);

massiveinteger operator +(const massiveinteger& m1, const massiveinteger& m2);

#endif
