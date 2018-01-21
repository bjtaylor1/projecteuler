#include "stdafx.h"
using namespace std;
mpz_class num_words(const int& N /* = distinct letter count */, const int& maxWordLength)
{
	mpz_class nfact;
	mpz_fac_ui(nfact.get_mpz_t(), N);
	mpz_class tot;
	for (int i = 1; i <= maxWordLength && i <= N; i++)
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

bool is_available(const string& phrase, string word)
{
	sort(word.begin(), word.end());
	return includes(phrase.begin(), phrase.end(), word.begin(), word.end());
}

mpz_class words_starting_with(map<char, int> available, string used, const char c, const int& maxWordLength)
{
	if (available.find(c) == available.end()) return 0;

	if (--available[c] == 0) available.erase(c);
	used += c;

	mpz_class tot(1);
	for (auto dc : available)
	{
		tot += 1 + words_starting_with(available, used, c, maxWordLength - 1);
	}


	for (int i = 0; i < 4 - maxWordLength; i++)
	{
		cout << " ";
	}
	cout << used << " -> " << tot << endl;

	return tot;
}

mpz_class words_starting_with(map<char,int> available, const string& word, const int& maxWordLength)
{
	for (auto c : word)
	{
		if (--available[c] == 0) available.erase(c);
	}

	mpz_class tot(1);
 	for (auto dc : available)
	{
		tot += 1 + words_starting_with(available, "", dc.first, maxWordLength - 1);
	}
	return tot;
}



mpz_class words_starting_with(string phraseLeft, const string& word, const int& maxWordLength)
{
	map<char, int> available;
	if (maxWordLength == 0) return 0;
	for (auto subword : word)
	{
		auto it = phraseLeft.find(subword);
		if (it == string::npos) return 0;
		phraseLeft = phraseLeft.erase(it, 1);
	}
	string distinctchars;
	unique_copy(phraseLeft.begin(), phraseLeft.end(), back_inserter(distinctchars));
	mpz_class count(1);
	for (auto c : distinctchars)
	{
		string subword(1, c);
		count += words_starting_with(phraseLeft, subword, maxWordLength - word.size());
	}
	return count;
}

mpz_class P(string phrase, const string& word, const int& maxWordLength)
{
	sort(phrase.begin(), phrase.end());
	map<char, int> available;
	for (auto c : phrase) available[c]++;
	map<char, int> indexes;
	map<int, char> chars;
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
			string lw = word.substr(0, charpos) + chars[lastindex];
			if(is_available(phrase, lw))
			{
				cout << lw << "..." << endl;
			}
		}
		if(charpos > 0) cout << word.substr(0, charpos) << endl;
	}
	return 0;
}

int main()
{
	string phrase = "aaab";

	cout << words_starting_with(phrase, "b", 4) << endl;
    return 0;
}

