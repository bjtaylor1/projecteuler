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
		if (tot == 0) carryon = false;
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
