#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"

void solver::process_mnpq(const mnpq& item)
{
	abcd sln = item.get_abcd();
	vector3d rootVector = sln.get_root_vector();
	vector<long> perm = item.as_vector();
	sort(perm.begin(), perm.end());
	while (next_permutation(perm.begin(), perm.end()))
	{
		mnpq alt(*perm.begin(), *perm.begin() + 1, *perm.begin() + 2, *perm.begin() + 3);
		set<vector3d> vectors = alt.get_abcd().get_vectors();
		for (set<vector3d>::const_iterator v = vectors.begin(); v != vectors.end(); v++)
		{

		}

	};

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