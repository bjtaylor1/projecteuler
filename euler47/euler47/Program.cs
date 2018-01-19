using System;
using System.Collections.Generic;
using System.Linq;
using Mpir.NET;

namespace euler47
{
    class Program
    {
        private static readonly HashSet<mpz_t> primes = new HashSet<mpz_t> { 2 };
        static bool HasDistinctPrimeFactors(mpz_t n, int numDpfRequired)
        {
            while (primes.Last() < n / 2) primes.Add(primes.Last().NextPrimeGMP());
            int dpf = 0;
            foreach(var p in primes)
            {
                if (n % p == 0 && ++dpf >= numDpfRequired) return true;
            }
            return false;
        }

        static void Main(string[] args)
        {
            const int N = 4;
            const int NUM_PRIMES_REQUIRED = 1000;
            const int RESULT_MAX = 1000000;
            var primes = new HashSet<int> { 2 };
            for (int i = 0; i < NUM_PRIMES_REQUIRED; i++) primes.Add((int)new mpz_t(primes.Last()).NextPrimeGMP());
            var numFactors = new int[RESULT_MAX];
            foreach (var p in primes)
                for (int pf = p; pf < RESULT_MAX; pf += p)
                    numFactors[pf]++;
            Console.WriteLine("Made sieve");
            for(int i = 0; i < RESULT_MAX; i++)
                if(numFactors.Skip(i).Take(N).All(n => n >= N))
                {
                    Console.WriteLine(i);
                    return;
                }
        }
    }
}
