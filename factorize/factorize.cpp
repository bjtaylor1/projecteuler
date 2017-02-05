// factorize.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2) return 1;
	long long n = stoll(argv[1]);
	for (long long f = 2; f <= n / 2; f++)
	{
		if (n%f == 0) cout << f << endl;
	}
}

