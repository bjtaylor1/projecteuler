using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Mpir.NET;

namespace _618
{
    class Program
    {
        const int N = 24;
        private static int[] fib = new int[N+1];
        private static int[] primes;
        static Program()
        {
            fib[1] = 1;
            for(int i = 2; i <= N; i++ )
            {
                fib[i] = fib[i - 1] + fib[i - 2];
            }
            var primesList = new List<mpz_t>();
            for(mpz_t p = 2; p <= fib.Last(); p = p.NextPrimeGMP())
            {
                primesList.Add(p);
            }
            primes = primesList.Select(i => (int)i).ToArray(); //it should fit
        }

        private static ConcurrentDictionary<int, int[]> cache = new ConcurrentDictionary<int, int[]>();
        static int[] S(int n)
        {
            var sum = cache.GetOrAdd(n, CalcN);
            return sum;
        }

        private static int[] CalcN(int n)
        {
            if (n == 0) return new int[] { };
            IEnumerable<int> subprimes = primes.Where(p => p <= n).ToArray();
            Console.WriteLine($"Calculating {n}");
            var values = subprimes.SelectMany(p =>
            {
                if (p == n) return new int[] { p };
                else
                {
                    int[] sLower = S(n - p);
                    int[] res = sLower.Select(s =>
                    {
                        var r = s * p;
                        return r;
                    }).ToArray();
                    return res;
                }
            }).ToList();
            if (primes.Contains(n)) values.Add(n);

            var allValues = values.Where(v => v > 0).ToArray();
            var result = allValues.Distinct().ToArray();
            return result;
        }

        static void Main(string[] args)
        {
            int max = 0;
            var s8 = S(1000);
            Console.WriteLine("Calculated");
            Console.WriteLine(s8.Sum());
        }
    }

    public class SequenceEqualityComparer : IEqualityComparer<int[]>
    {
        public static SequenceEqualityComparer Instance => new SequenceEqualityComparer();
        private SequenceEqualityComparer()
        {
        }

        public bool Equals(int[] x, int[] y)
        {
            return x.SequenceEqual(y);
        }

        public int GetHashCode(int[] obj)
        {
            unchecked { return obj.Aggregate(397, (st, i) => (st * 397) ^ i.GetHashCode()); }
        }
    }
}
