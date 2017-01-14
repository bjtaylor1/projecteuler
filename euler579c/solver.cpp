#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"
#include "util.h"
#include "macros.h"
#include "vectortriple.h"
#include "blockingqueue.h"

long long itcount = 0;

long long addgcd(long long current, const vector3d& v)
{
	return current + v.gcd();
}

BIGINT /*solver::C(0),*/ solver::S(0);
long long solver::maxSide = 0, solver::maxResultDigits = 0, solver::numThreads = 0;
set<cube> solver::cubes_done;

blockingqueue<mnpq> mnpq_queue(10000);
mutex m_data;

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



void solver::process_mnpq(const mnpq& item)
{
	abcd baseAbcd = item.get_abcd();
	vectortriple baseTriple = get_triple(baseAbcd, item);

	long sumgcd = baseTriple.u.gcd() + baseTriple.v.gcd() + baseTriple.n.gcd(); //  accumulate(cube->uvn.begin(), cube->uvn.end(), 0, addgcd);
	long long l = baseTriple.u.length;

	if (util::gcd(set<long long>({ baseAbcd.a, baseAbcd.b, baseAbcd.c, baseAbcd.d })) == 1)
	{
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
					cube c(cube::get_vertices(triple.u, triple.v, triple.n, flipX, flipY, flipZ, order));
					cubes.insert(c);
				} while (next_permutation(order, order + 3));
			}

		}

		//BIGINT thisCxr = 0;
		BIGINT thisS = 0;
		for (set<cube>::const_iterator thecube = cubes.begin(); thecube != cubes.end(); thecube++)
		{
			if (!thecube->is_oversize())
			{
				bool inserted;
				{
					lock_guard<mutex> lm(m_data);
					inserted = cubes_done.insert(*thecube).second;
				}
				if (inserted)
				{
					long long width = thecube->width,
						height = thecube->height,
						depth = thecube->depth;
					long long tmax = maxSide / (max(width, max(height, depth)));
					if (tmax <= 0) throw runtime_error("tmax <= 0");
					for (long long t = 1; t <= tmax; t++)
					{
						cube cm = (*thecube) * t;
						if (cm.is_oversize()) throw runtime_error("Cube multiplied is oversize!");

						long long repeatability =
							(maxSide + 1LL - cm.width) *
							(maxSide + 1LL - cm.height) *
							(maxSide + 1LL - cm.depth);

						if (repeatability <= 0) throw runtime_error("Repeatability is <= 0 (oversize cube?)");

						//thisCxr = thisCxr + BIGINT(repeatability);

						BIGINT ehp = BIGINT(l*l*l) * BIGINT(t*t*t)
							+ BIGINT(l*(sumgcd)) * BIGINT(t*t) + BIGINT((sumgcd)* t + 1);
						//from arXiv:1508.03643v2 [math.NT] 17 Mar 2016, theorem 2.14
						BIGINT contributionS = ehp * BIGINT(repeatability);

						thisS = thisS + contributionS;
					}
				}
			}
		}

		{
			lock_guard<mutex> lm(m_data);
			//C = C + thisCxr;
			S = S + thisS;
			if (maxResultDigits > 0 && ((itcount % 10) == 0))
			{
				//C.truncate(maxResultDigits);
				S.truncate(maxResultDigits);
			}
		}

	}
}

void processor()
{
	try
	{
		while (true)
		{
			mnpq item = mnpq_queue.pop();
			try
			{
				solver::process_mnpq(item);
			}
			catch (runtime_error e)
			{
				stringstream ss;
				ss << e.what() << " while processing " << item;
				cout << ss.str() << endl;
				exit(1);
			}
		}
	}
	catch (queuefinished)
	{
		//queue finished
	}

}

void solver::solve()
{
	vector<thread> threads;
	for (int i = 0; i < numThreads; i++)
	{
		threads.push_back(thread(processor));
	}

	for (long long m = 0; m <= ceil(sqrt(maxSide)) + 1.0; m++)
	{
		long long nmax = ceil(sqrt(maxSide - m*m)) + 1.0;
		for (long long n = 0; n <= nmax; n++)
		{
			long long pmax = ceil(sqrt(maxSide - m*m - n*n)) + 1.0;
			for (long long p = 0; p <= pmax; p++)
			{
				long long qmax = ceil(sqrt(maxSide - m*m - n*n - p*p)) + 1.0;
				for (long long q = 0; q <= qmax; q++)
				{
					if (((m + n + p + q) % 2) == 1)
					{
						mnpq it(m, n, p, q);
						mnpq_queue.push(it);
					}
				}
			}
		}
	}
	mnpq_queue.set_finished();
	for (vector<thread>::iterator thread = threads.begin(); thread != threads.end(); thread++)
	{
		thread->join();
	}
}
