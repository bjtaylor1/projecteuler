// euler580.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

int is_prime_2_64(unsigned long long a);

string get_representation(long long hs, long long hsm)
{
	stringstream ss;
	ss << hs << " x " << (hsm / hs);
	return ss.str();
}

#define MAXMEM 10000000


int main(int argc, char** argv)
{
	if (argc < 2) return 1;
	long long limit = stoll(argv[1]);
	bool* is_k_hs = new bool[MAXMEM];
	long long kmax = floor((limit - 1) / 4);
	long long partitions = ceil(((double)kmax) / MAXMEM);
	long long h;
	long long tot = 0;
	for (long long partition = 0; partition < partitions; partition++)
	{
		long long kmin = partition * MAXMEM;
		memset(is_k_hs, 0, sizeof(bool) * MAXMEM);
		for (long long k = kmin; k < kmin + MAXMEM; k++)
		{
			long long h = 4 * k + 1;
			long long hs = h*h;
			for (long long hsm = hs; hsm < limit; hsm += hs)
			{
				if ((hsm - 1) % 4 == 0)
				{
					long long k_of_hsm = (hsm - 1) / 4;
					
					if (hsm > MAXMEM)
					{
						cout << "reached memory limit" << endl;
					}
					is_k_hs[hsm - kmin] = true;
				}
			}
		}
		long long h;
		for (long long k = 1; (h = (4*k + 1)) < MAXMEM; k++)
		{
			if (!is_k_hs[4*k + 1]) tot++;
		}
	}

	cout << tot << endl;
	//string** is_hs = new string*[limit];

	//memset(is_hs, 0, limit * sizeof(string*));
	//long long h;
	//for (long long k = 1; (h = (4LL * k + 1LL)) < limit; k++)
	//{
	//	long long hs = h*h;
	//	for (long long hsm = hs; hsm < limit; hsm += hs)
	//	{
	//		string representation = get_representation(hs, hsm);
	//		if (is_hs[hsm])
	//		{
	//			is_hs[hsm]->append(" = " + representation);
	//			cout << *is_hs[hsm] << endl;
	//		}
	//		else
	//		{
	//			is_hs[hsm] = new string(representation);
	//		}
	//	}
	//}
	//long long tot = 0;
	//bool* is_hs_byp = new bool[limit];
	//memset(is_hs_byp, 0, limit * sizeof(bool));
	//for (long long k = 1; (h = (4 * k + 1)) < limit; k++)
	//{
	//	long long hs = h*h;
	//	long long phs;
	//	for (long long p = 1; p < ceil(limit / hs); p++)
	//	{
	//		long long phs = p*hs;
	//		if ((p == 1 ||is_prime_2_64(p)) && phs < limit)
	//		{
	//			if (is_hs_byp[phs])
	//			{
	//				cout << p << "x" << h << "^2 = " << phs << " already marked by another prime!" << endl;
	//			}
	//			is_hs_byp[phs] = true;
	//			if (is_hs[phs] == NULL)
	//			{
	//				cout << p << "x" << h << "^2 = " << phs << " marked by prime but not by other!" << endl;
	//			}
	//		}
	//	}
	//}

	//for (long long k = 0; (h = (4 * k + 1)) < limit; k++)
	//{
	//	if (is_hs[h])
	//	{
	//		if (is_hs_byp[h] == NULL) cout << h << " missed!" << endl;
	//		tot++;
	//		delete (is_hs[h]);
	//	}
	//}
	//delete[] is_hs;
	//cout << tot << endl;
 //   return 0;
}

