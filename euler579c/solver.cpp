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
	abcd baseAbcd = item.get_abcd();
	vector<int> baseKey = baseAbcd.get_key();

	if (baseAbcd.d <= maxSide && (util::gcd(baseKey.begin(), baseKey.end()) == 1) &&   keys.insert(baseKey).second)
	{
		cout << *(baseKey.begin()) << "," << *(baseKey.begin() + 1) << "," << *(baseKey.begin() + 2);
		vector<long long> perm = item.as_vector();
		sort(perm.begin(), perm.end());
		set<vector3d> allVectors;
		do {
			mnpq alt(*(perm.begin()), *(perm.begin() + 1), *(perm.begin() + 2), *(perm.begin() + 3));
			set<vector3d> abcdvectors = alt.get_abcd().get_vectors();

			//vector<int> key = alt.get_abcd().get_key();

			//if (util::gcd(key.begin(), key.end()) == 1 && keys.insert(key).second)
			//{
			//	if (key.size() != 4) throw runtime_error("Sanity check failed!");
			//	cout << *(key.begin()) << "," << *(key.begin() + 1) << "," << *(key.begin() + 2) << "," << *(key.begin() + 3) << endl;
			//}

			allVectors.insert(abcdvectors.begin(), abcdvectors.end());
		} while (next_permutation(perm.begin(), perm.end()));

		bool anyMatchAbcd = false;
		for (set<vector3d>::const_iterator u = allVectors.begin(); u != allVectors.end(); u++)
		{
			for (set<vector3d>::const_iterator v = allVectors.begin(); v != allVectors.end(); v++)
			{
				if (u != v && v->is_orthogonal_to(*u))
				{
					vector3d n = u->cross_product(*v);
					long long gcd = util::gcd(set<long long>({ u->gcd(), v->gcd(), n.gcd() }));
					if (gcd == 1)
					{
						vector<int> vkey = v->get_key(),
							ukey = u->get_key(),
							nkey = n.get_key();
						if (vkey == baseKey || ukey == baseKey || nkey == baseKey)
						{
							anyMatchAbcd = true;
							cube c(*v, *u, n);
							item.add_cube(c);
						}
					}
				}
			}
		}
		//cout << "oversize;width;height;depth;v1;v2;v3;v4;v5;v6;v7;v8;U;V;N" << endl;
		//for (set<cube>::const_iterator cube = item.cubes.begin(); cube != item.cubes.end(); cube++)
		//{
		//	cout << cube->oversize << ";"
		//		<< cube->width << ";"
		//		<< cube->height << ";"
		//		<< cube->depth;
		//	for (set<vertex>::const_iterator vertex = cube->vertices.begin(); vertex != cube->vertices.end(); vertex++)
		//	{
		//		cout << ";" << *vertex;
		//	}
		//	for (set<vector3d>::const_iterator vector = cube->uvn.begin(); vector != cube->uvn.end(); vector++)
		//	{
		//		if(vector->gcd() == 1)
		//		cout << ";" << *vector;
		//		else cout << ";";
		//	}
		//	cout << endl;
		//}

		//if (someOversize && someNotOversize) throw runtime_error("Mix of oversize and not oversize!");

		if (!anyMatchAbcd) throw runtime_error("None match!");

		int oversize = 0, notOversize = 0;

		BIGINT thisCxr = 0;
		BIGINT thisS = 0;
		for (set<cube>::const_iterator cube = item.cubes.begin(); cube != item.cubes.end(); cube++)
		{
			if (cube->oversize) oversize++;
			else
			{
				notOversize++;
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

		if (maxResultDigits > 0)
		{
			C.truncate(maxResultDigits);
			S.truncate(maxResultDigits);
		}

		//if (notOversize == 24)
		//{
		//	int count = 0;
		//	cout << "Base abcd = " << item.get_abcd() << endl;
		//	for (set<cube>::const_iterator cube = item.cubes.begin(); cube != item.cubes.end(); cube++)
		//	{
		//		cout << (count++);
		//		set<vector<int>> keys;
		//		for (set<vector3d>::const_iterator vertex = cube->uvn.begin(); vertex != cube->uvn.end(); vertex++)
		//		{
		//			vector<int> key = vertex->get_key();
		//			keys.insert(key);
		//		}
		//		for (set<vector<int>>::const_iterator key = keys.begin(); key != keys.end(); key++)
		//		{
		//			cout << "  " << (key->x) << "," << key->y << "," << key->z;
		//		}
		//		cout << endl;
		//	}
		//	throw runtime_error("Stop.");
		//}
		cout << "," << notOversize << endl;
	}
}

void solver::solve()
{
	for (long long m = 0; m <= sqrt(maxSide); m++)
	{
		long long nmax = ceil(sqrt(maxSide - m*m));
		for (long long n = 0; n <= nmax; n++)
		{
			long long pmax = ceil(sqrt(maxSide - m*m - n*n));
			for (long long p = 0; p <= pmax; p++)
			{
				long long qmax = ceil(sqrt(maxSide - m*m - n*n - p*p));
				for (long long q = 0; q <= qmax; q++)
				{
					if (((m + n + p + q) % 2) == 1 && util::gcd(set<long long>({ m,n,p,q })) == 1)
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
