#include "stdafx.h"
using namespace std;

vector<int> cache_key(const string& phraseleft)
{
	map<char, int> chars;
	for (auto c : phraseleft) chars[c]++;
	vector<int> counts;
	for (auto c : chars) counts.push_back(c.second);
	sort(counts.begin(), counts.end());
	return counts;
}

mpz_class words_starting_with(const string& phrase, const string& word, const int& maxWordLength, map<vector<int>, mpz_class>& cache)
{
	if (word.size() > maxWordLength) return 0;
	map<char, int> available;
	if (maxWordLength <= 0) return 0;
	string sortedWord = word;
	sort(sortedWord.begin(), sortedWord.end());
	if (!includes(phrase.begin(), phrase.end(), sortedWord.begin(), sortedWord.end()))
	{
		return 0;
	}
	string phraseLeft;
	set_difference(phrase.begin(), phrase.end(), sortedWord.begin(), sortedWord.end(), back_inserter(phraseLeft));

	vector<int> cachekey = cache_key(phraseLeft);
	auto cached = cache.find(cachekey);
	if (cached != cache.end())
	{
		return cached->second;
	}

	string distinctchars;
	unique_copy(phraseLeft.begin(), phraseLeft.end(), back_inserter(distinctchars));
	mpz_class count(1);
	for (auto c : distinctchars)
	{
		string subword(1, c);
		count += words_starting_with(phrase, word + subword, maxWordLength, cache);
	}
	
	cache[cachekey] = count;
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
	mpz_class result(1);
	map<vector<int>, mpz_class> cache;
	for (int charpos = word.size() - 1; charpos >= 0; charpos--)
	{
		for (int lastindex = indexes[word[charpos]] - 1; lastindex >= 0; lastindex--)
		{
			string lw = word.substr(0, charpos) + chars[lastindex];
			mpz_class n = words_starting_with(phrase, lw, maxWordLength, cache);
			result += n;
		}
		if (charpos > 0)
		{
			result++;
		}
	}
	return result;
}

//string basephrase = "thereisasyetinsufficientdataforameaningfulanswer";
string basephrase = "cbaa";
int maxlen;
void test(const string& word)
{
	cout << "P(" << word << ") = " << P(basephrase, word, maxlen) << endl;
}

void euler_test()
{
	basephrase = "thereisasyetinsufficientdataforameaningfulanswer";
	maxlen = 15;
	test("euler");
	test("eulera");
	test("ywuuttttssssrrr");
	test("aaaaaacdeeeeeef");
	test("aaaaaacdeeeeeeg");
	test("aaaaaacdeeeeeeh");
	test("aaaaaacdeeeeeey");
	test("aaaaaacdeeeeef");
	test("aaaaaacdeeeeefe");
	//test("euleoywuttttsss");
}

void simple_test()
{
	basephrase = "cbaa";
	maxlen = 3;
	test("a");
	test("aa");
	test("aab");
	test("aac");
	test("ab");
	test("aba");
	test("abc");
	test("ac");
	test("aca");
	test("acb");
	test("b");
	test("ba");
	test("baa");
	test("bac");
	test("bc");
	test("bca");
	test("c");
	test("ca");
	test("caa");
	test("cab");
	test("cb");
	test("cba");
}

int main()
{
	euler_test();
	return 0;
}

