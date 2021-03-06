// 0214.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
* Euler's totient function phi(n).
* http://en.wikipedia.org/wiki/Euler%27s_totient_function
*
*
*/

//#define N 400
//#define L 4
typedef unsigned long long ull;

#define N 40000000
#define L 25
using namespace std;
bitset<N> composite;
vector<ull> primes;
ull totients[N];

bool isprime(ull n)
{
	return !composite[n];
}

ull binary_gcd(ull u, ull v)
{
	ull shl = 0;

	while (u && v && u != v) {
		bool eu = !(u & 1);
		bool ev = !(v & 1);

		if (eu && ev) {
			++shl;
			u >>= 1;
			v >>= 1;
		}
		else if (eu && !ev) u >>= 1;
		else if (!eu && ev) v >>= 1;
		else if (u >= v) u = (u - v) >> 1;
		else {
			ull tmp = u;
			u = (v - u) >> 1;
			v = tmp;
		}
	}

	return !u ? v << shl : u << shl;
}

ull phi(const ull n);
ull make_phi(const ull n)
{
	// Base case
	if (n == 1)
		return 1;

	// Lehmer's conjecture
	if (isprime(n))
		return n - 1;

	// Even number?
	if (n & 1 == 0) {
		ull m = n >> 1;
		return !(m & 1) ? phi(m) << 1 : phi(m);
	}

	// For all primes ...
	for (std::vector<ull>::iterator p = primes.begin();
		p != primes.end() && *p <= n;
		++p)
	{
		ull m = *p;
		if (n % m) continue;

		// phi is multiplicative
		ull o = n / m;
		ull d = binary_gcd(m, o);
		return d == 1 ? phi(m)*phi(o) : phi(m)*phi(o)*d / phi(d);
	}
	cout << "FATAL error!" << endl;
	exit(1);
}

ull phi(const ull n)
{
	if (totients[n] > 0)
		return totients[n];
	totients[n] = make_phi(n);
	return totients[n];
}


bool is_length_correct(ull p)
{
	ull length, tot;
	for (length = 1, tot = p; tot > 1 ; length++)
	{
		tot = phi(tot);
		if (length > L) return false;
	}
	return length == L;
}

int main()
{
	composite[0] = true;
	composite[1] = true;
	for (ull p : {2, 3, 5, 7, 11, 13, 17, 19})
	{
		if (!composite[p])
		{
			for (ull mp = 2*p; mp < N; mp += p) composite[mp] = true;
		}
	}
	for (ull p = 23; p < N; p+=2)
	{
		if (!composite[p])
		{
			for (ull mp = 2*p; mp < N; mp += p) composite[mp] = true;
		}
	}
	primes.push_back(2);
	for (ull i = 3; i < N; i += 2)
	{
		if (!composite[i]) primes.push_back(i);
	}

	memset(totients, 0, sizeof(ull) * N);

	unsigned long long sum = 0;
	ull first = 0, last = 0;

	for (auto p : primes)
	{
		if (is_length_correct(p))
		{
			if (first == 0) first = p;
			last = p;
			sum += p;
		}
	}
	cout << sum << endl;
    return 0;
}

