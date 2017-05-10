// euler600.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hexagon.h"
#include "macros.h"
#include "util.h"

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

void process(const vector<set<sidepair>::const_iterator>& vs)
{
	for (auto v0 : vs)
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
}

class xy
{
public:
	double x, y;
	xy(double _x, double _y) : x(_x), y(_y) {}
	bool operator<(const xy& rhs) const
	{
		if (!ZERO(x - rhs.x)) return x < rhs.x;
		if (!ZERO(y - rhs.y)) return y < rhs.y;
		return false;
	}
	bool operator==(const xy& rhs) const
	{
		return ZERO(x - rhs.x) && ZERO(y - rhs.y);
	}
	double dist() const
	{
		return sqrt(x*x + y*y);
	}
};

xy operator*(const xy& lhs, long rhs)
{
	return xy(lhs.x * rhs, lhs.y * rhs);
}

class trio
{
public:
	long s0, s1, s2;
	xy pos;
	trio(long _s0, long _s1, long _s2, const xy& _pos) : s0(_s0), s1(_s1), s2(_s2), pos(_pos) {}
	trio(long _s0, long _s1, long _s2) : trio(_s0, _s1, _s2, xy(
		_s0 + _s1 * cos(ANGLE_INCREMENT) + _s2 * cos(2 * ANGLE_INCREMENT),
			  _s1 * sin(ANGLE_INCREMENT) + _s2 * sin(2 * ANGLE_INCREMENT))) {}
	bool operator<(const trio& rhs) const
	{
		if (s0 != rhs.s0) return s0 < rhs.s0;
		if (s1 != rhs.s1) return s1 < rhs.s1;
		if (s2 != rhs.s2) return s2 < rhs.s2;
		return false;
	}

	long total_perim() const
	{
		return s0 + s1 + s2;
	}
};

trio operator*(const trio& lhs, long rhs)
{
	return trio(lhs.s0 * rhs, lhs.s1 * rhs, lhs.s2 * rhs, xy(lhs.pos * rhs));
}

set<trio> trios;
mutex m_trios;
long CHUNK_SIZE = 16000;
atomic<long> current_s0(1);
void do_processing()
{
	bool isended = false;
	long maxside = (maxperim - 4) / 2;
	for (long s0 = current_s0++; s0 <= maxside; s0 = current_s0++)
	{
		set<trio> current_trios;
		for (long s1 = 1; s1 <= maxside; s1++)
		{
			for (long s2 = 1; s2 <= maxside; s2++)
			{
				if (util<long>::gcd(s0, s1, s2) == 1)
				{
					trio t(s0, s1, s2);
					long perimleft = maxperim - s0 - s1 - s2;
					long repetitions = maxperim / t.total_perim();
					for (long f = 1; f <= repetitions; f++)
					{
						current_trios.insert(t * f);
					}
				}
			}
		}
		{
			lock_guard<mutex> lm(m_trios);
			trios.insert(current_trios.begin(), current_trios.end());
		}
	}
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

	vector<thread> threads;
	for (long i = 0; i < numthreads; i++)
	{
		threads.push_back(thread(do_processing));
	}
	for (vector<thread>::iterator t = threads.begin(); t != threads.end(); t++)
	{
		t->join();
	}

	cout << "There are " << trios.size() << " trios." << endl;
	multimap<xy, trio> triomap;

	for (auto t : trios)
	{
		triomap.insert(pair<xy, trio>(t.pos, t));
	}

	cout << "Made trio map" << endl;
	
	for (auto t : trios)
	{
		for (multimap<xy, trio>::const_iterator match = triomap.lower_bound(t.pos); 
			match != triomap.end() && match->first == t.pos; match++)
		{
			hexagon h(vector<long>({ t.s0, t.s1, t.s2, match->second.s0, match->second.s1, match->second.s2 }));
			if (h.sides == h.sides_orig)
			{
				long totperim = t.total_perim() + match->second.total_perim();
				if (totperim <= maxperim)
				{
#if _DEBUG
					cout << h << endl;
#endif
					hexcount++;
				}
			}
		}
	}

	cout << "There are " << hexcount << " hexagons." << endl;

	{
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);
		std::cout << "end: " << std::put_time(std::localtime(&now_c), "%c") << endl;
	}

	return 0;
}


