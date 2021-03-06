// 0223.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
#define N 25000000
#define T 16
#define BATCHSIZE 10000
atomic<long> nextc(2);
typedef unsigned long long ull;
atomic<ull> result;
ull squares[N];

void process()
{
	for (long cstart = nextc.fetch_add(BATCHSIZE); cstart < N / 2; cstart = nextc.fetch_add(BATCHSIZE))
	{
		cout << ".";
		long end = cstart + BATCHSIZE;
		if (end > N / 2) end = N / 2;
		for (long c = cstart; c < end; c++)
		{
			long a = 1, b = c;
			while (a <= b)
			{
				if (squares[a] + squares[b] < squares[c] + 1)
					a++;
				else if (squares[a] + squares[b] > squares[c] + 1)
					b--;
				else
				{
					if (a + b + c <= N && a+b >c)
					{
						result++;
					}
					a++;
					b--;
				}
			}
		}
	}
}

int main()
{
	for (long c = 1; c < N / 2; c++)
	{
		squares[c] = c * c;
	}
	vector<thread> threads;
	for (long t = 0; t < T; t++)
	{
		threads.push_back(thread(process));
	}
	for (vector<thread>::iterator it = threads.begin(); it != threads.end(); it++)
	{
		it->join();
	}

	cout << result << endl;
	return 0;
}

