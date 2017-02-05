// euler580.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

string get_representation(long long hs, long long hsm)
{
	stringstream ss;
	ss << hs << " x " << (hsm / hs);
	return ss.str();
}
#define MAX(x,y) (x > y ? x : y)

int main(int argc, char** argv)
{
	if (argc < 2) return 1;
	long long limit = stoll(argv[1]);
	set<long long> sh;
	long long tot = 0;
	for (long long k = 1; k < (limit - 1) / 4; k++)
	{
		long long h = 4 * k + 1;
		long long hs = h*h;
		for (long long hsm = hs; hsm < limit; hsm += hs)
		{
			if (((hsm - 1) %4) == 0 && sh.insert(hsm).second)
			{
				tot++;
			}
		}
	}
	cout << ((limit/4) - tot) << endl;

    return 0;
}

