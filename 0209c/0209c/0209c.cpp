// 0209c.cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"
typedef unsigned long long ull;
const ull maxbits = 3; //e.g. 3, 6
const ull maxnum = 1 << maxbits; //e.g. 8, 256
const ull maskall = maxnum - 1; //e.g. 7, 255
const ull totnum = (1 << maxnum); //e.g. 2^8, 2^64
const ull exmaskall = totnum - 1; //e.g. 11111111, 111...(x64)

using namespace std;

pair<ull, ull> tts[maxnum];
ull exclusionmasks[maxnum];

ull getpartner(const ull& i)
{
	auto a = (i >> (maxbits-1) & 1);
	auto b = (i >> (maxbits-2) & 1);
	auto c = (i >> (maxbits-3)) & 1;
	auto axorbandc = a ^ (b&c);
	auto partner = ((i << 1) & maskall) | axorbandc;
	return partner;
}

void makecombinations(long min, long count, set<long>& current, set<set<long>>& combs)
{
	for (long i = min; i < maxnum; i++)
	{
		current.insert(i);
		if (current.size() == count)
		{
			combs.insert(current);
		}
		else
		{
			makecombinations(i + 1, count, current, combs);
		}
		current.erase(i);
	}
}

set<set<long>> getcombinations(long i, long count)
{
	set<set<long>> combs;
	set<long> current;
	current.insert(i);
	makecombinations(i + 1, count, current, combs);
	return combs;
}

ull bitcount(ull l)
{
	ull result = 0;
	while (l > 0)
	{
		if (l & 1 == 1) result++;
		l >>= 1;
	}
	return result;
}

long main()
{
	auto combs5 = getcombinations(3,3);
	for (ull i = 0; i < maxnum; i++)
	{
		tts[i].first = i;
		tts[i].second = getpartner(i);
		exclusionmasks[i] = exmaskall & (~(1 << tts[i].first)) & (~(1 << tts[i].second));
	}
	ull totalexclude = 0;
	for (ull i = maxnum - 1; i >= 0; i--)
	{
		ull exclude = 1 << bitcount(exclusionmasks[i]);
		for (long count = 2, excludeOrInclude = -1; count <= maxnum - i; count++, excludeOrInclude *= -1)
		{
			set<set<long>> combs = getcombinations(i, count);
			for (auto c : combs)
			{
				ull inclusionmodifier = exmaskall;
				for (auto l : c)
				{
					inclusionmodifier &= exclusionmasks[l];
				}
				long inclusionmod = excludeOrInclude * (1 << bitcount(inclusionmodifier));
				exclude += inclusionmod;
			}
		}
		totalexclude += exclude;
	}
	auto result = totnum - totalexclude;
	cout << result << endl;
    return 0;
}

