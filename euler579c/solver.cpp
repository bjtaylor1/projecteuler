#include "stdafx.h"
#include "mnpq.h"
#include "solver.h"
#include "util.h"
#include "macros.h"
#include "vectortriple.h"
#include "blockingqueue.h"
#include "primemultiples.h"

long long addgcd(long long current, const vector3d& v)
{
	return current + v.gcd();
}

BIGINT /*solver::C(0),*/ solver::S(0);
long long solver::maxSide = 0, solver::numThreads = 0, solver::countOne = 0;
size_t solver::maxResultDigits = 0;
set<cube> solver::cubes_done;
long long solver::primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73 };
primemultiples solver::primeMultiples;

blockingqueue<mnpq> mnpq_queue(10000);
mutex m_data, m_primemultiples;


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

	//abcd key = baseAbcd.to_key();
	//if (key.a == 7 && key.b == 24 && key.c== 60)
	//{
	//	cout << item << endl;
	//	exit(1);
	//}

	if (baseAbcd.d <= maxSide)
	{
		vectortriple baseTriple = get_triple(baseAbcd, item);
		if (baseTriple.u.gcd() == 1 || baseTriple.v.gcd() == 1 || baseTriple.n.gcd() == 1)
		{
			if (baseAbcd.d >= 65) //optimization - 65 is the smallest side of first primitive cube with all vectors non-primitive / gcd of that vector (ref. Ionascu)
			{
				for (long long prime : primes)
				{
					//is it already paired?
					long long multiple = prime * baseAbcd.d;
					set<primemultiple> pmsThis = primeMultiples[multiple];


					//store this one's info
					if (multiple >= maxSide) break;
					primeMultiples.add(multiple, prime, item);
				}
			}

			set<cube> cubes;
			for (long long x = 0; x < 4; x++)
			{
				for (long long y = 0; y < 4; y++)
				{
					for (long long z = 0; z < 4; z++)
					{
						//for (long long r = 0; r < 4; r++)
						{
							vectortriple vt = /*reflection[r] * */ (t_x[x] * (t_y[y] * (t_z[z] * baseTriple)));
							cube c(vt);
							cubes.insert(c);
						}
					}
				}
			}

			long long l = baseTriple.u.length;

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
						vectortriple rt = thecube->get_triple();
						if (!(rt.n.gcd() == 1 || rt.u.gcd() == 1 || rt.v.gcd() == 1)) throw runtime_error("Non primitive cube!");

						long long width = thecube->width,
							height = thecube->height,
							depth = thecube->depth;
						long long maxSize = max(width, max(height, depth));
						long long tmax = maxSide / maxSize;
						if (tmax * maxSize > maxSide) throw runtime_error("tmax is too lenient - would produce oversize cubes!");
						if ((tmax + 1) * maxSize <= maxSide) throw runtime_error("tmax is not lenient enough - could squeeze another one out!");

						if (tmax <= 0) throw runtime_error("tmax <= 0");
						BIGINT thisContributionS;
						for (long long t = 1; t <= tmax; t++)
						{
							BIGINT repeatability =
								BIGINT(maxSide + 1LL - thecube->width*t) *
								BIGINT(maxSide + 1LL - thecube->height*t) *
								BIGINT(maxSide + 1LL - thecube->depth*t);

							//thisCxr = thisCxr + BIGINT(repeatability);

							BIGINT ehp = BIGINT(l*l*l) * BIGINT(t*t*t)
								+ BIGINT(l*(thecube->sumgcd)) * BIGINT(t*t)
								+ BIGINT((thecube->sumgcd)* t + 1);
							//from arXiv:1508.03643v2 [math.NT] 17 Mar 2016, theorem 2.14
							BIGINT contributionS = ehp * repeatability;

							thisContributionS = thisContributionS + contributionS;
						}
						thisS = thisS + thisContributionS;
					}
				}
			}

			{
				lock_guard<mutex> lm(m_data);
				//C = C + thisCxr;
				S = S + thisS;
				//if (maxResultDigits > 0)
				//{
				//	//C.truncate(maxResultDigits);
				//	S.truncate(maxResultDigits);
				//}
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

	for (long long m = 0; m <= ceil(/*sqrt*/(maxSide)) + 1.0; m++)
	{
		long long nmax = (long long)ceil(sqrt(maxSide - m*m));
		for (long long n = 0; n <= nmax; n++)
		{
			long long pmax = (long long)ceil(sqrt(maxSide - m*m - n*n));
			for (long long p = 0; p <= pmax; p++)
			{
				long long qmax = (long long)ceil(sqrt(maxSide - m*m - n*n - p*p));
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
