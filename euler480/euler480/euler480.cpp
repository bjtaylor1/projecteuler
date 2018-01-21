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

template<class It>
mpz_class arrangements(It begin, It end, int size)
{
	string wordtest(begin, end);
	auto reps = repetitions(begin, end);
	mpz_class combs;
	mpz_fac_ui(combs.get_mpz_t(), size);
	for (auto r : reps)
	{
		mpz_class fr;
		mpz_fac_ui(fr.get_mpz_t(), r);
		mpz_divexact(combs.get_mpz_t(), combs.get_mpz_t(), fr.get_mpz_t());
	}
	return combs;
}

mpz_class words_starting_with2(const string& phraseLeft, string word, const int& maxWordLength)
{
	string rem;
	set<string> words; //distinct words checked
	sort(word.begin(), word.end());
	set_difference(phraseLeft.begin(), phraseLeft.end(), word.begin(), word.end(), back_inserter(rem));
	mpz_class tot = 1;
	for (auto start = rem.begin(); start != rem.end(); start++)
	{
		auto last = start;
		for (int size = 1; size <= maxWordLength - word.size(); size++)
		{
			++last;
			string wordtest(start, last);
			if (words.insert(wordtest).second)
			{
				mpz_class arr = arrangements(start, last, size);
				cout << arr << " arrangements of " << wordtest << endl;
				tot += arr;
			}
			if (last == rem.end()) break;
		}
	}
	return tot;
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
			if(is_available(phrase, lw))
			{
				mpz_class n = words_starting_with2(phrase, lw, maxWordLength);
				if(word == "b") cout << n << " starting with " << lw << endl;
				result += n;
			}
		}
		if (charpos > 0)
		{
			result++;
		}
	}
	return result;
}

//string basephrase = "thereisasyetinsufficientdataforameaningfulanswer";
string basephrase = "aabc";

void test(const string& word)
{
	cout << "P(" << word << ") = " << P(basephrase, word, 15) << endl;
}

int main()
{
	cout << words_starting_with2("aabc", "a", 15) << endl;
	return 0;
	test("a");
	test("aa");
	test("aab");
	test("aabc");
	test("aac");
	test("aacb");
	test("ab");
	test("aba");
	test("abac");
	test("abc");
	test("abca");
	test("ac");
	test("aca");
	test("acab");
	test("acb");
	test("acba");
	test("b");
	test("ba");
	test("baa");
	test("baac");
	test("bac");
	test("baca");
	test("bc");
	test("bca");
	test("bcaa");
	test("c");
	test("ca");
	test("caa");
	test("caab");
	test("cab");
	test("caba");
	test("cb");
	test("cba");
	test("cbaa");


	//test("aaaaaacdee");
	//test("aaaaaacdeeeeefe");
	//test("aaaaaacdeeeeeey");
	//cout << words_starting_with2("aaab", "ab", 4) << " should be 3" << endl;
    return 0;
}

