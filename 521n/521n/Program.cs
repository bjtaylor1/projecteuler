using System;
using System.Collections.Generic;
using System.Linq;
using Mpir.NET;

namespace _521n
{
    class Program
    {
        static int smpf(mpz_t n)
        {
            for (mpz_t p = 2; ; p = p.NextPrimeGMP())
                if (n % p == 0) return (int) p;
        }

        static mpz_t S(mpz_t n)
        {
            var dict = new Dictionary<int, int>();
            for (int i = 2; i <= n; i++)
            {
                dict[i] = smpf(i);
            }
            var res = dict.Values.Sum();
            var groups = dict.GroupBy(k => k.Value).ToArray();
            var groupq = groups.Select(k => k.Select(m => m.Key / k.Key)).ToArray();
            var res2 = groups.Sum(g => g.Sum(k => k.Value));

            var primes = new List<mpz_t>();
            for (mpz_t p = 2; p < n; p = p.NextPrimeGMP())
                primes.Add(p);
            mpz_t resf = 0;
            foreach(var p in primes)
            {
             //   var t = p
            }
            mpz_t s = primes.Aggregate((st, p) => st + p * primes.Count(op => op >= p));
            return s;
        }

        static mpz_t S_bf(int n)
        {
            var dict = new Dictionary<int,int>();
            for (int i = 2; i <= n; i++)
            {
                dict[i] = smpf(i);
            }
            var res = dict.Values.Sum();
            var groups = dict.GroupBy(k => k.Value).ToArray();
            var groupq = groups.Select(k => k.Select(m => m.Key / k.Key)).ToArray();
            var res2 = groups.Sum(g => g.Sum(k => k.Value));
            return res;
        }
        static void Main(string[] args)
        {
            Console.WriteLine(S(10));
        }
    }
}
