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

template<typename TInt>
static long gcd_l(TInt a, TInt b)
{
	a = abs(a);
	b = abs(b);
	for (;;)
	{
		if (a == 0)
			return b;
		b %= a;
		if (b == 0)
			return a;
		a %= b;
	}
	throw runtime_error("gcd not found");
}

template<typename TIterator, typename TInt>
static TInt gcd(TIterator first, TIterator last)
{
	TInt l1 = *first;
	if (++first == last) return l1;
	else return gcd_l(abs(l1), abs(gcd<TIterator, TInt>(first, last)));
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
			return ZERO(x) && ZERO(y);// &&
				//gcd<vector<long>::const_iterator, long>(sides.begin(), sides.end()) == 1;
		}
	}
	return true;
}

mutex m_currentfirstside, m_numfound;
long currentfirstside = 1, numfound, maxperim;

void findhexagons(long totperim, vector<long> sides)
{
	long sidemax = sides.front();
	for (long side = 1; side <= maxperim - totperim && side <= sidemax; side++)
	{
		sides.push_back(side);
		long count = sides.size();
		auto checksidesresult = checksides(sides, maxperim);

		if (checksidesresult)
		{
			if (sides.size() == 6)
			{
				long finalperim = totperim + side;
				hexagon h(sides, finalperim);
				if (h.sides == h.sides_orig)
				{
					lock_guard<mutex> lm(m_numfound);
					numfound++;
				}
			}
			else
			{
				findhexagons(totperim + side, sides);
			}
		}

		sides.pop_back();
	}
}


void start_findhexagons()
{
	long firstside;
	do
	{
		{
			lock_guard<mutex> lm(m_currentfirstside);
			firstside = currentfirstside++;
			cout << firstside << endl;
		}

		vector<long> sides({ firstside });
		findhexagons(0, sides);

	} while (firstside <= (maxperim - 4) / 2);
}


int main(int argc, char** argv)
{
	const clock_t begin = clock();

	set<hexagon> hexagons;
	maxperim = stoi(argv[1]);
	numfound = 0;

	vector<thread> vthreads;
	for (long t = 0; t < 8; t++)
	{
		vthreads.push_back(thread(start_findhexagons));
	}

	for (vector<thread>::iterator t = vthreads.begin(); t != vthreads.end(); t++)
	{
		t->join();
	}

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

