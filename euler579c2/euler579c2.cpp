#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "massiveinteger.h"

using namespace std;

long long gcd(long long a, long long b)
{
	if (!b) return a;
	return gcd(b, a%b);
}

int main(int argc, char** argv)
{
	if (argc < 2) return 1;

	long mod = (long)(1e9);
	long maxval = 2147483647;
	long times = maxval * 2;
	long exp = times % mod;
	
	long long actualtimes = ((long long)maxval) * 2;
	long long actualexp = actualtimes % mod;

	return 0;

	//long long N = stoi(argv[1]);
	//for (long long a = -sqrt(N); a <= sqrt(N); a++)
	//{
	//	for (long long b = -sqrt(N - a*a); b <= sqrt(N - a*a); b++)
	//	{
	//		for (long long c = -sqrt(N - a*a - b*b); c <= sqrt(N - a*a - b*b); c++)
	//		{
	//			for (long long d = -sqrt(N - a*a - b*b - c*c); d <= sqrt(N - a*a - b*b - c*c); d++)
	//			{
	//				if ((a || b || c || d) && abs(gcd(gcd(a, b), gcd(c, d))) == 1 && (a*a + b*b + c*c + d*d) % 2 == 1)
	//				{
	//					if ((a*a + b*b + c*c + d*d) == 1105 
	//						&& (a*a + b*b - c*c - d*d) == -65 
	//						&& (2*(b*c + d*a)) == 156 
	//						&& (2*(b*d - c*a)) == 1092 
	//						&& (a*a - b*b - c*c + d*d) == 119
	//						/*&& (2*(b*c - d*a) == 420)*/
	//						)
	//					{
	//						cout << a << "," << b << "," << c << "," << d << endl;
	//					}
	//						
	//				}
	//			}
	//		}
	//	}
	//}
    return 0;
}

