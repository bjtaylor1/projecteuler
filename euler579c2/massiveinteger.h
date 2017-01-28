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
	massiveinteger(long long n)
	{
		if (n < 0)
		{
			stringstream ss;
			ss << "massiveinteger was initialized with " << n;
			throw runtime_error(ss.str().c_str());
		}
		do {
			long long rem = n % 10;
			digits.push_back(rem);
			n = (n - rem) / 10;
		} while (n > 0);
	}

	void truncate(size_t n);
	string approx() const;
	string mod10e(long n) const;
	string all() const;
};

ostream& operator<<(ostream& os, const massiveinteger& m1);

massiveinteger operator +(const massiveinteger& m1, const massiveinteger& m2);
massiveinteger operator +=(massiveinteger& m1, const massiveinteger& m2);
massiveinteger operator *(const massiveinteger& m1, const massiveinteger& m2);

#endif
