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
#define MEMMAX 10000000

	long partitions = ceil(((double)kmax) / MEMMAX);
	bool* is_hs = new bool[MEMMAX];
	memset(is_hs, 0, kmax * sizeof(bool));
	long long tot = 0;

	for (long partition = 0; partition < partitions; partition++)
	{
		long long h, kmin = partition * MEMMAX;
		for (long long k = kmin + 1; (h = (4 * k + 1)) < limit; k++)
		{
			long long hs = h*h;
			for (long long hsm = hs; hsm < limit; hsm += hs)
			{
				if (((hsm - 1) % 4) == 0)
				{
					long long kofhsm = ((hsm - 1) / 4) - kmin;
					if (kofhsm >= MEMMAX) break;
					is_hs[kofhsm] = true;
				}
			}
		}
		for (long long k = 0; k < MEMMAX && (h = (4 * k + 1)) < limit; k++)
		{
			if (!is_hs[k])
			{
				tot++;
			}
		}
	}
	delete[] is_hs;
	cout << tot << endl;
    return 0;
}

