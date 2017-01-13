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

		if (argc > 1) {

			long long n = stoi(argv[1]);
			cube::maxSize = n;

			solver s(n, 9);

			s.solve();
			//cout << "C(" << s.maxSide << ") = " << s.C << endl;
			cout << "S(" << s.maxSide << ") = " << s.S << endl;
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

