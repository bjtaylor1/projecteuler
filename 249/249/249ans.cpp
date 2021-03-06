#include "stdafx.h"

//#define SIZE 1570000
#define SIZE 1570000
#define TARGET 20
typedef unsigned __int64 U64;

static char primes[SIZE];
static U64 buf[SIZE];
static U64 l16 = 10000000000000000ULL;
using namespace std;

void sieve()
{
	memset(primes, 1, sizeof primes);
	primes[0] = primes[1] = 0;
	for (int i = 2; i * i < SIZE; i++) {
		for (int j = i * i; j < SIZE; j += i) primes[j] = 0;
	}
}

int main(int argc, char **argv)
{
	sieve();
	for (int i = 0; i < SIZE; i++) buf[i] = 0;
	buf[0] = 1;
	int max = 0;

	for (int i = 2; i < TARGET; i++) {
		if (primes[i]) {
			for (int j = max; j >= 0; j--) {
				buf[j + i] += buf[j];
				if (buf[j + i] >= l16) buf[j + i] -= l16;
			}
			max += i;
			cout << "i = " << i << ", max is now " << max << endl;
		}
	}
	U64 sum = 0;

	for (int i = 2; i <= max; i++)
	{
		cout << "buf[" << i << "] = " << buf[i] << endl;
		if (primes[i]) sum = (sum + buf[i]) % l16;
	}
	printf("%I64d\n", sum);
	return 0L;
}