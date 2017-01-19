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

void make_triples(long pmax)
{
	long long* a = new long long[pmax];
	for (long long i = 0; i < pmax; i++)
	{
		a[i] = i*i;
	}
	for (long i = 1; i < pmax; i++)
	{
		if (i % 1000 == 0) cout << "\r" << (((double)i) / pmax);
		long j = 1, k = i;
		while (j < k)
		{
			if (a[j] + a[k] == a[i])
			{
				//if (util<long>::gcd(i, j, k) == 1)
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
	cout << endl;
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
	return triplesMap.find(side)->second;


	//for (long f = 3; f < side; f++)
	//{
	//	if (side % f == 0)
	//	{
	//		auto factorsTriples = triplesMap.find(f);
	//		if (factorsTriples != triplesMap.end())
	//		{
	//			long d = side / f;
	//			for (auto triple : factorsTriples->second)
	//			{
	//				result.insert(pair<long, long>({ triple.first * d, triple.second * d }));
	//			}
	//		}
	//	}
	//}
	return result;
}

long long ptot = 0;
long nmax;
void process_envelopes(long AE, long AB, long AD)
{
	auto BYBXes = get_triples_including_nonprimitive(AE);
	for (const pair<long, long>& BYBX : BYBXes)
	{
		long BY = BYBX.first;
		if (BY > AB) break; //they should be sorted, so no point continuing, as the rest will be 'overflap' as well
		long BX = BYBX.second;
		if (BX % 2 == 1) continue; // BC needs to be integral
		long AC2 = hyp(AE, (2 * AB) + BY);
		if (AC2 != -1 && AC2 % 2 == 0)
		{
			long p = AE + (2 * AB) + BX;
			if (p <= nmax)
			{
				long r = 1;// nmax / p;
				long pThis = p * r * (r + 1) / 2;
				ptot += pThis;
				cout << AE << "," << AB << "," << (BX / 2) << " = " << p << ", " << r << " times = " << pThis << endl;
			}
			//it's a heron
		}
	}
}

int main(int argc, char** argv)
{
	if (argc < 2) return 1;

	long long tot = 0;
	nmax = stoi(argv[1]);
	make_triples(nmax);
	make_triples_map();

	for (const pair<long, set<pair<long, long>>> AEset : triplesMap)
	{
		long AE = AEset.first;
		for (const pair<long, long>& ABAD : AEset.second)
		{
			long AB = ABAD.first;
			long AD = ABAD.second;
			process_envelopes(AE, AB, AD);
			//process_envelopes(AB, AE, AD);
		}
	}

	cout << ptot << endl;
	return 0;
}

