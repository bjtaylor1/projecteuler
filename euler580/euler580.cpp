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
	if (argc < 3) return 1;
	long long limit = stoll(argv[1]);
	long long MEMMAX = stoll(argv[2]);
	long long kmax = (limit - 1) / 4;

	long partitions = ceil(((double)kmax) / MEMMAX);
	bool* is_hs = new bool[MEMMAX];
	
	long long tot = 0;

	
	for (long partition = 0; partition < partitions; partition++)
	{
		cout << "\r" << setprecision(0) << (100.0 * (double)partition / partitions) << "%";
		memset(is_hs, 0, MEMMAX * sizeof(bool));
		long long h, kmin = partition * MEMMAX;
		for (long long k = 1; (h = (4 * k + 1)) < limit; k++)
		{
			cout << "\r" << setprecision(0) << (100.0 * (double)h / limit) << "%";
			long long hs = h*h;
			long long minm = (4 * kmin + 1) / hs;  //min multiple for this partition
			minm = MAX(minm, 1);
			for (long long hsm = hs * minm; hsm < limit; hsm += hs)
			{
				if (((hsm - 1) % 4) == 0)
				{
					long long kofhsm = ((hsm - 1) / 4) - kmin;
					if (kofhsm >= MEMMAX) break;
					if(kofhsm >= 0) is_hs[kofhsm] = true;
				}
			}
		}
		for (long long k = 0; k < MEMMAX && (h = (4 * (k+kmin) + 1)) < limit; k++)
		{
			if (!is_hs[k])
			{
				//cout << ((k+kmin)*4+1) << endl;
				tot++;
			}
		}
	}
	delete[] is_hs;
	cout << endl << tot << endl;
    return 0;
}

