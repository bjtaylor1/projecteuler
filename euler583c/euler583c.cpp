// euler583c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "util.h"

using namespace std;

set<vector<long>> triples;
map<long, set<pair<long, long>>> triplesMap;

long hyp(long long a, long long b)
{
	long long h2 = (a*a) + (b*b);
	double hd = sqrt(h2);
	long long h = (long long)(hd + 0.5);
	return (long)(h*h == h2 ? h : -1);
}
long side(long long h, long long b)
{
	long long a2 = (h*h) - (b*b);
	double ad = sqrt(a2);
	long long a = (long long)(ad + 0.5);
	return (long)(a*a == a2 ? a : -1);
}

void make_triples(long pmax)
{
	long long* a = new long long[pmax];
	for (long long i = 0; i < pmax; i++)
	{
		a[i] = i*i;
	}
	for (long i = 1; i < pmax; i++)
	{
		//if (i % 1000 == 0) cout << "\r" << (((double)i) / pmax);
		long j = 1, k = i;
		while (j < k)
		{
			if (a[j] + a[k] == a[i])
			{
				if (util<long>::gcd(i, j, k) == 1)
				{
					vector<long> triple({ j,k,i });
					triples.insert(triple);
				}
				j++; k--;
			}
			else if (a[j] + a[k] < a[i]) j++;
			else if (a[j] + a[k] > a[i]) k--;
		}
	}
	delete[] a;
}



void make_triples_map()
{
	for (const vector<long>& triple : triples)
	{
		long AE = triple[0];
		long AB = triple[1];
		long AD = triple[2];
		triplesMap.insert(pair<long, set<pair<long, long>>>(AE, set<pair<long, long>>())).first->second.insert(pair<long, long>(AB, AD));
		triplesMap.insert(pair<long, set<pair<long, long>>>(AB, set<pair<long, long>>())).first->second.insert(pair<long, long>(AE, AD));
	}
}

set<pair<long, long>> get_triples_including_nonprimitive(long side)
{
	set<pair<long, long>> result;
	//if we have non primitiveS:
	//return triplesMap.find(side)->second;


	for (long f = 3; f < side; f++)
	{
		if (side % f == 0)
		{
			auto factorsTriples = triplesMap.find(f);
			if (factorsTriples != triplesMap.end())
			{
				long d = side / f;
				for (auto triple : factorsTriples->second)
				{
					result.insert(pair<long, long>({ triple.first * d, triple.second * d }));
				}
			}
		}
	}
	return result;
}

long nmax;

int main(int argc, char** argv)
{
	if (argc < 2) return 1;

	long long tot = 0;
	nmax = stoi(argv[1]);

	for (long long P = nmax; P >= 1; P-=2)
	{
		for (long long AE = 2; AE <= P / 2; AE+=2) //assumption: AE is even...
		{
			cout << "\r" << (((double)AE) / (P / 2));
			long long halfAE = AE / 2;
			long long ABmax = (P - (2 * AE)) / 2;
			for (long long AB = ABmax; AB >= 1; AB--) //by going down, the flap will grow - so we can break as soon as the flap is too big
			{
				long long AD = hyp(AB, AE);
				if (AD != -1)
				{
					long long twiceBC = (P - (2 * AB) - AE);
					if ((twiceBC % 2) == 0)
					{
						long long BC = twiceBC / 2;
						long long twiceBX = side(twiceBC, AE);
						if (twiceBX > 2 * AB) break;

						if (twiceBX != -1)
						{
							long long BX = twiceBX / 2;
							long long AC = hyp(halfAE, AB + BX);
							if (AC != -1)
							{
								cout << AE << "," << AB << "," << BC  << "," << AD << "," << AC << "," << P << endl;
								tot += P;
							}
						}
					}
				}
			}
		}
	}


	cout << tot << endl;
	return 0;
}

