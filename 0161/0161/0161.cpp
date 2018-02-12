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
	tile(long w, long h) : W(w), H(h), populated(0), shapecount(0)
	{
		//b = new bool[w*h];
		mpz_class unsetval;
		mpz_ui_pow_ui(unsetval.get_mpz_t(), 2, w);
		unsetval--;
		for (long y = 0; y < h; y++)
		{
			mpz_set(empty[y].get_mpz_t(), unsetval.get_mpz_t());
		}
		maxxs.push(-1);
		maxys.push(-1);
		//memset(b, 0, sizeof(bool) * w * h);
	}
	//~tile()
	//{
	//	delete[] b;
	//}
	//bool* b;
	mpz_class full[12], empty[12];
	stack<long> maxxs;
	stack<long> maxys;
	long populated, shapecount;
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

	//long row_code(long y) const //1s if empty. full row is zero
	//{
	//	long rowcode = 0;
	//	for (long x = 0; x < W; x++)
	//		if (!b[y * W + x]) rowcode |= (1 << x);
	//	return rowcode;
	//}

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

	static bool is_contigious(const mpz_class& c)
	{
		auto first1 = mpz_scan1(c.get_mpz_t(), 0);
		auto next0 = mpz_scan0(c.get_mpz_t(), first1);
		auto iscontigious = next0 - first1 == mpz_popcount(c.get_mpz_t());
		return iscontigious;
	}

	pair<long, long> get_remaining(long fromy) const
	{
		long state = -1, count = 0;
		mpz_class code;
		for (long y = fromy; y < H; y++)
		{
			if (state == -1) //'before'
			{
				if (empty[y] > 0)
				{
					if (!is_contigious(empty[y])) return pair<long, long>(-1, -1);
					state = 0;
					count++;
					mpz_set(code.get_mpz_t(), empty[y].get_mpz_t());
				}
			}
			else if (state == 0) // 'during'
			{
				if (empty[y] == 0)
				{
					state = 1;
				}
				else if (empty[y] == code) count++;
				else return pair<long, long>(-1, -1); //not square (e.g. 0,3,2,...)
			}
			else if (state == 1) // 'after'
			{
				if (empty[y] != 0) return pair<long, long>(-1, -1); //not square (e.g. 0,3,0,3,...)
			}
		}
		//'normalize' it (makes cache key less unique for comparing larger vs smaller)
		//only return the bit count. we already know it's contigious
		return pair<long, long>(mpz_popcount(code.get_mpz_t()), count);
	}

	pair<long, long> get_total() const
	{
		return pair<long, long>(pow(2, W) - 1, H);
	}

	void add(const shape& s)
	{
		long maxx = maxxs.top(), maxy = maxys.top();
		for (auto p : s.poses)
		{
			fill(p.X, p.Y);
			if (p.X > maxx) maxx = p.X;
			if (p.Y > maxy) maxy = p.Y;
		}
		maxxs.push(maxx);
		maxys.push(maxy);
		shapes.push_back(s);
		populated += s.poses.size();
		shapecount++;
	}

	void remove(const shape& s)
	{
		for (auto p : s.poses)
		{
			clr(p.X, p.Y);
		}
		populated -= s.poses.size();
		shapecount--;
		maxxs.pop();
		maxys.pop();
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

	stack<long> xstack, ystack;
	stack<pair<bool, pair<long, long>>> degeneratexstack;
	long x, y, itcount;

	solver(long w, long h) : W(w), H(h), t(w, h), x(0), y(0), itcount(0) {};
	long gettiles(long x, long y, long depth)
	{
		long tot = 0;
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
					//cout << "Added one, it is now: " << endl << t << endl;
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
					if (depth == 0)
					{
						cout << "total " << endl << " = " << addition << endl;
					}
					tot += addition;
				}
				t.remove(posshape);
			}
		}
		return tot;
	}
};

int main()
{
	solver s(9,2);
	cout << s.gettiles(0, 0, 0) << endl;

}
