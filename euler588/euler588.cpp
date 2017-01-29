// euler588.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

void addterms(long* ts, long thisTerm, const vector<long>& terms, long kr)
{
	if (kr == 0)
	{
		ts[thisTerm]++;
	}
	else
	{
		for (auto l : terms)
		{
			addterms(ts, thisTerm + l, terms, kr - 1);
		}
	}
}

void printterms(long k)
{
	long* ts = new long[k * 4 + 1];
	memset(ts, 0, (k*4+1) * sizeof(long));
	vector<long> terms({4,3,2,1,0});
	addterms(ts, 0, terms, k);
	cout << k << ":      ";
	for (long k0 = 0; k0 <= k*4; k0++)
	{
		cout << ts[k0] << " ";
	}
	cout << endl;
	delete[] ts;
}

int main()
{
	for (long k = 2; k < 10; k++)
	{
		printterms(k);
	}
    return 0;
}

