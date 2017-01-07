#include "stdafx.h"
#include "solver.h"

void solver::process_mnpq(const mnpq& mnpq)
{
	
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