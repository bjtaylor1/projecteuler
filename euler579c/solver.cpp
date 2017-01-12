#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"
#include "util.h"
#include "macros.h"
#include "vectortriple.h"

long long itcount = 0;

long long addgcd(long long current, const vector3d& v)
{
	return current + v.gcd();
}

vectortriple get_triple(const abcd& baseAbcd, const mnpq& hint)
{
	vector<vectortriple> triples;
	set<vector3d> allVectors;
	vector<long long> perm = hint.as_vector();
	sort(perm.begin(), perm.end());
	do {
		mnpq alt(*(perm.begin()), *(perm.begin() + 1), *(perm.begin() + 2), *(perm.begin() + 3));
		set<vector3d> abcdvectors = alt.get_abcd().get_vectors();
		allVectors.insert(abcdvectors.begin(), abcdvectors.end());
	} while (next_permutation(perm.begin(), perm.end()));

	vector3d u(baseAbcd.a, baseAbcd.b, baseAbcd.c);
	for (set<vector3d>::const_iterator v = allVectors.begin(); v != allVectors.end(); v++)
	{
		if (v->is_orthogonal_to(u))
		{
			vector3d n = v->cross_product(u);
			return vectortriple(u, *v, n);
		}
	}
	throw runtime_error("No triple found!");
}

void solver::process_mnpq(mnpq& item)
{
	abcd baseAbcd = item.get_abcd();
	vectortriple baseTriple = get_triple(baseAbcd, item);

	long sumgcd = baseTriple.u.gcd() + baseTriple.v.gcd() + baseTriple.n.gcd(); //  accumulate(cube->uvn.begin(), cube->uvn.end(), 0, addgcd);
	long long l = baseTriple.u.length;

	if (util::gcd(set<long long>({ baseAbcd.a, baseAbcd.b, baseAbcd.c, baseAbcd.d })) == 1)
	{
		if ((itcount++ % 1000) == 0) cout << baseAbcd << endl;

		set<cube> cubes;
		set<abcd> abcds = get_permutations(baseAbcd);
		for (set<abcd>::const_iterator abcd = abcds.begin(); abcd != abcds.end(); abcd++)
		{
			vectortriple triple = get_triple(*abcd, item);

			for (int f = 0; f < 8; f++)
			{
				bool flipX = (f & 1) != 0,
					flipY = (f & 2) != 0,
					flipZ = (f & 4) != 0;
				int order[] = { 0,1,2 };
				do {
					cube c(triple.u, triple.v, triple.n, flipX, flipY, flipZ, order);
					if (cubes_done.insert(c).second)
					{
						cubes.insert(c);
					}
				} while (next_permutation(order, order + 3));
			}

		}
		if (cubeCounts.insert(cubes.size()).second)
			cout << cubes.size() << endl;

		BIGINT thisCxr = 0;
		BIGINT thisS = 0;
		for (set<cube>::const_iterator cube = cubes.begin(); cube != cubes.end(); cube++)
		{
			if (!cube->oversize)
			{
				long long width = cube->width,
					height = cube->height,
					depth = cube->depth;
				long long tmax = maxSide / (max(width, max(height, depth)));

				for (long long t = 1; t <= tmax; t++)
				{
					long long repeatability =
						(maxSide + 1LL - t * width) *
						(maxSide + 1LL - t * height) *
						(maxSide + 1LL - t * depth);

					thisCxr = thisCxr + BIGINT(repeatability);

					BIGINT ehp = BIGINT(l*l*l) * BIGINT(t*t*t)
						+ BIGINT(l*(sumgcd)) * BIGINT(t*t) + BIGINT((sumgcd)* t + 1);
					//from arXiv:1508.03643v2 [math.NT] 17 Mar 2016, theorem 2.14
					BIGINT contributionS = ehp * BIGINT(repeatability);

					thisS = thisS + contributionS;
				}
			}
		}


		C = C + thisCxr;
		S = S + thisS;

		if (maxResultDigits > 0 && ((itcount % 10) == 0))
		{
			C.truncate(maxResultDigits);
			S.truncate(maxResultDigits);
		}
	}
}

void solver::solve()
{
	for (long long m = 0; m <= sqrt(maxSide); m++)
	{
		long long nmax = sqrt(maxSide - m*m);
		for (long long n = 0; n <= nmax; n++)
		{
			long long pmax = sqrt(maxSide - m*m - n*n);
			for (long long p = 0; p <= pmax; p++)
			{
				long long qmax = sqrt(maxSide - m*m - n*n - p*p);
				for (long long q = 0; q <= qmax; q++)
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
