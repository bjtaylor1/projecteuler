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

int main(int argc, char** argv)
{
	if (argc < 2) return 1;
	long long limit = stoll(argv[1]);
	long long kmax = (limit - 1) / 4;
	bool* is_hs = new bool[kmax];
	memset(is_hs, 0, kmax * sizeof(bool));
	long long h;
	for (long long k = 1; (h = (4 * k + 1)) < limit; k++)
	{
		long long hs = h*h;
		for (long long hsm = hs; hsm < limit; hsm += hs)
		{
			if (((hsm - 1) % 4) == 0)
			{
				long long kofhsm = (hsm - 1) / 4;
				is_hs[kofhsm] = true;
			}
		}
	}
	long long tot = 0;
	for (long long k = 0; (h = (4 * k + 1)) < limit; k++)
	{
		if (!is_hs[k])
		{
			tot++;
		}
	}
	delete[] is_hs;
	cout << tot << endl;
    return 0;
}

