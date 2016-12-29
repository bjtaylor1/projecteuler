// euler579.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

class square
{
public:
	long x, y, z, size;
	mutable long count;
	square(long _x, long _y, long _z, long _size) : count(1), size(_size)
	{
		vector<long> vertices{ _x, _y, _z };
		sort(vertices.begin(), vertices.end());
		x = vertices[0];
		y = vertices[1];
		z = vertices[2];
	}
};

bool operator<(const square& s1, const square& s2)
{
	if (s1.x != s2.x) return s1.x < s2.x;
	if (s1.y != s2.y) return s1.y < s2.y;
	if (s1.z != s2.z) return s1.z < s2.z;
	if (s1.size != s2.size) return s1.size < s2.size;
	return false;
}

long add_count(long tot, const square& s)
{
	return tot + s.count;
}

void C(long n, long& c, long& s)
{
	set<square> sqs;
	for (long x = 0; x < n; x++)
	{
		for (long y = 0; y < n; y++)
		{
			for (long z = 0; z < n; z++)
			{
				for (long size = 1; size <= n - max(max(x,y),z); size++)
				{
					square sq(x,y,z, size);
					set<square>::iterator it = sqs.find(sq);
					if (it != sqs.end())
					{
						it->count++;
					}
					else sqs.insert(sq);
				}
			}
		}
	}
	c = accumulate(sqs.begin(), sqs.end(), 0, &add_count);
}

void test(long n)
{
	long c = 0, s = 0;
	C(n, c, s);
	cout << "C(" << n << ") = " << c << ", S(" << n << ") = " << s << endl;
}

int main()
{
	test(1);
	test(2);
	test(3);
	test(4);

    return 0;
}

