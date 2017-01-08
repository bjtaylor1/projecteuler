// euler579c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "solver.h"
#include "util.h"

using namespace std;


int main()
{
	cout << util::gcd(set<long>({ 4,8,12,2 })) << endl;
	cout << util::gcd(set<long>({ 15,10,5})) << endl;
	cout << util::gcd(set<long>({ 2,15,10})) << endl;

	//solver s(10);
	//s.process_mnpq(mnpq(1,1,2,3));

	//s.solve();
}

