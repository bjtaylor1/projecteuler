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

vector<vectortriple> get_triples(const abcd& baseAbcd, const mnpq& hint)
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

	for (int m = 0; m < 8; m++)
	{
		int m0 = (m & 1) == 0 ? 1 : -1;
		int m1 = (m & 2) == 0 ? 1 : -1;
		int m2 = (m & 4) == 0 ? 1 : -1;
		
		vector3d u(baseAbcd.a * m0, baseAbcd.b * m1, baseAbcd.c * m2);
		for (set<vector3d>::const_iterator v = allVectors.begin(); v != allVectors.end(); v++)
		{
			if (v->is_orthogonal_to(u))
			{
				vector3d n = v->cross_product(u);
				triples.push_back(vectortriple(u, *v, n));
			}
		}
	}
	return triples;
}

void solver::process_mnpq(mnpq& item)
{
	abcd baseAbcd = item.get_abcd();

	if (util::gcd(set<long long>({ baseAbcd.a, baseAbcd.b, baseAbcd.c, baseAbcd.d })) == 1)
	{
		if ((itcount++ % 1000) == 0) cout << baseAbcd << endl;
		
		vector<vectortriple> triples = get_triples(baseAbcd, item);

		set<cube> cubes;
		for (vector<vectortriple>::const_iterator triple = triples.begin(); triple != triples.end(); triple++)
		{
			int swapPos[] = { 0, 1, 2 };
			do {
				int uArray[] = { triple->u.x, triple->u.y, triple->u.z };
				int vArray[] = { triple->v.x, triple->v.y, triple->v.z };
				int nArray[] = { triple->n.x, triple->n.y, triple->n.z };

				vector3d u(uArray[swapPos[0]], uArray[swapPos[1]], uArray[swapPos[2]]);
				vector3d v(vArray[swapPos[0]], vArray[swapPos[1]], vArray[swapPos[2]]);
				vector3d n(nArray[swapPos[0]], nArray[swapPos[1]], nArray[swapPos[2]]);
				cube c(u, v, n);
				cubes.insert(c);
			} while (next_permutation(swapPos, swapPos + 3));
		}

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
