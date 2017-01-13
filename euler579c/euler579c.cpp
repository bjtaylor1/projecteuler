// euler579c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "solver.h"
#include "util.h"
#include "massiveinteger.h"

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

			long long n = stoi(argv[1]),
				numThreads = stoi(argv[2]);

			cube::maxSize = n;
			solver::maxSide = n;
			solver::maxResultDigits = 9;// n >= 5000 ? 9 : 12;
			solver::numThreads = numThreads;

			solver::solve();
			//cout << "C(" << solver::maxSide << ") = " << solver::C << endl;
			cout << "S(" << solver::maxSide << ") = " << solver::S << endl;
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

