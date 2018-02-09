// 0181c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
#define W 2
#define H 9

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
};

shape operator+(const shape& s, const pos& p)
{
	set<pos> newposes;
	for (auto sp : s.poses) newposes.insert(sp + p);
	return shape(newposes);
}

static bool operator<(const shape& s1, const shape& s2)
{
	return s1.poses < s2.poses;
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
	tile()
	{
		memset(b, 0, sizeof(bool) * W * H);
	}
	tile(const bool** source)
	{
		memcpy(b, source, sizeof(bool) * W * H);
	}
	bool b[H][W];
};

tile t;
long total = 0;
void gettiles()
{
	bool lastPopulated = true;
	for (long y = 0; y < H; y++)
	{
		if (lastPopulated)
		{
			long x = 0;
			while (x < W && t.b[y][x]) x++;
			lastPopulated = x > 0;

		}


	}
}

int main()
{


	return 0;
}

