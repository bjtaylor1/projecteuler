#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"
#include "util.h"

bigulong addgcd(bigulong current, const vector3d& v)
{
	return current + v.gcd();
}

void solver::process_mnpq(mnpq& item)
{
	vector<biglong> perm = item.as_vector();
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
				biglong gcd = util::gcd(set<biglong>({u->gcd(), v->gcd(), n.gcd()}));
				if (gcd == 1)
				{
					cube c(*v, *u, n);
					item.add_cube(c);
				}
			}
		}
	}
	

	
	bigulong combinations = item.cubes.size();
	bigulong thisCxr = 0;
	bigulong thisS = 0;
	if (combinations > 0) //might be zero for primitive ones e.g. from 0,0,0,3.
	{
		set<cube>::const_iterator cube = item.cubes.begin();
		bigulong width = cube->width,
			height = cube->height,
			depth = cube->depth;
		bigulong tmax = maxSide / (max(width, max(height, depth)));
		bigulong sumgcd = accumulate(cube->uvn.begin(), cube->uvn.end(), 0, addgcd);

		for (bigulong t = 1; t <= tmax; t++)
		{
			bigulong repeatability =
				(maxSide + 1 - t * width) *
				(maxSide + 1 - t * height) *
				(maxSide + 1 - t * depth);
			thisCxr += (repeatability * combinations);
			if (M > 0) thisCxr %= M;

			bigulong l = cube->uvn.begin()->length;
			bigulong ehp = (l*l*l)*(t*t*t) + l*(sumgcd) * (t*t) + (sumgcd)* t + 1; //from arXiv:1508.03643v2 [math.NT] 17 Mar 2016, theorem 2.14
			bigulong contributionS = ehp * repeatability * combinations;
			if (M > 0) contributionS %= M;
			thisS += contributionS;
			if (M > 0) thisS %= M;
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
	for (biglong m = 0; m <= sqrt(maxSide); m++)
	{
		biglong nmax = sqrt(maxSide - m*m);
		for (biglong n = m; n <= nmax; n++)
		{
			biglong pmax = sqrt(maxSide - m*m - n*n);
			for (biglong p = n; p <= pmax; p++)
			{
				biglong qmax = sqrt(maxSide - m*m - n*n - p*p);
				for (biglong q = p; q <= qmax; q++)
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