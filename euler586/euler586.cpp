// euler586.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mpz_w.h"
#include "euler586.h"

using namespace std;

#define N 100000
#define R 4
long long K[N + 1];

mpz_w As[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };

long long factor2(long long nminus1, long long& twotother, long long& r, long long& d)
{
	return 0;
}


bool is_prime(mpz_w n)
{
	if (n <= 3 || n.is_even()) return true;
	mpz_w d, nminus1 = n - mpz_w::one;
	long long s = mpz_scan1(nminus1.val, 0); //n = 2^s.d
	mpz_tdiv_q_2exp(d.val, nminus1.val, s);
	for (mpz_w a : As)
	{
		mpz_w atothedmodn;
		mpz_powm(atothedmodn.val, a.val, d.val, n.val);
		if (atothedmodn != mpz_w::one)
		{
			for (long long r = 0; r < s; r++)
			{
				mpz_w dtwotother;
				mpz_mul_2exp(dtwotother.val, d.val, r);
				mpz_w atothedtwotother;
				mpz_powm(atothedtwotother.val, a.val, dtwotother.val, n.val);
				if (++atothedtwotother != n)
				{
					return false;
				}
			}
		}
	}
	return true;
}

int main()
{
	long incorrect = 0, correct = 0;
	//pseudoprimes:
	for (long long p : { 221,
		2047, 3277, 4033, 4681, 8321, 15841, 29341, 42799, 49141, 52633, 65281, 74665, 80581, 85489, 88357, 90751, //base 2 pseudoprimes
			121, 703, 1891, 3281, 8401, 8911, 10585, 12403, 16531, 18721, 19345, 23521, 31621, 44287, 47197, 55969, 63139, 74593, 79003, 82513, 87913, 88573, 97567, //base 3 pseudoprimes
			781, 1541, 5461, 5611, 7813, 13021, 14981, 15751, 24211, 25351, 29539, 38081, 40501, 44801, 53971, 79381 //base 5 pseudoprimes
	})
	{
		bool isprime = is_prime(p);
		if (isprime)
		{
			incorrect++;
			cout << p << " is_prime WRONG - returned true for non prime!" << endl;
		}
		else
		{
			correct++;
			cout << p << " - correct" << endl;
		}
	}

		//primes:
	for (long long p : {5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953, 5981, 5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133, 6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301, 6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473, 6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673, 6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761, 6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833, 6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997, 7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103, 7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207, 7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411, 7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499, 7507})
	{
		bool isprime = is_prime(p);
		if (!isprime)
		{
			incorrect++;
			cout << p << " is_prime WRONG - returned false for a prime!" << endl;
		}
		else
		{
			correct++;
			cout << p << " - correct" << endl;
		}
	}

	cout << "Correct: " << correct << ", incorrect: " << incorrect << endl;




	bool b = is_prime(221);

	return 0;
	memset(K, 0, N * sizeof(long long));

	for (long long a = 1; a <= N; a++)
	{
		for (long long b = a + 1; b <= N; b++)
		{
			long long res = a*a + 3 * a*b + b*b;
			if (res == 9889) cout << a << "," << b << endl;
			if (res > N)
				break;
			else
				K[res]++;
		}
	}
	long long tot = 0;
	for (long long ki = 0; ki <= N; ki++)
	{
		if (K[ki] == R)
		{
			cout << ki << ",";
			tot++;
		}
	}
	cout << endl << endl << tot << endl;
	return 0;
}
