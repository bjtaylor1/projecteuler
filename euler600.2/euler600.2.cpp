// euler600.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hexagon.h"
#include "macros.h"

using namespace std;

long maxperim;
mutex m_multivectors;
double ANGLE_INCREMENT = M_PI / 3;

long numthreads = 0;

class sidepair
{
public:
	long s0, s1;
	double x, y;
	sidepair(long _s0, long _s1) : s0(_s0), s1(_s1),
		x(_s0 * cos(ANGLE_INCREMENT * 4) + s1 * cos(ANGLE_INCREMENT * 5)),
		y(_s0 * sin(ANGLE_INCREMENT * 4) + s1 * sin(ANGLE_INCREMENT * 5)) {}
	sidepair(double _x, double _y) : x(_x), y(_y) {}

	bool operator<(const sidepair& rhs) const
	{
		if (!ZERO(x - rhs.x)) return x < rhs.x;
		if (!ZERO(y - rhs.y)) return y < rhs.y;
		return false;
	}

	bool operator==(const sidepair&  rhs) const
	{
		return ZERO(x - rhs.x) && ZERO(y - rhs.y);
	}
};

ostream& operator <<(ostream& os, const sidepair& sp)
{
	os << sp.s0 << "," << sp.s1;
	return os;
}

atomic<unsigned long long> hexcount(0);
#if _DEBUG
set<hexagon> hexagons;
#endif
set<sidepair> sidepairs;
set<sidepair>::const_iterator currentsidepair;
mutex m_currentsidepair;

void process(const set<sidepair>::const_iterator v0)
{
	if (v0->s0 >= v0->s1)
	{
		long found = 0, duplicates = 0;
		for (auto v1 = sidepairs.begin(); v1 != sidepairs.end(); v1++)
		{
			double totx =
				v0->s0 * cos(ANGLE_INCREMENT * 0) +
				v0->s1 * cos(ANGLE_INCREMENT * 1) +
				v1->s0 * cos(ANGLE_INCREMENT * 2) +
				v1->s1 * cos(ANGLE_INCREMENT * 3);
			double toty =
				v0->s0 * sin(ANGLE_INCREMENT * 0) +
				v0->s1 * sin(ANGLE_INCREMENT * 1) +
				v1->s0 * sin(ANGLE_INCREMENT * 2) +
				v1->s1 * sin(ANGLE_INCREMENT * 3);
			double distsq = totx*totx + toty*toty;
			double perimleft = (maxperim - v0->s0 - v0->s1 - v1->s0 - v1->s1);
			double perimleftsq = perimleft * perimleft;
			if (distsq <= perimleftsq)
			{
				sidepair v2finder(-totx, -toty);

				for (auto v2 = sidepairs.lower_bound(v2finder); v2 != sidepairs.end() && *v2 == v2finder; v2++)
				{
					vector<long> sides({ v0->s0, v0->s1, v1->s0, v1->s1, v2->s0, v2->s1 });
					if (accumulate(sides.begin(), sides.end(), 0) <= maxperim)
					{
						hexagon h(sides);
#if _DEBUG
						hexagons.insert(h);
#else
						if (h.sides == h.sides_orig) hexcount++;
						if (hexcount >= ULONG_MAX) throw runtime_error("Overflow");
#endif
					}
				}
			}

		}
	}
}

void do_processing()
{
	set<sidepair>::const_iterator sp;
	do {
		{
			lock_guard<mutex> lm(m_currentsidepair);
			if (currentsidepair == sidepairs.end()) return;
			sp = currentsidepair++;
		}
		process(sp);
	} while (sp != sidepairs.end());
}

int main(int argc, char** argv)
{
	{
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);
		std::cout << "start: " << std::put_time(std::localtime(&now_c), "%c") << endl;
	}

	maxperim = stoi(argv[1]);
	numthreads = stoi(argv[2]);

	for (long s0 = 1; s0 <= (maxperim - 4) / 2; s0++)
	{
		long maxside = (maxperim / 2) - s0 - 1;
		for (long s1 = 1; s1 <= maxside; s1++)
		{
			sidepair sp(s0, s1);
			sidepairs.insert(sp);
		}
	}

	cout << "Preprocessed " << sidepairs.size() << " side pairs." << endl;
	currentsidepair = sidepairs.begin();

	vector<thread> threads;
	for (long i = 0; i < numthreads; i++)
	{
		threads.push_back(thread(do_processing));
	}
	for (vector<thread>::iterator t = threads.begin(); t != threads.end(); t++)
	{
		t->join();
	}


#if _DEBUG
	for (auto h : hexagons)
	{
		cout << h << endl;
	}
	cout << "There are " << hexagons.size() << " hexagons." << endl;
#else
	cout << "There are " << hexcount << " hexagons." << endl;
#endif

	{
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);
		std::cout << "end: " << std::put_time(std::localtime(&now_c), "%c") << endl;
	}

	return 0;
}


