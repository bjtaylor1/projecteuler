// euler600.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hexagon.h"

using namespace std;

#define EPSILON 1e-12
#define ZERO(dbl) (fabs(dbl) <= EPSILON)
#define LT_ZERO(dbl) (dbl < -EPSILON)
#define GT_ZERO(dbl) (dbl > EPSILON)
bool checksides(const vector<long>& sides, long maxperim)
{
	double x = 0, y = 0;
	double angle = 0;
	long sidescounted = 0;
	long perimeterused = 0;
	for (auto side : sides)
	{
		x += side * cos(angle);
		y += side * sin(angle);
		if (LT_ZERO(y)) return false;
		perimeterused += side;
		++sidescounted;
		double dist = sqrt(x*x + y*y);
		long maxtogetback = maxperim - perimeterused;
		if (GT_ZERO(dist - maxtogetback)) return false; //we can't possibly get back in time

		angle += (M_PI / 3); //60 degrees
		
		if (sidescounted == 6)
		{
			return ZERO(x) && ZERO(y);
		}
	}
	return true;
}

void findhexagons(long maxperim, long totperim, vector<long> sides, set<hexagon>& found)
{
	long sidestart = sides.size() > 0 && sides.size() <= 2 ? *sides.begin() : 1;
	for (long side = sidestart; side <= maxperim - totperim; side++)
	{
		sides.push_back(side);
		if (checksides(sides, maxperim))
		{
			if (sides.size() == 6)
			{
				found.insert(hexagon(sides, totperim + side));
			}
			else
			{
				findhexagons(maxperim, totperim + side, sides, found);
			}
		}
		sides.pop_back();
	}
}

int main(int argc, char** argv)
{
	set<hexagon> hexagons;

	long maxperim = stoi(argv[1]);
	findhexagons(maxperim, 0, vector<long>(), hexagons);
	//for (auto h : hexagons)
	//{
	//	for (auto side : h.sides)
	//	{
	//		cout << side;
	//	}
	//	cout << endl;
	//}
	cout << "H(" << maxperim << ") = " << hexagons.size() << endl;

    return 0;
}

