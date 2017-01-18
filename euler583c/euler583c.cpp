// euler583c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "util.h"

using namespace std;

set<vector<long>> triples;
map<long, set<pair<long, long>>> triplesMap;

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

void process_envelopes(long AE, long AB)
{
	for (const pair<long, long>& BYBX : triplesMap.find(AE)->second)
	{
		long BY = BYBX.first;
		if (BY > AB) break; //they should be sorted, so no point continuing, as the rest will be 'overflap' as well
		long BX = BYBX.second;
		if (BX % 2 == 1) continue; // BC needs to be integral

	}
}

int main(int argc, char** argv)
{
	if (argc < 2) return 1;

	long long tot = 0;
	long nmax = stoi(argv[1]);
	make_triples(nmax);
	make_triples_map();
		
	for (const pair<long, set<pair<long, long>>> AEAB : triplesMap)
	{

	}
	for (const vector<long>& triple : triples)
	{
		long AE = triple[0];
		long AB = triple[1];
		
	}
//	cout << "There are " << count << " triples. They are:" << endl;
	//for (const set<long>& triple : triples)
	//{
	//	vector<long> v(triple.begin(), triple.end());
	//	cout << v[0] << "," << v[1] << "," << v[2] << endl;
	//}
	return 0;
}

