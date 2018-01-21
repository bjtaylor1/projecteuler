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


template<class It>
vector<int> repetitions(const It start, const It end)
{
	int i = 1;
	vector<int> retval;
	for (It it1 = start, it2 = start; ++it2 != end; it1++)
	{
		if (*it1 != *it2)
		{
			retval.push_back(i);
			i = 1;
		}
		else i++;
	}
	retval.push_back(i);
	return retval;
}

vector<int> repetitionss(const string& s)
{
	return repetitions(s.begin(), s.end());
}

mpz_class words_starting_with2(string phraseLeft, const string& word, const int& maxWordLength)
{
	string rem;
	set_difference(phraseLeft.begin(), phraseLeft.end(), word.begin(), word.end(), back_inserter(rem));
	mpz_class count(1);

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
	auto v0 = repetitionss("aabbb");
	auto v1 = repetitionss("aabb");
	auto v2 = repetitionss("abb");
	auto v3 = repetitionss("a");

	cout << words_starting_with2("aaab", "ab", 3) << " should be 3" << endl;
	cout << words_starting_with("aaab", "b", 4) << " should be 4" <<  endl;
	cout << words_starting_with("aaab", "a", 4) << " should be 9" << endl;
	cout << words_starting_with("aaab", "a", 3) << " should be 6" << endl;
	
	cout << words_starting_with("aaab", "ab", 4) << " should be 3" << endl;
	cout << words_starting_with("abcd", "ab", 4) << " should be 5" << endl;
    return 0;
}

