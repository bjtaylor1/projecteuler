// euler581.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mpz_w.h"

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

typedef iterative_result<mpz_w> irl;

pair<mpz_w, mpz_w> solve_pell(long long D)
{
	//http://mathafou.free.fr/themes_en/kpell.html
	mpz_w a0((long long)sqrt(D));
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
				pair<mpz_w, mpz_w> solution(P[n], Q[n]);
				return solution;
			}
			else
			{
				pair<mpz_w, mpz_w> solution(P[n] * P[n] + D * Q[n] * Q[n], 2 * P[n] * Q[n]);
				return solution;
			}
		}

		P[n + 1] = a[n + 1] * P[n] + P[n - 1];
		Q[n + 1] = a[n + 1] * Q[n] + Q[n - 1];
	}

}

mpz_w get_highest_prime_factor(mpz_w p)
{
	for (mpz_w f = 2; f <= p.sqrt(); ++f)
	{
		while (p % f == 0) p /= f;
	}
	return p;
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

vector<pair<mpz_w, mpz_w>> makemoresolutions(pair<mpz_w, mpz_w> fundamental, long long numberOfSolutions, long long D)
{
	mpz_w x = fundamental.first, y = fundamental.second;
	vector<pair<mpz_w, mpz_w>> result;
	for (long long i = 0; i < numberOfSolutions; i++)
	{
		result.push_back(pair<mpz_w, mpz_w>(x, y));
		mpz_w newx = fundamental.first * x + D * fundamental.second * y;
		mpz_w newy = fundamental.first * y + fundamental.second * x;
		x = newx;
		y = newy;
	}
	return result;
}

set<long long> makesquarefrees(const set<long long>& primesSet)
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

int main(int argc, char** argv)
{
	//from https://en.wikipedia.org/wiki/St%C3%B8rmer's_theorem
	if (argc < 2) return 1;
	long long N = stoi(argv[1]);
	
	set<long long> primes;
	long long pk;
	for (long long p = 1; p <= N; p++)	if (isprime(p)) primes.insert(pk = p);
	cout << "made " << primes.size() << " primes, the highest of which is " << pk << endl;
	set<long long> squarefrees = makesquarefrees(primes);

	cout << "made " << squarefrees.size() << " squarefrees" << endl;
	mpz_w tot = 0;
	long long maxsolutions = MAX(3, (pk + 1) / 2);
	long long count = 0;
	for (long long sf : squarefrees)
	{
		auto firstsolution = solve_pell(sf*2);
		auto allsolutions = makemoresolutions(firstsolution, maxsolutions, sf*2);
		cout << "\r" << (count++) << " of " << squarefrees.size() << ": " << allsolutions.size() << " solutions, highest = " << allsolutions.rbegin()->first.val;
		for (auto solution : allsolutions)
		{
			mpz_w p1 = (solution.first - 1) / 2, p2 = (solution.first + 1) / 2;
			mpz_w t = (p1 * p2) / 2;

			if (get_highest_prime_factor(t) <= pk)
			{
				tot += p1;
			}
		}
	}
	cout << endl << tot.val << endl;
    return 0;
}

