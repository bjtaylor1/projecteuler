// euler581.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

#define MAX(arg1,arg2) (arg1 > arg2 ? arg1 : arg2)

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
	//http://mathafou.free.fr/themes_en/kpell.html
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
			if (n % 2 == 1)
			{
				pair<long long, long long> solution(P[n], Q[n]);
				return solution;
			}
			else
			{
				pair<long long, long long> solution(P[n] * P[n] + D * Q[n] * Q[n], 2 * P[n] * Q[n]);
				return solution;
			}
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

void makesquarefrees(const set<long long>& primes, set<long long>& current, set<long long>& squarefrees, long long sf)
{
	if(sf != 2) squarefrees.insert(sf);
	for (auto p : primes)
	{
		if (current.insert(p).second)
		{
			makesquarefrees(primes, current, squarefrees, p*sf);
			current.erase(p);
		}
	}
}

vector<pair<long long, long long>> makemoresolutions(pair<long long, long long> fundamental, long long numberOfSolutions, long long D)
{
	long long x = fundamental.first, y = fundamental.second;
	vector<pair<long long, long long>> result;
	for (long long i = 0; i < numberOfSolutions; i++)
	{
		result.push_back(pair<long long, long long>(x, y));
		long long newx = fundamental.first * x + D * fundamental.second * y;
		long long newy = fundamental.first * y + fundamental.second * x;
		x = newx;
		y = newy;
	}
	return result;
}

void addtosquarefrees(long long current, set<long long>& squarefrees, set<long long>& primes)
{
	for (long long p : primes)
	{
		long long sf = current * p;
		if (sf != 2) squarefrees.insert(sf);
		set<long long> otherPrimes = primes;
		otherPrimes.erase(p);
		addtosquarefrees(sf, squarefrees, otherPrimes);
	}
}

set<long long> makesquarefrees4(const set<long long>& primesSet)
{
	set<long long> squarefrees;
	int size = primesSet.size();
	if (size > 64) throw runtime_error("Too many");
	long long primesArray[32];
	long long i = 0;
	for (long long p : primesSet)
	{
		primesArray[i++] = p;
	}
	long long max = (long long)pow(2, size);
	for (long long i = 1; i < max; i++)
	{
		long long t = 1;
		for (int bit = 0; bit < size; bit++)
		{
			if (((1 << bit) & i) != 0) t *= primesArray[bit];
		}
		if(t != 2) squarefrees.insert(t);
	}
	cout << endl;
	return squarefrees;
}



set<long long> makesquarefrees2(const set<long long>& primesSet) //it's copied, not referenced
{
	set<long long> squarefrees, primes = primesSet;
	addtosquarefrees(1, squarefrees, primes);
	return squarefrees;
}

set<long long> makesquarefrees(const set<long long>& primesSet) //it's copied, not referenced
{
	vector<long long> primes(primesSet.begin(), primesSet.end());
	set<long long> squarefrees;
	do
	{
		long long tot = 1;

		for (auto p : primes)
		{
			long long sf = tot *= p;
			if (sf != 2) squarefrees.insert(sf);
		}
	} while (next_permutation(primes.begin(), primes.end()));
	return squarefrees;
}

int main(int argc, char** argv)
{
	//from https://en.wikipedia.org/wiki/St%C3%B8rmer's_theorem
	if (argc < 2) return 1;
	long long N = stoi(argv[1]);
	
	set<long long> primes;
	long long pk;
	for (long long p = 1; p <= N; p++)	if (isprime(p)) primes.insert(pk = p);
	cout << "made " << primes.size() << " primes, the highest of which is " << pk << endl;
	set<long long> squarefrees = makesquarefrees4(primes);

	cout << "made " << squarefrees.size() << " squarefrees" << endl;
	long long tot = 0;
	long long maxsolutions = MAX(3, (pk + 1) / 2);
	long long count = 0;
	for (long long sf : squarefrees)
	{
		if (++count % 100 == 0) cout << "\r" << count << " of " << squarefrees.size();
		auto firstsolution = solve_pell(sf*2);
		auto allsolutions = makemoresolutions(firstsolution, maxsolutions, sf*2);
		for (auto solution : allsolutions)
		{
			long long p1 = (solution.first - 1) / 2, p2 = (solution.first + 1) / 2;
			long long t = (p1 * p2) / 2;

			if (get_highest_prime_factor(t) <= pk)
			{
				tot += p1;
			}
		}
	}
	cout << endl << tot << endl;
    return 0;
}

