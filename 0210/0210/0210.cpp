// 0210.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

long i(long r)
{
	double p = r / 4; //e.g. 6, 250,000,000
	double cr = (p / 2)*sqrt(2);
	long crsq = (p * p) / 2;
	long totalpoints = 0;
	double x1; // = (x - p/2)^2
	cout << endl;
	for (long y = 1; (x1 = crsq - pow(y - p / 2, 2)) > 0; y++)
	{
		double xl = p / 2 - sqrt(x1);
		double xr = y > p ? p / 2 + sqrt(x1) : y;
		long points = ceil(xr) - floor(xl) - 1;
		totalpoints += points;
		cout << "   " << y << ": " << points << endl;
	}

	return totalpoints;
}

int main()
{
	long r = 28;

	for (long r = 4; r <= 36; r += 4)
	{
		cout << "r = " << r << ", i = " << i(r) << endl;
	}

    return 0;
}

