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
		shape({pos(0,0), pos(0,1), pos(1,1)}),
		shape({pos(0,0), pos(-1,1), pos(0,1)}),
		shape({pos(0,0), pos(1,0), pos(0,1)}),
		shape({pos(0,0), pos(1,0), pos(1,1)}),
		shape({pos(0,0), pos(0,1), pos(0,2)}),
		shape({pos(0,0), pos(1,0), pos(2,0)})
	});

class tile
{
public:
	long W, H;
	tile(long w, long h) : W(w), H(h), populated(0), shapecount(0)
	{
		b = new bool[w*h];
		maxxs.push(-1);
		maxys.push(-1);
		memset(b, 0, sizeof(bool) * w * h);
	}
	~tile()
	{
		delete[] b;
	}
	bool* b;
	stack<long> maxxs;
	stack<long> maxys;
	long populated, shapecount;
	vector<shape> shapes;

	bool fits(const shape& s)
	{
		for (auto p : s.poses)
		{
			if (p.X < 0 || p.X >= W || p.Y < 0 || p.Y >= H
				|| b[p.Y * W + p.X])
				return false;
		}
		return true;
	}

	long row_code(long y) const //1s if empty. full row is zero
	{
		long rowcode = 0;
		for (long x = 0; x < W; x++)
			if (!b[y * W + x]) rowcode |= (1 << x);
		return rowcode;
	}

	pair<long, long> get_remaining(long fromy) const
	{
		long state = -1, code = 0, count = 0;
		for (long y = fromy; y < H; y++)
		{
			long rowcode = row_code(y);
			if (state == -1) //'before'
			{
				if (rowcode > 0)
				{
					state = 0;
					count++;
					code = rowcode;
				}
			}
			else if (state == 0) // 'during'
			{
				if (rowcode == 0)
				{
					state = 1;
				}
				else if (rowcode == code) count++;
				else return pair<long, long>(-1, -1); //not square (e.g. 0,3,2,...)
			}
			else if (state == 1) // 'after'
			{
				if (rowcode != 0) return pair<long, long>(-1, -1); //not square (e.g. 0,3,0,3,...)
			}
		}
		while (code & 1 == 0) code >>= 1;
		return pair<long, long>(code, count);
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
			b[p.Y* W + p.X] = true;
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
			b[p.Y* W + p.X] = false;
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
	long total;
	stack<long> xstack, ystack;
	long x, y;

	solver(long w, long h) : W(w), H(h), t(w, h), total(0), x(0), y(0) {};
	void gettiles()
	{
		while (y < H && x < W)
		{
			if (!t.b[y*W + x])
			{

				pos p(x, y);
				for (auto s : allshapes)
				{
					auto posshape = s + p;
					if (t.fits(posshape))
					{
						t.add(posshape);
						xstack.push(x);
						ystack.push(y);

						if (t.populated == W * H)
						{
							total++;
						}
						else
						{
							auto remaining = t.get_remaining(y);
							auto remainderpopulation = bitset<16>(remaining.first).count() * remaining.second;
							auto knownremainder = cache.end();
							if (t.populated + remainderpopulation == W * H)
							{
								knownremainder = cache.find(remaining);
							}

							if (knownremainder != cache.end())
							{
								cout << ".";
								//cout << "From: " << endl << t << "adding " << knownremainder->second << endl;
								total += knownremainder->second;
							}
							else
								gettiles();

						}

						x = xstack.top();
						y = ystack.top();
						xstack.pop();
						ystack.pop();
						t.remove(posshape);
					}
				}
			}
			if (++x >= W)
			{
				x = 0;
				y++;
			}
		}
	}

};

int main()
{
	for (long w = 2; w <= 12; w += 2)
	{
		for (long h = 3; h <= 9; h += 3)
		{
			solver s(w, h);
			s.gettiles();
			cache[s.t.get_total()] = s.total;
			cout << w << "," << h << " = " << s.total << endl;
		}
	}
}
