// euler600.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hexagon.h"

using namespace std;

#define EPSILON 1e-12
#define ZERO(dbl) (fabs(dbl) <= EPSILON)
#define LT_ZERO(dbl) (dbl < -EPSILON)
#define GT_ZERO(dbl) (dbl > EPSILON)

template<typename T>
vector<vector<T>> split(const vector<T>& _v, long numvectors)
{
	vector<T> v(_v.begin(), _v.end());
	vector<vector<T>> result;
	long countineach = v.size() / numvectors;
	for (long n = 0; n < numvectors; n++)
	{
		vector<T> v1(v.begin(), v.begin() + countineach);
		v.erase(v.begin(), v.begin() + countineach);
		result.push_back(v1);
	}
	return result;
}

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
		perimeterused += side;
		++sidescounted;

		if (LT_ZERO(y))
		{
			//gone below
			return false;
		}

		if (ZERO(x) && ZERO(y) && sidescounted < 6)
		{
			//back at origin but not used all sides
			return false;
		}

		double dist = sqrt(x*x + y*y);
		long maxtogetback = maxperim - perimeterused;
		//we can't possibly get back in time:
		if (GT_ZERO(dist - maxtogetback)) return false;


		angle += (M_PI / 3); //60 degrees

		if (sidescounted == 6)
		{
			if (ZERO(x) && ZERO(y))
			{
				bool sorted = true;
				//all_of(split3.begin(), split3.end(), [](vector<long> v) { return is_sorted(v.rbegin(), v.rend()); }) ||
				//all_of(split2.begin(), split2.end(), [](vector<long> v) { return is_sorted(v.rbegin(), v.rend()); });
				return sorted;
			}
			else return false;
		}
	}
	return true;
}

void findhexagons(long maxperim, long totperim, vector<long> sides, set<hexagon>& found, long& numfound)
{
	long sidemax = sides.size() > 0 ? sides.front() : LONG_MAX;
	for (long side = 1; side <= maxperim - totperim && side <= sidemax; side++)
	{
		sides.push_back(side);
		long count = sides.size();
		auto checksidesresult = checksides(sides, maxperim);

		if (checksidesresult)
		{
			if (sides.size() == 6)
			{

				hexagon h(sides, totperim + side);
				if (h.sides == h.sides_orig) numfound++;
				//auto insertresult = found.insert(h);
				//if (insertresult.second) numfound++;
				//if (!insertresult.second)
				//{
				//	cout << h << "is a duplicate of " << *insertresult.first << endl;
				//}
			}
			else
			{
				findhexagons(maxperim, totperim + side, sides, found, numfound);
			}
		}

		sides.pop_back();
	}
}

int main(int argc, char** argv)
{
	const clock_t begin = clock();

	set<hexagon> hexagons;
	long maxperim = stoi(argv[1]);
	long numfound = 0;
	findhexagons(maxperim, 0, vector<long>(), hexagons, numfound);
	cout << endl << "found: " << endl;
	for (auto h : hexagons)
	{
		cout << h << endl;
	}

	clock_t end = clock();
	float duration = ((float)(end - begin)) / CLOCKS_PER_SEC;

	cout << "H(" << maxperim << ") = " << numfound << endl;
	cout << "Took " << fixed << setprecision(2) << duration << " seconds " << endl;
	return 0;
}

