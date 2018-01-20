using System;
using System.Collections.Generic;
using System.Linq;
using Mpir.NET;

namespace euler50
{
    class Program
    {
        static void Main(string[] args)
        {
            const int limit = 1000000;
            var primes = new HashSet<int>();
            for(mpz_t p = 2; p <= limit; p = p.NextPrimeGMP())
            {
                primes.Add((int)p);
            }
            var indexes = Enumerable.Range(0, primes.Count+1).ToArray();
            int max = 0;
            int bestprime = 0;
            for(int i = 0; i < primes.Count && 
                primes.Skip(i).Take(max).Sum() <= limit /* is it possible to generate a longer string? */
                ; i++)
            {
                for(int c = max; c <= primes.Count-i; c++)
                {
                    var sum = primes.Skip(i).Take(c).Sum();
                    if (sum >= limit) break;
                    if (c > max && primes.Contains(sum))
                    {
                        bestprime = sum;
                        max = c;
                    }
                }
            }
            Console.WriteLine($"\n{bestprime}, max = {max}");
        }
    }
}
