#include "stdafx.h"
using namespace std;
mpz_class NumWords(const int& N /* = distinct letter count */, const int& maxWordLength)
{
	mpz_class nfact;
	mpz_fac_ui(nfact.get_mpz_t(), N);
	mpz_class tot;
	for (int i = 1; i <= maxWordLength; i++)
	{
		//combinations of this number of letters * permutations of that combination.
		// = N!/(k!(N-k)!) * k! = N!/(N-k)!
		mpz_class c, b;
		mpz_fac_ui(b.get_mpz_t(), N - i);
		mpz_divexact(c.get_mpz_t(), nfact.get_mpz_t(), b.get_mpz_t());
		tot += c;
	}
	return tot;
}

void wordindex(map<char,int> available, const string& word, const string::const_iterator& pos, const string::const_iterator& end, mpz_class& tot, const int& maxWordLength)
{
	mpz_class numwords = NumWords(available.size(), maxWordLength);

}

mpz_class P(const string& phrase, const string& word, const int& maxWordLength)
{
	map<char, int> available;
	for (auto c : phrase) available[c]++;
	map<char, int> pool(available);
	mpz_class tot;
	int n = 0;
	for (auto it = word.begin(); it != word.end(); it++, n++)
	{
		string wordleft(it, word.end());
		mpz_class numwords = NumWords(pool.size(), maxWordLength - n);
		map<char, int> indexes;
		int i = 0;
		for (auto c : pool) indexes[c.first] = i++;
		int index = indexes[*it];
		mpz_class groupsize;
		mpz_divexact_ui(groupsize.get_mpz_t(), numwords.get_mpz_t(), pool.size());
		tot += index * groupsize;
		if (--pool[*it] == 0) pool.erase(*it);
	}
	tot += word.size() - 1;
	return tot;
}

int main()
{

	cout << P("abc", "cba",  3) << endl;
    return 0;
}

