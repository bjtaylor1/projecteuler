// 0210.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

typedef unsigned long long ull;

ull i(ull r)
{
	double p = r / 4; //e.g. 6, 250,000,000
	double cr = (p / 2)*sqrt(2);
	double crsq = (p * p) / 2;
	ull totalpoints = 0;
	double x1; // = (x - p/2)^2
	for (ull y = 1; (x1 = crsq - pow(y - p / 2, 2)) > 0; y++)
	{
		double xl = p / 2 - sqrt(x1);
		double xr = y > p ? p / 2 + sqrt(x1) : y;
		ull points = ceil(xr) - floor(xl) - 1;
		totalpoints += points;
	}

	return totalpoints;
}

ull tot(ull r)
{
	ull p = r / 4;
	ull tot = 2 * (12 * p*p + i(r));
	return tot;
}

int main()
{
	cout << tot(1000000000) << endl;
    return 0;
}

