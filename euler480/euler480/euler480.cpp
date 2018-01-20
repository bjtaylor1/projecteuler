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
	map<char, int> indexes;
	map<int, char> chars;
	for (auto c : phrase) available[c]++;
	int i = 0;
	for (auto c : available)
	{
		chars[i] = c.first;
		indexes[c.first] = i++;
	}
	for (int charpos = word.size() - 1; charpos >= 0; charpos--)
	{
		for (int lastindex = indexes[word[charpos]] - 1; lastindex >= 0; lastindex--)
		{
			string lowerword = word.substr(0, charpos) + chars[lastindex];
			cout << lowerword << "..." << endl;
		}
		if(charpos > 0) cout << word.substr(0, charpos) << endl;
	}
	return 0;
}

int main()
{

	cout << P("abc", "abc",  3) << endl;
    return 0;
}

