using System;
using System.Collections.Generic;
using System.Linq;
using Mpir.NET;

namespace euler58
{
    class Program
    {
        static void Main(string[] args)
        {
            int i = 1;
            double d;
            mpz_t lastPrime = 2;
            int numTotal = 0;
            mpz_t[] diags = { 1 };
            double totPrimes = 0;
            double ratio = double.MaxValue;
            int totDiags = 1;
            for(int sideLength = 3; ; sideLength+= 2)
            {
                diags = new mpz_t[] { 1, 2, 3, 4 }.Select(s => diags.Last() + s * (sideLength - 1)).ToArray();
                var primes = new HashSet<mpz_t>();
                for (mpz_t prime = lastPrime; prime <= diags.Last(); lastPrime = prime = prime.NextPrimeGMP())
                {
                    primes.Add(prime);
                }
                primes.IntersectWith(diags);
                totPrimes += primes.Count;
                totDiags += 4;
                ratio = totPrimes / totDiags;
                if(sideLength % 10 == 1) Console.Write($"Side length = {sideLength}, ratio = {ratio}                \r");
                if(ratio < 0.1)
                {
                    Console.WriteLine(sideLength);
                    return;
                }
            }
        }
    }
}
