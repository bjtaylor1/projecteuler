// euler581.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

bool isprime(long long t)
{
	for (long long f = (long long)((t / 2) + 1); f > 0; f--)
	{
		if (f < t && f > 1 && t % f == 0) return false;
	}
	return true;
}

template<typename T>
class iterative_result
{
private:
	long long depth;
public:
	iterative_result(long long Depth) : depth(Depth) {};
	map<long long, T> values;

	struct setter
	{
		iterative_result& r;
		long long index;
		setter(iterative_result& R, long long Index) : r(R), index(Index){}
		operator T() { return r.values[index]; }
		T& operator=(const T& rhs)
		{
			r.values.erase(index - r.depth);
			return r.values[index] = rhs;
		}
	};
	setter operator[](long long index) { return setter(*this, index); }

};

typedef iterative_result<long long> irl;
typedef iterative_result<double> ird;

pair<long long, long long> solve_pell(long long D)
{
	long long a0 = (long long)sqrt(D);
	irl U(2), V(2), a(2), P(2), Q(2);
	U[0] = 0;
	V[0] = 1;
	P[-1] = 1;
	Q[-1] = 0;
	a[0] = a0;
	P[0] = a0;
	Q[0] = 1;
	for (long long n = 0;; n++)
	{
		U[n + 1] = a[n] * V[n] - U[n];
		V[n + 1] = (D - U[n + 1] * U[n + 1]) / V[n];
		a[n + 1] = (a0 + U[n + 1]) / V[n + 1];
		if (a[n + 1] > a0)
		{
			pair<long long, long long> solution(P[n], Q[n]);
			return solution;
		}

		P[n + 1] = a[n + 1] * P[n] + P[n - 1];
		Q[n + 1] = a[n + 1] * Q[n] + Q[n - 1];
	}

}

long long get_highest_prime_factor(long long p)
{
	for (long long f = (long long)((p / 2) + 1);f > 0; f--)
	{
		if (p % f == 0 && isprime(f)) return f;
	}
	return 1;
}

int main()
{
	auto sln = solve_pell(13);
	cout << sln.first << "," << sln.second << endl;

	return 0;


	long long count = 0;
	long long tot = 0;
	for (long long n = 1; ; n++)
	{
		long long t = (n*(n + 1)) / 2;
		long long hpf = get_highest_prime_factor(t);
		if (hpf <= 47)
		{
			cout << "T(" << n << ") = " << t << ", hpf = " << hpf << ", total = " << (tot += n) << endl;
		}
		//else  if (count++ % 1000 == 0)
		//	cout << "n = " << n << ", hpf = " << hpf << endl;
	}
    return 0;
}

