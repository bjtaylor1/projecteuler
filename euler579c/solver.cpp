#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"

void solver::process_mnpq(mnpq& item)
{
	vector<long> perm = item.as_vector();
	sort(perm.begin(), perm.end());
	set<vector3d> allVectors;
	while (next_permutation(perm.begin(), perm.end()))
	{
		mnpq alt(*(perm.begin()), *(perm.begin() + 1), *(perm.begin() + 2), *(perm.begin() + 3));
		set<vector3d> abcdvectors = alt.get_abcd().get_vectors();
		allVectors.insert(abcdvectors.begin(), abcdvectors.end());
	};

	for (set<vector3d>::const_iterator u = allVectors.begin(); u != allVectors.end(); u++)
	{
		for (set<vector3d>::const_iterator v = allVectors.begin(); v != allVectors.end(); v++)
		{
			if (u != v && v->is_orthogonal_to(*u))
			{
				vector3d n = u->cross_product(*v);
				cube c(*v, *u, n);
				item.add_cube(c);
			}
		}
	}
	
	for (set<cube>::const_iterator it = item.cubes.begin(); it != item.cubes.end(); it++)
	{
		cout << *it << endl;
	}
}

void solver::solve()
{
	for (long m = 0; m <= sqrt(maxSide); m++)
	{
		for (long n = m; n <= sqrt(maxSide - m*m); n++)
		{
			for (long p = n; p <= sqrt(maxSide - m*m - n*n); p++)
			{
				long qStart = p + ((m + n + p) % 2) + 1; //the total must be odd (otherwise it won't be primitive) - so q might as well go up in twos
				for (long q = qStart; q <= sqrt(maxSide - m*m - n*n - p*p); q += 2)
				{
					mnpq it(m, n, p, q);
					process_mnpq(it);
				}
			}
		}
	}
}