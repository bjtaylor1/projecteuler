#include "stdafx.h"
#include "massiveinteger.h"

ostream & operator<<(ostream & os, const massiveinteger& m1)
{
	for (vector<int>::const_reverse_iterator it = m1.digits.rbegin(); it != m1.digits.rend(); it++)
	{
		os << *it;
	}
	return os;
}

massiveinteger operator +=(massiveinteger& m1, const massiveinteger& m2)
{
	m1 = m1 + m2;
	return m1;
}

massiveinteger operator+(const massiveinteger & m1, const massiveinteger & m2)
{
	vector<int> result;
	vector<int>::const_iterator it1 = m1.digits.begin(), it2 = m2.digits.begin();
	int remainder = 0;
	bool end1 = it1 == m1.digits.end(), end2 = it2 == m2.digits.end();
	for (bool carryon = true; carryon;)
	{
		long d1 = end1 ? 0 : *it1;
		long d2 = end2 ? 0 : *it2;
		long tot = d1 + d2 + remainder;
		if (tot == 0 && end1 && end2) carryon = false;
		else
		{
			int item = tot % 10;
			remainder = (tot - item) / 10;
			result.push_back(item);
			end1 = end1 || (++it1 == m1.digits.end());
			end2 = end2 || (++it2 == m2.digits.end());
		}
	}
	return massiveinteger(result);
}

massiveinteger operator*(const massiveinteger & m1, const massiveinteger & m2)
{
	massiveinteger total;
	vector<int> current = m1.digits;
	for (vector<int>::const_iterator it = m2.digits.begin(); it != m2.digits.end(); it++)
	{
		massiveinteger currentval(current);
		for (int i = 0; i < *it; i++)
		{
			total += currentval;
		}
		current.insert(current.begin(), 0);
	}
	return total;
}

void massiveinteger::truncate(size_t n)
{
	if (digits.size() >= n)
	{
		digits.erase(digits.begin() + n, digits.end());
	}
}

string massiveinteger::approx() const
{
	int count = 0;
	stringstream ss;
	for (vector<int>::const_reverse_iterator digit = digits.rbegin(); digit != digits.rend(); digit++)
	{
		if (count == 1) ss << "." << (*digit);
		else if (count < 4) ss << (*digit);
		else break;
		count++;
	}
	ss << "e" << (digits.size() - 1);
	return ss.str();
}

string massiveinteger::mod10e(long n) const
{
	stringstream ss;
	if (digits.size() <= n) ss << (*this);
	else
	{
		vector<int> digitsR(digits.rbegin(), digits.rend());
		digitsR.erase(digitsR.begin(), digitsR.begin() + (digitsR.size() - n));
		for (int digit : digitsR)
		{
			ss << digit;
		}
	}
	return ss.str();

}

string massiveinteger::all() const
{
	stringstream ss;
	vector<int> digitsR(digits.rbegin(), digits.rend());
	for (int digit : digitsR)
	{
		ss << digit;
	}
	return ss.str();

}