// 614.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define min(x,y) (x < y ? x : y)
using namespace std;

int main()
{
	const int M = (1e9 + 7);
	int limit = 1e7;
	unsigned long long* w = new unsigned long long[limit+1];
	memset(w, 0, (limit+1) * sizeof(unsigned long long));
	w[0] = 1;
	int max = 3;
	long tot = 0;
	for (int i = 1; i <= limit; i++)
	{
		if (i % 100 == 0) cout << i << endl;

		if (i % 2 != 0 || i % 4 == 0)
		{
			for (int j = min(max,limit); j >= i; j--)
			{
				w[j] += w[j - i];
				tot = (tot + w[j - i]) % M;
			}
		}
		max += i;
	}
	cout << w[1000] << endl;
	delete[] w;
	cout << tot << endl;
    return 0;
}

