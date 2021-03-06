// 0181c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

map<pair<long, long>, long> cache;

class pos
{
public:
	int X, Y;
	pos(int x, int y) : X(x), Y(y) {}
};

static pos operator+(const pos& p1, const pos& p2)
{
	return pos(p1.X + p2.X, p1.Y + p2.Y);
}


static bool operator <(const pos& l, const pos& r)
{
	if (l.Y != r.Y) return l.Y < r.Y;
	if (l.X != r.X) return l.X < r.X;
	return false;
}

class shape
{
public:
	shape(const set<pos>& _poses) : poses(_poses) {}
	set<pos> poses;
	friend bool operator==(const shape& s1, const shape& s2);
	friend bool operator<(const shape& s1, const shape& s2);

};

shape operator+(const shape& s, const pos& p)
{
	set<pos> newposes;
	for (auto sp : s.poses) newposes.insert(sp + p);
	return shape(newposes);
}

bool operator<(const shape& s1, const shape& s2)
{
	return s1.poses < s2.poses;
}

bool operator==(const shape& s1, const shape& s2)
{
	return !(s1.poses < s2.poses || s2.poses < s1.poses);
}

static set<shape> allshapes(
	{
		shape({ pos(0,0), pos(1,0), pos(0,1) }), // xx 
												 // x

		shape({pos(0,0), pos(1,0), pos(1,1)}),	 // xx
												 //  x

		shape({pos(0,0), pos(0,1), pos(1,1)}),	 // x
												 // xx

		shape({pos(0,0), pos(-1,1), pos(0,1)}),	 //  x
												 // xx

		shape({pos(0,0), pos(0,1), pos(0,2)}),	 // x
												 // x
												 // x

		shape({pos(0,0), pos(1,0), pos(2,0)})	 // xxx
	});

class tile
{
public:
	long W, H;
	tile(long w, long h) : W(w), H(h), populated(0)
	{
		mpz_class unsetval;
		mpz_ui_pow_ui(unsetval.get_mpz_t(), 2, w);
		unsetval--;
		for (long y = 0; y < h; y++)
		{
			mpz_set(empty[y].get_mpz_t(), unsetval.get_mpz_t());
		}
	}

	mpz_class get_key() const
	{
		mpz_class retval(0);

		for (long y = 0; y < H; y++)
		{
			retval |= (full[y] << (y*W));
		}
		return retval;
	}

	mpz_class full[12], empty[12];
	long populated;
	vector<shape> shapes;

	bool fits(const shape& s)
	{
		for (auto p : s.poses)
		{
			if (p.X < 0 || p.X >= W || p.Y < 0 || p.Y >= H
				|| get(p.X, p.Y))
				return false;
		}
		return true;
	}

	bool get(long x, long y)
	{
		return mpz_tstbit(full[y].get_mpz_t(), x);
	}

	void fill(long x, long y)
	{
		mpz_setbit(full[y].get_mpz_t(), x);
		mpz_clrbit(empty[y].get_mpz_t(), x);
	}

	void clr(long x, long y)
	{
		mpz_clrbit(full[y].get_mpz_t(), x);
		mpz_setbit(empty[y].get_mpz_t(), x);
	}

	void add(const shape& s)
	{
		for (auto p : s.poses)
		{
			fill(p.X, p.Y);
		}
		shapes.push_back(s);
		populated += s.poses.size();
	}

	void remove(const shape& s)
	{
		for (auto p : s.poses)
		{
			clr(p.X, p.Y);
		}
		populated -= s.poses.size();
		shapes.erase(find(shapes.begin(), shapes.end(), s));
	}
};

ostream& operator<<(ostream& os, const tile& t)
{
	char* i = new char[t.W * t.H];
	memset(i, (int)'x', sizeof(char) * t.W * t.H);
	char c = '0';
	for (auto s : t.shapes)
	{
		for (auto p : s.poses)
		{
			i[p.Y * t.W + p.X] = c;
		}
		c++;
	}
	for (long y = 0; y < t.H; y++)
	{
		for (long x = 0; x < t.W; x++)
		{
			os << i[y*t.W + x];
		}
		os << endl;
	}
	delete[] i;
	return os;
}

class solver
{
public:
	long W, H;
	tile t;
	map<mpz_class, mpz_class> cache;
	solver(long w, long h) : W(w), H(h), t(w, h) {};
	mpz_class gettiles(long x, long y, long depth)
	{
		mpz_class tot = 0;
		auto key = t.get_key();
		auto cached = cache.find(key);
		if (cached != cache.end())
		{
			return cached->second;
		}

		pos p(x, y);
		
		for (auto s : allshapes)
		{
			auto posshape = s + p;
			if (t.fits(posshape))
			{
				t.add(posshape);
				if (t.populated == W * H)
				{
					tot++;
				}
				else
				{
					int oldx = x, oldy = y;
					while (t.get(x, y))
					{
						if (++x >= W)
						{
							x = 0;
							y++;
						}
					}
					auto addition = gettiles(x, y, depth+1);
					x = oldx;
					y = oldy;
					tot += addition;
				}
				t.remove(posshape);
			}
		}
		cache[key] = tot;
		return tot;
	}
};

int main()
{
	solver s(9,12);
	cout << s.gettiles(0, 0, 0) << endl;

}
