// euler579c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "solver.h"
#include "util.h"

using namespace std;


int main(int argc, char** argv)
{
	try {

		if (argc > 1) {
			long long n = 0;
			stringstream(argv[1]) >> n;
			overlong::MAX = n >= 5000 ? 1e9 : 0;

			solver s(n);
			
			s.solve();
			cout << "C(" << s.maxSide.val << ") = " << s.C << endl;
			cout << "S(" << s.maxSide.val << ") = " << s.S << endl;
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

