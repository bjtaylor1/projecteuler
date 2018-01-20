// euler49.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

template<class T, class Tit>
void make_diffs(const Tit& it1, const Tit& end , multimap<T,int>& diffs)
{
	for (Tit it2 = it1; ++it2 != end;)
	{
		T diff = *it2 - *it1;
		diffs.insert(pair<T,int>(diff,1));
		make_diffs(it2, end, diffs);
	}
}

template<class T, class Tit>
void make_seq(const Tit& it1, const Tit& end, const T& reqdiff, set<T>& seq)
{
	for (Tit it2 = it1; ++it2 != end;)
	{
		auto diff = *it2 - *it1;
		if (diff == reqdiff) seq.insert(*it1);
		make_seq(it2, end, reqdiff, seq);
	}
}

template<class T>
bool get_arithmetic_sequence(const set<T>& s, int min, set<T>& seq)
{
	multimap<T,int> diffs;
	make_diffs(s.begin(), s.end(), diffs);
	for (auto k : diffs)
	{
		if (diffs.count(k.first) >= min)
		{
			make_seq(s.begin(), s.end(), k.first, seq);
			return true;
		}
	}
	return false;
}

int main()
{
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	for (mpz_class p(1009); p < 10000; mpz_nextprime(p.get_mpz_t(), p.get_mpz_t()))
	{
		if (p == 1487)
		{
			cout << "test" << endl;
		}
		stringstream ss;
		ss << p;
		string ps = ss.str();
		sort(ps.begin(), ps.end());
		if (ps[0] == '0') continue;
		
		set<mpz_class> sequence;
		do
		{
			mpz_class r(ps);
			if (mpz_probable_prime_p(r.get_mpz_t(), randstate, 10, 0))
			{
				auto inserted = sequence.insert(r);
				mpz_class diff = 0;
			}
		} while (next_permutation(ps.begin(), ps.end()));

		set<mpz_class> seq;
		if (get_arithmetic_sequence(sequence, 3, seq))
		{
			for (auto s : seq) cout << s << endl;
			return 0;
		}
	}
    return 0;
}

