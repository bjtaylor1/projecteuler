// 0209.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef unsigned long long btt;
typedef unsigned long long ull;
#define BIT(i,bitnum) ((i >> bitnum) & 1)
using namespace std;

int main()
{
	btt upperLimit = ((btt)-1);
	ull count = 0;
	for (btt b = 0; b < upperLimit; b++)
	{
		if (b % 100000000 == 0) cout << b << " : " << upperLimit << "\r" << endl;
		if (BIT(b, 0) & BIT(b, 0) == 0 &&
			BIT(b, 2) & BIT(b, 1) == 0 &&
			BIT(b, 4) & BIT(b, 2) == 0 &&
			BIT(b, 6) & BIT(b, 3) == 0 &&
			BIT(b, 8) & BIT(b, 4) == 0 &&
			BIT(b, 10) & BIT(b, 5) == 0 &&
			BIT(b, 12) & BIT(b, 6) == 0 &&
			BIT(b, 14) & BIT(b, 7) == 0 &&
			BIT(b, 16) & BIT(b, 8) == 0 &&
			BIT(b, 18) & BIT(b, 9) == 0 &&
			BIT(b, 20) & BIT(b, 10) == 0 &&
			BIT(b, 22) & BIT(b, 11) == 0 &&
			BIT(b, 24) & BIT(b, 12) == 0 &&
			BIT(b, 26) & BIT(b, 13) == 0 &&
			BIT(b, 28) & BIT(b, 14) == 0 &&
			BIT(b, 30) & BIT(b, 15) == 0 &&
			BIT(b, 32) & BIT(b, 16) == 0 &&
			BIT(b, 34) & BIT(b, 17) == 0 &&
			BIT(b, 36) & BIT(b, 18) == 0 &&
			BIT(b, 38) & BIT(b, 19) == 0 &&
			BIT(b, 40) & BIT(b, 20) == 0 &&
			BIT(b, 42) & BIT(b, 21) == 0 &&
			BIT(b, 44) & BIT(b, 22) == 0 &&
			BIT(b, 46) & BIT(b, 23) == 0 &&
			BIT(b, 49) & BIT(b, 24) == 0 &&
			BIT(b, 51) & BIT(b, 25) == 0 &&
			BIT(b, 53) & BIT(b, 26) == 0 &&
			BIT(b, 55) & BIT(b, 27) == 0 &&
			BIT(b, 57) & BIT(b, 28) == 0 &&
			BIT(b, 59) & BIT(b, 29) == 0 &&
			BIT(b, 61) & BIT(b, 30) == 0 &&
			BIT(b, 63) & BIT(b, 31) == 0 &&
			BIT(b, 1) & BIT(b, 32) == 0 &&
			BIT(b, 3) & BIT(b, 33) == 0 &&
			BIT(b, 5) & BIT(b, 34) == 0 &&
			BIT(b, 7) & BIT(b, 35) == 0 &&
			BIT(b, 9) & BIT(b, 36) == 0 &&
			BIT(b, 11) & BIT(b, 37) == 0 &&
			BIT(b, 13) & BIT(b, 38) == 0 &&
			BIT(b, 15) & BIT(b, 39) == 0 &&
			BIT(b, 17) & BIT(b, 40) == 0 &&
			BIT(b, 19) & BIT(b, 41) == 0 &&
			BIT(b, 21) & BIT(b, 42) == 0 &&
			BIT(b, 23) & BIT(b, 43) == 0 &&
			BIT(b, 25) & BIT(b, 44) == 0 &&
			BIT(b, 27) & BIT(b, 45) == 0 &&
			BIT(b, 29) & BIT(b, 46) == 0 &&
			BIT(b, 31) & BIT(b, 47) == 0 &&
			BIT(b, 33) & BIT(b, 48) == 0 &&
			BIT(b, 35) & BIT(b, 49) == 0 &&
			BIT(b, 37) & BIT(b, 50) == 0 &&
			BIT(b, 39) & BIT(b, 51) == 0 &&
			BIT(b, 41) & BIT(b, 52) == 0 &&
			BIT(b, 43) & BIT(b, 53) == 0 &&
			BIT(b, 45) & BIT(b, 54) == 0 &&
			BIT(b, 47) & BIT(b, 55) == 0 &&
			BIT(b, 48) & BIT(b, 56) == 0 &&
			BIT(b, 50) & BIT(b, 57) == 0 &&
			BIT(b, 52) & BIT(b, 58) == 0 &&
			BIT(b, 54) & BIT(b, 59) == 0 &&
			BIT(b, 56) & BIT(b, 60) == 0 &&
			BIT(b, 58) & BIT(b, 61) == 0 &&
			BIT(b, 60) & BIT(b, 62) == 0 &&
			BIT(b, 62) & BIT(b, 63) == 0)
			count++;
	}

	cout << count << endl;
    return 0;
}

