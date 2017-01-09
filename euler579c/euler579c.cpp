// euler579c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "solver.h"
#include "util.h"
#include "massiveinteger.h"

using namespace std;

void testmassiveint(int i, int j)
{
	massiveinteger mi(i), mj(j);
	cout << mi << " + " << mj << " == " << (mi + mj) << endl;

}

int main(int argc, char** argv)
{
	try {

		if (argc > 1) {
			long long n = stoi(argv[1]);
			overlong::MAX = n >= 5000 ? 1e9 : 0;

			testmassiveint(99, 999);


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

