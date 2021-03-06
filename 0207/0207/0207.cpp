// 0207.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
typedef unsigned long long ull;

ull isint(double d)
{
	ull dl = (ull)(d+0.5);
	return abs(d - dl) < 1e-10 ? dl : 0;
}

int main()
{
	ull prop = 12345;

	ull perfects = 0, total = 0;
	ull oldtotal;
	for (ull p = 1, lpt = 1, pt = 2;; p++)
	{
		total += pt - lpt - 1; //roll forward to the state just *before* the next power of 2
		if ((p-1) * prop < total)
		{
			ull mintotal = (prop * (p-1)) + 1; //the minimum total we need to get the desired proportion
			ull twotothet = lpt + mintotal - oldtotal; //the first 2^t for which P(m) < 1/prop, note t might not be int
			ull t = (twotothet * twotothet) - twotothet;
			cout << t << endl;
			return 0;
		}
		lpt <<= 1; pt <<= 1;
		perfects++;
		total++;
		oldtotal = total;
	}

	//ull total = 0, perfect = 0, tl, moflastperfect;
	//for (ull fourtl = 2; ; fourtl++)
	//{
	//	double t = log(fourtl) / log(4);
	//	double twotd = pow(2, t);
	//	ull m = 0, twotl = 0;
	//	if ((twotl = isint(twotd)) > 0)
	//	{
	//		m = fourtl - twotl;
	//		total++;
	//		if (isint(t))
	//		{
	//			moflastperfect = m;
	//			perfect++;
	//		}
	//		cout << fourtl << " = " << twotl << " + " << m << " ( " << perfect << " / " << total << ")";
	//		if (isint(t))
	//		{
	//			cout << " P";
	//		}

	//		if (perfect  * prop < total)
	//		{
	//			cout << " == " << m << endl;
	//		}
	//		cout << endl;
	//	}
	//	//else cout << ".";
	//}
	return 0;
}

