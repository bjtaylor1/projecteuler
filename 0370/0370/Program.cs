using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using Mpir.NET;

namespace _0370
{
    class Program
    {
        //static mpz_t P = new mpz_t(2).Multiply(new mpz_t(10).Power(13));
        static mpz_t P = 5000;
        static mpz_t bmax = P.Divide(3);
        static mpz_t rbmax = P.Sqrt().Add(1).Divide(3);
        static mpz_t[] Primes;
    
        static void MakePrimes()
        {
            var primesList = new List<mpz_t>();
            for(mpz_t p = 2; p <= rbmax; p = p.NextPrimeGMP())
            {
                primesList.Add(p);
            }
            Primes = primesList.ToArray();
        }

        static ConcurrentDictionary<mpz_t, mpz_t[]> factorsCache = new ConcurrentDictionary<mpz_t, mpz_t[]>();
        static ConcurrentDictionary<mpz_t, FactorPair[]> factorPairsCache = new ConcurrentDictionary<mpz_t, FactorPair[]>();
        static mpz_t[] GetFactors(mpz_t n) => factorsCache.GetOrAdd(n, MakeFactors);
        static FactorPair[] GetFactorPairs(mpz_t n) => factorPairsCache.GetOrAdd(n, MakeFactorPairs);

        private static FactorPair[] MakeFactorPairs(mpz_t n)
        {
            var factorPairs = GetFactors(n).Select(p => new FactorPair(p, n.DivideExactly(p))).Distinct().ToArray();
            return factorPairs;
        }

        private static mpz_t[] MakeFactors(mpz_t n)
        {
            var allFactors = new List<mpz_t>();
            var factors = Primes.Where(p => p <= n.Sqrt()).SelectMany(p =>
             {
                 var pair = n.Divide(p, out mpz_t rem);
                 var factorsthis = new List<mpz_t>();
                 if (rem == 0)
                 {
                     factorsthis.Add(p);
                     factorsthis.Add(pair);
                     factorsthis.AddRange(GetFactors(pair));
                 }
                 return factorsthis.Distinct().ToArray();
             }).Distinct().ToArray();
            Array.Sort(factors);
            return factors;
        }

        static Program()
        {
            MakePrimes();
        }

        static void Main(string[] args)
        {
            mpz_t tot = P/3;
            for (mpz_t roota = 1; roota <= P.Sqrt(); roota++)
            {
                if(roota % 1000 == 0)
                {
                    Console.Write($"\r{roota} : {P.Sqrt()}");
                }
                for (mpz_t rootc = roota+1; ; rootc++)
                {
                    mpz_t a = roota.Power(2);
                    mpz_t c = rootc.Power(2);
                    mpz_t b = roota.Multiply(rootc);
                    if (a + b + c > P || c > a + b) break;
                    //if (mpz_t.Gcd(a, c) == 1)
                    {
                        Console.WriteLine($"{a},{b},{c}  {roota} / {rootc}{(mpz_t.Gcd(a,c) > 1 ? " M" : "")}");
                        tot += P.Divide(a + b + c);
                    }
                }
                Console.WriteLine();
            }
            Console.WriteLine(tot);
        }
    }

    public class FactorPair
    {
        public mpz_t Lower { get; }
        public mpz_t Higher { get; }
        public FactorPair(mpz_t p1, mpz_t p2)
        {
            if(p1 > p2)
            {
                Lower = p2;
                Higher = p1;
            }
            else
            {
                Lower = p1;
                Higher = p2;
            }
        }

        public override bool Equals(object obj)
        {
            return obj is FactorPair pair &&
                   EqualityComparer<mpz_t>.Default.Equals(Lower, pair.Lower) &&
                   EqualityComparer<mpz_t>.Default.Equals(Higher, pair.Higher);
        }

        public override int GetHashCode()
        {
            var hashCode = -1196965672;
            hashCode = hashCode * -1521134295 + EqualityComparer<mpz_t>.Default.GetHashCode(Lower);
            hashCode = hashCode * -1521134295 + EqualityComparer<mpz_t>.Default.GetHashCode(Higher);
            return hashCode;
        }

        public override string ToString()
        {
            return $"{Lower}, {Higher}";
        }
    }
}
