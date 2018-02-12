// 0223.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
#define N 25e6
typedef unsigned long long ull;
int main()
{
	map<ull,ull> squares;
	for (ull i = 1; i < (N / 2); i++)
	{
		squares.insert(pair<ull,ull>(i*i, i));
	}
	ull result = 0;
	for (auto asq : squares)
	{
		cout << asq.second << endl;
		for (auto bsq = squares.lower_bound(asq.first + 1); bsq != squares.end(); bsq++)
		{
			auto csqplus1 = asq.first + bsq->first;
			if (csqplus1 > squares.rbegin()->first)
			{
				break;
			}
			auto csq = csqplus1 - 1;
			double c = sqrt(csq);
			if (asq.second + bsq->second + c > N) break;
			if (squares.find(csq) != squares.end())
			{
				if(asq.second > 1) cout << asq.second << "," << bsq->second << "," << c << endl;
				if (asq.second + bsq->second <= c) throw runtime_error("Bollocks!");
				result++;
			}
		}
	}
	cout << result << endl;
	return 0;
}

