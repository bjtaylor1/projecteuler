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
			biglong n = 0;
			stringstream(argv[1]) >> n;
			solver s(n);
			s.solve();
			cout << "C(" << s.maxSide << ") = " << s.C << endl;
			cout << "S(" << s.maxSide << ") = " << s.S << endl;
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

