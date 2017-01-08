#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"
#include "util.h"

void solver::process_mnpq(mnpq& item)
{
	vector<long> perm = item.as_vector();
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
				long gcd = util::gcd(set<long>({u->gcd(), v->gcd(), n.gcd()}));
				if (gcd == 1)
				{
					cube c(*v, *u, n);
					item.add_cube(c);
				}
			}
		}
	}
	
	//for (set<cube>::const_iterator it = item.cubes.begin(); it != item.cubes.end(); it++)
	//{
	//	cout << *it << endl;
	//}

	
	unsigned long long combinations = item.cubes.size();
	if (combinations > 0) //might be zero for primitive ones e.g. from 0,0,0,3.
	{
		unsigned long long width = item.cubes.begin()->width,
			height = item.cubes.begin()->height,
			depth = item.cubes.begin()->depth;
		unsigned long long tmax = maxSide / (max(width, max(height, depth)));
		for (unsigned long long t = 1; t <= tmax; t++)
		{
			unsigned long long repeatability =
				(maxSide + 1 - t * width) *
				(maxSide + 1 - t * height) *
				(maxSide + 1 - t * depth);
			C += (repeatability * combinations);
		}
	}

}

void solver::solve()
{
	for (long m = 0; m <= sqrt(maxSide); m++)
	{
		long nmax = sqrt(maxSide - m*m);
		for (long n = m; n <= nmax; n++)
		{
			long pmax = sqrt(maxSide - m*m - n*n);
			for (long p = n; p <= pmax; p++)
			{
				long qStart = max(1L, p + ((m + n + p) % 2)); //the total must be odd (otherwise it won't be primitive) - so q might as well go up in twos
				long qmax = sqrt(maxSide - m*m - n*n - p*p);
				for (long q = qStart; q <= qmax; q += 2)
				{
					mnpq it(m, n, p, q);
					process_mnpq(it);
				}
			}
		}
	}
}