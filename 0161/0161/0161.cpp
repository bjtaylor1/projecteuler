// 0181c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
#define W 9
#define H 12

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
	tile() : populated(0), shapecount(0)
	{
		maxxs.push(-1);
		maxys.push(-1);
		memset(b, 0, sizeof(bool) * W * H);
	}
	bool b[H][W];
	stack<long> maxxs;
	stack<long> maxys;
	long populated, shapecount;
	vector<shape> shapes;

	bool fits(const shape& s)
	{
		for (auto p : s.poses)
		{
			if (p.X < 0 || p.X >= W || p.Y < 0 || p.Y >= H
				|| b[p.Y][p.X])
				return false;
		}
		return true;
	}

	void add(const shape& s)
	{
		long maxx = maxxs.top(), maxy = maxys.top();
		for (auto p : s.poses)
		{
			b[p.Y][p.X] = true;
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
			b[p.Y][p.X] = false;
		}
		populated -= s.poses.size();
		shapecount--;
		maxxs.pop();
		maxys.pop();
		shapes.erase(find(shapes.begin(), shapes.end(), s));
	}
};

tile t;
long total = 0;
set<set<shape>> shapesets;

void render(const vector<shape>& shapes)
{
	int b[H][W];
	int i = 0;
	for (auto s : shapes)
	{
		for (auto p : s.poses)
		{
			b[p.Y][p.X] = i;
		}
		i++;
	}
	for (long y = 0; y < H; y++)
	{
		for (long x = 0; x < W; x++)
		{
			cout << b[y][x];
		}
		cout << endl;
	}
}

stack<long> xstack, ystack;
long x = 0, y = 0;
void gettiles()
{
	while (y < H && x < W)
	{
		if (!t.b[y][x])
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
						//cout << endl;
						//render(t.shapes);
						//if (!shapesets.insert(set<shape>(t.shapes.begin(), t.shapes.end())).second)
						//{
						//	cout << "(already seen!)" << endl;
						//}
					}
					else
						gettiles();

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

int main()
{
	gettiles();
	cout << total << endl;
	//cout << shapesets.size() << endl;
}

