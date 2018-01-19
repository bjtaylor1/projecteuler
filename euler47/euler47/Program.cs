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
            for(mpz_t i = 2; ; i++)
            {
                bool found = true;
                mpz_t j = new mpz_t(i);
                for (; j < i+N; j++)
                {
                    if (!HasDistinctPrimeFactors(j, N))
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                {
                    Console.WriteLine(i);
                    return;
                }
                else i = j;
            }
        }
    }
}
