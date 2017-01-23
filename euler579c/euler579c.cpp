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

int main(int argc, char** argv)
{
	try {
		if (argc > 2) {

			//vector3d u(-65, 156, 1092);
			//vector3d v(420, 1015, -120);
			//vector3d n(1020, -408, 119);
			//vectortriple vt(u, v, n);
			//cube c(vt);

			//cout << c.width << "," << c.height << "," << c.depth << endl;


			long long over231 = 2147483647LL + 100LL;
			BIGINT b(over231);


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
			//cout << "C(" << solver::maxSide << ") = " << solver::C << endl;
			cout << "S(" << solver::maxSide << ") = " << solver::S.mod10e(n <= 50 ? 15 : 9) << " (" << solver::S.all() << ")" << endl;
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

