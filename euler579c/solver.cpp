#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"
#include "util.h"
#include "overlong.h"

unsigned long addgcd(unsigned long current, const vector3d& v)
{
	return current + v.gcd();
}

void solver::process_mnpq(mnpq& item)
{
	vector<unsigned long> perm = item.as_vector();
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
				unsigned long gcd = util::gcd(set<long>({u->gcd(), v->gcd(), n.gcd()}));
				if (gcd == 1)
				{
					cube c(*v, *u, n);
					item.add_cube(c);
				}
			}
		}
	}
	

	
	unsigned long combinations = item.cubes.size();
	overlong thisCxr = 0;
	overlong thisS = 0;
	if (combinations > 0) //might be zero for primitive ones e.g. from 0,0,0,3.
	{
		set<cube>::const_iterator cube = item.cubes.begin();
		overlong width = cube->width,
			height = cube->height,
			depth = cube->depth;
		overlong tmax = maxSide / (max(width, max(height, depth)));
		overlong sumgcd = accumulate(cube->uvn.begin(), cube->uvn.end(), 0, addgcd);

		for (overlong t = 1; t <= tmax; t++)
		{
			overlong repeatability =
				overlong(maxSide + 1 - t * width) *
				overlong(maxSide + 1 - t * height) *
				overlong(maxSide + 1 - t * depth);
			thisCxr += repeatability * combinations;

			overlong l = cube->uvn.begin()->length;
			overlong ehp = (l*l*l)*(t*t*t) + l*(sumgcd) * (t*t) + (sumgcd)* t + 1; //from arXiv:1508.03643v2 [math.NT] 17 Mar 2016, theorem 2.14
			overlong contributionS = ehp * repeatability * combinations;

			thisS += contributionS;

		}
	}
	C += thisCxr;
	S += thisS;
	if (M > 0)
	{
		S %= M;
		C %= M;
	}

}

void solver::solve()
{
	for (unsigned long m = 0; m <= sqrt(maxSide.val); m++)
	{
		unsigned long nmax = sqrt(maxSide.val - m*m);
		for (unsigned long n = m; n <= nmax; n++)
		{
			unsigned long pmax = sqrt(maxSide.val - m*m - n*n);
			for (unsigned long p = n; p <= pmax; p++)
			{
				unsigned long qmax = sqrt(maxSide.val - m*m - n*n - p*p);
				for (unsigned long q = p; q <= qmax; q++)
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