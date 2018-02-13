// 0223.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
#define N 25000000
#define T 16
#define BATCHSIZE 10

typedef unsigned long long ull;
atomic<long> result = 0;
map<ull, ull> squares;

atomic<long> processstartpointer = 0;
ull maxsquare;
mutex m;

pair<long, long> getnext()
{
	long start = processstartpointer.fetch_add(BATCHSIZE);
	if (start >= N / 2) return pair<long, long>(-1, -1);
	long end = start + BATCHSIZE;
	if (end > N / 2) end = N / 2;
	return pair<long, long>(start, end);
}

void process()
{
	for (pair<long, long> proc = getnext(); proc.first >= 0; proc = getnext())
	{
		cout << ".";
		for (ull a = proc.first; a < proc.second; a++)
		{
			ull asq = a * a;
			for (auto bsq = squares.lower_bound(asq + 1); bsq != squares.end(); bsq++)
			{
				auto csq = asq + bsq->first - 1;
				auto csqandc = squares.find(csq);
				if (csqandc != squares.end())
				{
					if (a + bsq->second + csqandc->second <= N)
					{
						result++;
					}
				}
			}
		}

	}
}

int main()
{

	long t = 0;
	for (long i = 1; i < (N / 2); i++)
	{
		squares.insert(pair<ull, ull>(i*i, i));
	}

	process();
	vector<thread> ts;
	for (long t = 0; t < T; t++)
	{
		ts.push_back(thread(process));
	}

	for (vector<thread>::iterator it = ts.begin(); it != ts.end(); it++)
	{
		it->join();
	}

	cout << "Result = " << result << endl;
	return 0;
}

