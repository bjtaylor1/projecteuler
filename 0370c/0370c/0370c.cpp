// 0370c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

typedef unsigned long long INTTYPE;
const INTTYPE P = (INTTYPE)25e12;
#define SQMAX 5000000
#define NT 1
const INTTYPE batch = SQMAX / NT;
INTTYPE squares[SQMAX];
atomic<INTTYPE> result = P/3;

atomic<INTTYPE> maxdiff(0);

void process(INTTYPE start)
{
	mpz_class mpz_a;
	
	for (INTTYPE roota = start + 1; roota <= start + batch; roota++)
	{
		for (INTTYPE rootc = roota + 1;; rootc += (2- (roota %2)))
		{
			INTTYPE a = squares[roota], c = squares[rootc];
			INTTYPE b = roota * rootc;
			if (a + b + c > P || c > a + b) break;
			mpz_set_ui(mpz_a.get_mpz_t(), a);
			if (mpz_gcd_ui(NULL, mpz_a.get_mpz_t(), c) == 1)
			{
				result += (P / (a + b + c));
			}
			if (rootc - roota > maxdiff)
			{
				maxdiff = rootc - roota;
				cout << maxdiff << " , a,b,c = " << a << "," << b << "," << c << endl;
			}
		}
	}
}

int main()
{
	for (INTTYPE r = 1; r < SQMAX; r++)
	{
		squares[r] = r * r;
	}
	vector<thread> threads;
	for (INTTYPE start = 0; start < SQMAX; start += batch)
	{
		threads.push_back(thread(process, start));
	}
	for (vector<thread>::iterator it = threads.begin(); it != threads.end(); it++)
	{
		it->join();
	}
	cout << result << endl;
    return 0;
}

