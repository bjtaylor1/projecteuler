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
			overlong::MAX = n >= 5000 ? 1e9 : 0;

			testmassiveint(10001, 1003);
			testmassiveint(101, 12);
			testmassiveint(123, 2346487);
			testmassiveint(409265432165, 65465464321);


			//solver s(n);
			//
			//s.solve();
			//cout << "C(" << s.maxSide.val << ") = " << s.C << endl;
			//cout << "S(" << s.maxSide.val << ") = " << s.S << endl;
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

