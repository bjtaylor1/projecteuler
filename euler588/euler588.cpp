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

#define MAXTERM 4
void count_term(long long make, long long tot, vector<long long>& terms, set<vector<long long>>& sums, long long k, long long it)
{
	if (tot > make) return;
	else if (tot == make)
	{
		vector<long long> sum = terms;
		generate_n(back_inserter(sum), k - it, []() {return 0; }); //pad it with zeros
		//copy_if(terms.begin(), terms.end(), back_inserter(sum), [](long long i) -> bool {return i > 0; });
		sort(sum.begin(), sum.end());
		sums.insert(sum);
	}
	else if(it < k)
	{
		for (long long i = 0; i <= MAXTERM; i++)
		{
			tot += i;
			terms.push_back(i);
			count_term(make, tot, terms, sums, k, it + 1);
			terms.pop_back();
			tot -= i;
		}
	}
}

long long fact(long long n)
{
	if (n == 1 || n == 0) return 1;
	else return n*fact(n - 1);
}

long long get_coefficient(long long k, long long coeff)
{
	long long factk = fact(k);
	vector<long long> terms;
	set<vector<long long>> sums;
	count_term(coeff, 0, terms, sums, k, 0);
	long long result = 0;
	cout << endl;
	for (auto sum : sums)
	{
		set<long long> setunique(sum.begin(), sum.end());
		long divisor = 1;
		for (auto i : setunique)
		{
			long long countthis = count_if(sum.begin(), sum.end(), [i](long long n) -> bool { return n == i; });
			divisor *= fact(countthis);
		}
		long long constituent = factk / divisor;
		if (constituent % 2 == 1)
		{
			cout << coeff << ": " << factk << "/" << divisor << " = " << constituent << endl;
		}
		result += constituent;
	}
	return result;
}

int main(int argc, char** argv)
{
	if (argc < 2) return 1;
	long long k = stoi(argv[1]);
	long long countodd = 0;
	for (long long term = k * MAXTERM; term >= 0; term--)
	{
		long long coeff = get_coefficient(k, term);
		if (coeff % 2 == 1) countodd++;
	}

	cout << "count odd: " << countodd << endl;

    return 0;
}

