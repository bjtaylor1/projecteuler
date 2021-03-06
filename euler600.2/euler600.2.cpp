// euler600.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hexagon.h"
#include "macros.h"
#include <stxxl/vector>
#include <stxxl/map>

using namespace std;

long maxperim;
mutex m_multivectors;
double ANGLE_INCREMENT = M_PI / 3;

class sidepair
{
public:
	long s0, s1;
	double x, y;
	sidepair() {}
	sidepair(long _s0, long _s1, double _x, double _y) : s0(_s0), s1(_s1), x(_x), y(_y) {}

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

class sidepair_comparer
{
public:
	bool operator()(const sidepair& lhs, const sidepair& rhs) const
	{
		return lhs < rhs;
	}

	static sidepair max_value()
	{
		sidepair spmax(LONG_MAX, LONG_MAX, DBL_MAX, DBL_MAX);
		return spmax;
	}
};

ostream& operator <<(ostream& os, const sidepair& sp)
{
	os << sp.s0 << "," << sp.s1;
	return os;
}

class dummy
{
public:
};

ostream& operator<<(ostream& os, const dummy& d)
{
	return os;
}
#define DATA_NODE_BLOCK_SIZE (4096)
#define DATA_LEAF_BLOCK_SIZE (4096)

typedef stxxl::map<sidepair, dummy, sidepair_comparer, DATA_NODE_BLOCK_SIZE, DATA_LEAF_BLOCK_SIZE> sidepairmap;

mutex m_currentsidepair;
long it_s0;

void process(sidepairmap& sidepairs, atomic<unsigned long long>& hexcount, const vector<sidepairmap::const_iterator>& vs)
{
	for (auto v0 : vs)
	{
		if (v0->first.s0 >= v0->first.s1)
		{
			long found = 0, duplicates = 0;
			for (auto v1 = sidepairs.begin(); v1 != sidepairs.end(); v1++)
			{
				double totx =
					v0->first.s0 * cos(ANGLE_INCREMENT * 0) +
					v0->first.s1 * cos(ANGLE_INCREMENT * 1) +
					v1->first.s0 * cos(ANGLE_INCREMENT * 2) +
					v1->first.s1 * cos(ANGLE_INCREMENT * 3);
				double toty =
					v0->first.s0 * sin(ANGLE_INCREMENT * 0) +
					v0->first.s1 * sin(ANGLE_INCREMENT * 1) +
					v1->first.s0 * sin(ANGLE_INCREMENT * 2) +
					v1->first.s1 * sin(ANGLE_INCREMENT * 3);
				double distsq = totx*totx + toty*toty;
				double perimleft = (maxperim - v0->first.s0 - v0->first.s1 - v1->first.s0 - v1->first.s1);
				double perimleftsq = perimleft * perimleft;
				if (distsq <= perimleftsq)
				{
					sidepair v2finder(-totx, -toty);

					for (auto v2 = sidepairs.lower_bound(v2finder); v2 != sidepairs.end() && v2->first == v2finder; v2++)
					{
						vector<long> sides({ v0->first.s0, v0->first.s1, v1->first.s0, v1->first.s1, v2->first.s0, v2->first.s1 });
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
}

long CHUNK_SIZE = 16000;
void do_processing(sidepairmap& sidepairs, atomic<unsigned long long>& hexcount, sidepairmap::const_iterator& currentsidepair)
{
	bool isended = false;
	do {
		vector<sidepairmap::const_iterator> vs;
		{
			lock_guard<mutex> lm(m_currentsidepair);
			for (long i = 0; i++ < CHUNK_SIZE; currentsidepair++)
			{
				if (currentsidepair == sidepairs.end())
				{
					isended = true;
					break;
				}
				else vs.push_back(currentsidepair);
			}
		}
		process(sidepairs, hexcount, vs);
	} while (!isended);
}

int main(int argc, char** argv)
{
	try
	{
		{
			auto now = std::chrono::system_clock::now();
			auto now_c = std::chrono::system_clock::to_time_t(now);
			std::cout << "start: " << std::put_time(std::localtime(&now_c), "%c") << endl;
		}

		maxperim = stoi(argv[1]);

		atomic<unsigned long long> hexcount(0);
#if _DEBUG
		set<hexagon> hexagons;
#endif
		sidepairmap sidepairs(sidepair_comparer(), sidepairmap::node_block_type::raw_size * 3, sidepairmap::node_block_type::raw_size * 3);
		sidepairmap::const_iterator currentsidepair;

		for (it_s0 = 1; it_s0 <= (maxperim - 4) / 2; it_s0++)
		{
			long maxside = (maxperim / 2) - it_s0 - 1;
			for (long s1 = 1; s1 <= maxside; s1++)
			{
				sidepair sp(it_s0, s1);
				pair<sidepair, dummy> p(sp, dummy());
				sidepairs.insert(p);
			}
		}

		cout << "Preprocessed " << sidepairs.size() << " side pairs." << endl;
		currentsidepair = sidepairs.begin();

		do_processing(sidepairs, hexcount, currentsidepair);

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
	catch (std::exception e)
	{
		cout << e.what() << endl;
		return 1;
	}
}


