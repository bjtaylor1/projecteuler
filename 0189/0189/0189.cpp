// 0189.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define MAXROWS 8
#define MAXCOLS ((MAXROWS*2)-1)
#define RED 0
#define GREEN 1
#define BLUE 2
#define NUMCOLOURS 3
using namespace std;

class pos
{
public:
	long y, x;
	pos(long _y, long _x) : y(_y), x(_x) {}
	pos& operator++()
	{
		if (++x > y * 2)
		{
			x = 0;
			y++;
		}
		return *this;
	}
	pos& operator--()
	{
		if (--x < 0)
		{
			x = (--y) * 2;
		}
		return *this;
	}
};

template<class T, class TIt>
map<T, long> get_orders(TIt current, TIt end)
{
	map<T, long> orders;
	set<T> values(current, end);
	long order = 0;
	bool start = false;
	TIt prev = current;
	while (orders.size() < values.size())
	{
		while (orders.size() > 0 && *current == *prev)
			current++;
		orders[*current] = order++;
		prev = current;
	}
	return orders;
}

class triangle
{
public:
	long colours[MAXROWS][MAXCOLS]; //from top down, i.e. row 0 has only got 1
	map<string, mpz_class> cache;

	string getcachekey(const pos& p) const
	{
		vector<long> markers;
		stringstream retval;
		retval << p.x << "," << p.y << ",";
		long y = p.y;
		pos mp(p);
		--mp;
		for(;mp.y >= 0 && (mp.y == p.y || (mp.y == p.y - 1 && mp.x >= p.x - 1)); --mp)
		{
			if ((mp.y == p.y && mp.x == p.x - 1) //the one to the left always matters, odd or even, as its right edge is exposed
				|| mp.x % 2 == 0) //and all even ones matter as their bottom edge is exposed.
			{
				markers.push_back(colours[mp.y][mp.x]);
			}
		}
		auto orders = get_orders<long, vector<long>::const_iterator>(markers.begin(), markers.end());
		for (auto m : markers)
		{
			retval << m;
		}
		string cachekey = retval.str();
		return cachekey;
	}

	mpz_class colourings(const pos& p)
	{
		if (p.y >= MAXROWS) return 1;
		auto cachekey = getcachekey(p);
		auto cached = cache.find(cachekey);
		if (cached != cache.end())
		{
			return cached->second;
		}

		mpz_class tot;
		set<long> validcolours({ RED, GREEN, BLUE });
		if (p.x > 0)
		{
			validcolours.erase(colours[p.y][p.x - 1]);//the one to the left
			if (p.x % 2 == 1)// it's 'upside down', so look at the one above as well
			{
				validcolours.erase(colours[p.y - 1][p.x - 1]);
			}
		}

		for (auto c : validcolours)
		{
			colours[p.y][p.x] = c;
			pos np(p);
			tot += colourings(++np);
		}
		cache[cachekey] = tot;
		return tot;
	}
};

int main()
{
	triangle t;
	cout << t.colourings(pos(0,0)) << endl;
    return 0;
}

