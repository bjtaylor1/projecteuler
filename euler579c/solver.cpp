#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"
#include "util.h"
#include "macros.h"

long long itcount = 0;

long long addgcd(long long current, const vector3d& v)
{
	return current + v.gcd();
}

void solver::process_mnpq(mnpq& item)
{
	if((itcount++ % 1000) == 0) cout << item.get_abcd() << endl;

	vector<long long> perm = item.as_vector();
	sort(perm.begin(), perm.end());
	set<vector3d> allVectors;
	do {
		mnpq alt(*(perm.begin()), *(perm.begin() + 1), *(perm.begin() + 2), *(perm.begin() + 3));
		set<vector3d> abcdvectors = alt.get_abcd().get_vectors();
		allVectors.insert(abcdvectors.begin(), abcdvectors.end());
	} while (next_permutation(perm.begin(), perm.end()));

	bool someOversize = false, someNotOversize = false;

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
					if (c.oversize) someOversize = true; else someNotOversize = true;
					item.add_cube(c);
				}
			}
		}
	}

	

	BIGINT thisCxr = 0;
	BIGINT thisS = 0;
	for(set<cube>::const_iterator cube = item.cubes.begin(); cube != item.cubes.end(); cube++)
	{
		if (!cube->oversize)
		{
			long long width = cube->width,
				height = cube->height,
				depth = cube->depth;
			long long tmax = maxSide / (max(width, max(height, depth)));
			long long sumgcd = accumulate(cube->uvn.begin(), cube->uvn.end(), 0, addgcd);

			for (long long t = 1; t <= tmax; t++)
			{
				long long repeatability =
					(maxSide + 1LL - t * width) *
					(maxSide + 1LL - t * height) *
					(maxSide + 1LL - t * depth);

				thisCxr = thisCxr + BIGINT(repeatability);

				long long l = cube->uvn.begin()->length;
				BIGINT ehp = BIGINT(l*l*l) * BIGINT(t*t*t)
					+ BIGINT(l*(sumgcd)) * BIGINT(t*t) + BIGINT((sumgcd)* t + 1);
				//from arXiv:1508.03643v2 [math.NT] 17 Mar 2016, theorem 2.14
				BIGINT contributionS = ehp * BIGINT(repeatability);

				thisS = thisS + contributionS;
			}
		}
	}


	C  = C + thisCxr;
	S = S + thisS;

	if (maxResultDigits > 0 && ((itcount % 10) == 0))
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
						try
						{
							process_mnpq(it);
						}
						catch (exception e)
						{
							stringstream ss;
							ss << e.what() << " while processing " << it;
							throw runtime_error(ss.str().c_str());
						}
					}
				}
			}
		}
	}
}
