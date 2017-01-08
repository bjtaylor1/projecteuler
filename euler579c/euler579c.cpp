// euler579c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "solver.h"
#include "util.h"

using namespace std;


int main()
{
	solver s(5);
	s.solve();
	cout << "C(" << s.maxSide << ") = " << s.C << endl;
}

