#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"
#include "util.h"
#include "massiveinteger.h"

long long itcount = 0;

long long addgcd(long long current, const vector3d& v)
{
	return current + v.gcd();
}

void solver::process_mnpq(mnpq& item)
{
	vector<long long> perm = item.as_vector();
	sort(perm.begin(), perm.end());
	set<vector3d> allVectors;
	do {
		mnpq alt(*(perm.begin()), *(perm.begin() + 1), *(perm.begin() + 2), *(perm.begin() + 3));
		set<vector3d> abcdvectors = alt.get_abcd().get_vectors();
		allVectors.insert(abcdvectors.begin(), abcdvectors.end());
	} while (next_permutation(perm.begin(), perm.end()));

	for (set<vector3d>::const_iterator u = allVectors.begin(); u != allVectors.end(); u++)
	{
		for (set<vector3d>::const_iterator v = allVectors.begin(); v != allVectors.end(); v++)
		{
			if (u != v && v->is_orthogonal_to(*u))
			{
				vector3d n = u->cross_product(*v);
				long long gcd = util::gcd(set<long long>({u->gcd(), v->gcd(), n.gcd()}));
				if (gcd == 1)
				{
					cube c(*v, *u, n);
					item.add_cube(c);
				}
			}
		}
	}
	
	long long combinations = item.cubes.size();
	massiveinteger thisCxr = 0;
	massiveinteger thisS = 0;
	if (combinations > 0) //might be zero for primitive ones e.g. from 0,0,0,3.
	{
		set<cube>::const_iterator cube = item.cubes.begin();
		long long width = cube->width,
			height = cube->height,
			depth = cube->depth;
		long long tmax = maxSide / (max(width, max(height, depth)));
		long long sumgcd = accumulate(cube->uvn.begin(), cube->uvn.end(), 0, addgcd);

		for (long long t = 1; t <= tmax; t++)
		{
			long long repeatability =
				(maxSide + 1 - t * width) *
				(maxSide + 1 - t * height) *
				(maxSide + 1 - t * depth);

			thisCxr += massiveinteger(repeatability) * massiveinteger(combinations);

			long long l = cube->uvn.begin()->length;
			massiveinteger ehp = massiveinteger(l*l*l) * massiveinteger(t*t*t) 
				+ massiveinteger(l*(sumgcd)) * massiveinteger(t*t) + massiveinteger((sumgcd)* t + 1);
				//from arXiv:1508.03643v2 [math.NT] 17 Mar 2016, theorem 2.14
			massiveinteger contributionS = ehp * massiveinteger(repeatability) * massiveinteger(combinations);

			thisS += contributionS;
		}
	}


	C += thisCxr;
	S += thisS;

	if (maxResultDigits > 0)
	{
		C.truncate(maxResultDigits);
		S.truncate(maxResultDigits);
	}
}

void solver::solve()
{
	for (long long m = 0; m <= sqrt(maxSide); m++)
	{
		long long nmax = sqrt(maxSide - m*m);
		for (long long n = m; n <= nmax; n++)
		{
			long long pmax = sqrt(maxSide - m*m - n*n);
			for (long long p = n; p <= pmax; p++)
			{
				long long qmax = sqrt(maxSide - m*m - n*n - p*p);
				for (long long q = p; q <= qmax; q++)
				{
					if (((m + n + p + q) % 2) == 1)
					{
						mnpq it(m, n, p, q);
						process_mnpq(it);
					}
				}
			}
		}
	}
}
