// euler579c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "solver.h"
#include "util.h"
#include "massiveinteger.h"
#include "vector3d.h"

using namespace std;

void testmassiveint(unsigned long long i, unsigned long long j)
{
	massiveinteger mi(i), mj(j);
	cout << mi << " + " << mj << " == " << (mi + mj) << " == " << (i+j) << endl;
	cout << mi << " x " << mj << " == " << (mi * mj) << " == " << (i*j) << endl;
}

class cubecount
{
public:
	cubecount(long long Counted) : counted(Counted), frequency(0) {}
	long long counted;
	mutable long long frequency;
};

bool operator<(const cubecount& c1, const cubecount& c2)
{
	return c1.counted < c2.counted;
}

int main(int argc, char** argv)
{
	try {
		if (argc > 2) {

			double d = cos(M_PI);
			double angleDegrees = 180.0;
			double angleRads = angleDegrees * M_PI / 180;
			double d1 = cos(angleRads);
			long long l1 = my_round(d1);

			long long n = stoi(argv[1]),
				numThreads = stoi(argv[2]);

			cube::maxSize = n;
			solver::maxSide = n;
			solver::maxResultDigits = n > 50 ? 9 : 12;
			solver::numThreads = numThreads;

			solver::solve();

			set<cubecount> cubecounts;
			for (cube c : solver::cubes_done)
			{
				cubecounts.insert(c.counted).first->frequency++;
			}
			for (cubecount cc : cubecounts)
			{
				cout << "Counted " << cc.counted << " times: " << cc.frequency << " cubes" << endl;
			}

			//cout << "C(" << solver::maxSide << ") = " << solver::C << endl;
			cout << "S(" << solver::maxSide << ") = " << solver::S.mod10e(n <= 50 ? 15 : 9) 
#ifdef NOTRUNCATE
				<< " (" << solver::S.all() << ")" 
#endif
				<< endl;
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

