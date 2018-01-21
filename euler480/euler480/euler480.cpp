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

mpz_class words_starting_with(const string& phrase, const string& word, const int& maxWordLength)
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

	string distinctchars;
	unique_copy(phraseLeft.begin(), phraseLeft.end(), back_inserter(distinctchars));
	mpz_class count(1);
	for (auto c : distinctchars)
	{
		string subword(1, c);
		count += words_starting_with(phrase, word + subword, maxWordLength);
	}
	//cout << count << " starting with " << word << endl;
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
	for (int charpos = word.size() - 1; charpos >= 0; charpos--)
	{
		for (int lastindex = indexes[word[charpos]] - 1; lastindex >= 0; lastindex--)
		{
			string lw = word.substr(0, charpos) + chars[lastindex];
			mpz_class n = words_starting_with(phrase, lw, maxWordLength);
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

void test(const string& word)
{
	cout << "P(" << word << ") = " << P(basephrase, word, 3) << endl;
}

int main()
{
	//set<string> words;
	//make_combinations("aaaaaacdeeeeeef", words);
	//for (auto w : words) cout << w << endl;
	//return 0;

	//test("aaaaaacdeeeeeef");
	//test("aaaaaacdeeeeeeg");
	//test("aaaaaacdeeeeeeh");
	//test("aaaaaacdeeeeeey");
	//test("aaaaaacdeeeeef");
	//test("aaaaaacdeeeeefe");
	//test("euleoywuttttsss");
	//test("euler");
	//test("eulera");
	//return 0;
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


	//test("aaaaaacdee");
	//test("aaaaaacdeeeeefe");
	//test("aaaaaacdeeeeeey");
	//cout << words_starting_with2("aaab", "ab", 4) << " should be 3" << endl;
	return 0;
}

