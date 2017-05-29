// euler600.3.c.cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"
#include "hexagon.h"

using namespace std;

long main(long argc, char** argv)
{
	double xp = cos(M_PI / 3);
	double yp = sin(M_PI / 3);
	long N = stoi(argv[1]);
	long sMax = (N - 4) / 2;
	unsigned long long totalHexagons = 0;
	for (long s0 = 1; s0 <= sMax; s0++)
	{
		for (long s1 = s0; s1 <= sMax - s0; s1++)
		{
			for (long s2 = 1; s2 <= (N / 2) - s0 - s1; s2++)
			{
				for (long s3 = 1; s3 <= sMax && s3 < N - s0 - s1 - s2; s3++)
				{
					for (long s4 = 1; s4 <= sMax && s4 < N - s0 - s1 - s2 - s3; s4++)
					{
						double x = s0- s3 + (s1 - s2 - s4) * xp;
						double y = (s1 + s2 - s4) * yp;
						if (y <= 0) break;

						if (abs(x / y + xp / yp) < 1e-10)
						{
							double s5d = sqrt(x * x + y * y);
							long s5 = (long)round(s5d);
							if (abs(s5 - s5d) < 1e-10 && s0 + s1 + s2 + s3 + s4 + s5 <= N)
							{
								hexagon h(vector<long>({ s0, s1, s2, s3, s4, s5 }));
								if (h.sides == h.sides_orig)
								{
									totalHexagons++;
								}
							}
						}
					}
				}
			}
		}
	}
	cout << "There are " << totalHexagons << " hexagons." << endl;

    return 0;
}

