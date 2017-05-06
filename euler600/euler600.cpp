// euler600.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hexagon.h"

using namespace std;

#define EPSILON 1e-12
#define ZERO(dbl) (fabs(dbl) <= EPSILON)
#define LT_ZERO(dbl) (dbl < -EPSILON)
#define GT_ZERO(dbl) (dbl > EPSILON)

typedef pair<bool, vector<long>> checksidesresult;

checksidesresult checksides(const vector<long>& sides, long maxperim)
{
	double x = 0, y = 0;
	double angle = 0;
	long sidescounted = 0;
	long perimeterused = 0;
	for (auto side : sides)
	{
		x += side * cos(angle);
		y += side * sin(angle);
		perimeterused += side;
		++sidescounted;

		if (LT_ZERO(y))
		{
			//gone below
			return checksidesresult(false, vector<long>());
		}
		
		if (ZERO(x) && ZERO(y) && sidescounted < 6)
		{
			//back at origin but not used all sides
			return checksidesresult(false, vector<long>());
		}

		double dist = sqrt(x*x + y*y);
		long maxtogetback = maxperim - perimeterused;
		//we can't possibly get back in time:
		if (GT_ZERO(dist - maxtogetback)) return checksidesresult(false, vector<long>());
		

		angle += (M_PI / 3); //60 degrees
		
		if (sidescounted == 6)
		{
			return checksidesresult(ZERO(x) && ZERO(y), vector<long>());
		}
	}
	return checksidesresult(true, vector<long>());
}

void findhexagons(long maxperim, long totperim, vector<long> sides, set<hexagon>& found)
{
	long sidestart = 1;
	for (long side = sidestart; side <= maxperim - totperim; side++)
	{
		sides.push_back(side);
		long count = sides.size();
		auto checksidesresult = checksides(sides, maxperim);
		long addedcount = checksidesresult.second.size();
		if (addedcount > 0)
		{
			sides.insert(sides.end(), checksidesresult.second.begin(), checksidesresult.second.end());
		}
		if (checksidesresult.first)
		{
			if (sides.size() == 6)
			{
				hexagon h(sides, totperim + side);
				auto insertresult = found.insert(h);
				//if (!insertresult.second)
				//{
				//	cout << h << "is a duplicate of " << *insertresult.first << endl;
				//}
			}
			else
			{
				findhexagons(maxperim, totperim + side, sides, found);
			}
		}
		if (addedcount > 0)
		{
			sides.resize(sides.size() - addedcount);
		}
		sides.pop_back();
	}
}

int main(int argc, char** argv)
{
	const clock_t begin = clock();

	set<hexagon> hexagons;
	long maxperim = stoi(argv[1]);
	findhexagons(maxperim, 0, vector<long>(), hexagons);
	//cout << endl << "found: << " << endl;
	//for (auto h : hexagons)
	//{
	//	for (auto side : h.sides)
	//	{
	//		cout << side;
	//	}
	//	cout << endl;
	//}

	clock_t end = clock();
	float duration = ((float)(end - begin)) / CLOCKS_PER_SEC;

	cout << "H(" << maxperim << ") = " << hexagons.size() << endl;
	cout << "Took " << fixed << setprecision(2) << duration << " seconds " << endl;
    return 0;
}

