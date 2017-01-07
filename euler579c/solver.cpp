#include "stdafx.h"
#include "solver.h"



void solver::solve()
{
	for (long m = 0; m <= sqrt(maxSide); m++)
	{
		for (long n = 0; n <= sqrt(maxSide - m*m); n++)
		{
			for (long p = 0; p <= sqrt(maxSide - m*m - n*n); p++)
			{
				long qStart = (m + n + p) % 2; //the total must be odd (otherwise it won't be primitive) - so q might as well go up in twos
				for (long q = qStart; q <= sqrt(maxSide - m*m - n*n - q*q); q += 2)
				{

				}
			}
		}
	}
}