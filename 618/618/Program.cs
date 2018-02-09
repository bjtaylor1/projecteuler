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
        const int N = 6;
        private static int[] fib = new int[N+1];
        private static int[] primes;
        private static mpz_t[] products;
        static Program()
        {
            fib[1] = 1;
            for (int i = 2; i <= N; i++)
            {
                fib[i] = fib[i - 1] + fib[i - 2];
            }
            var primesList = new List<mpz_t>();
            for (mpz_t p = 2; p <= fib.Last(); p = p.NextPrimeGMP())
            {
                primesList.Add(p);
            }
            primes = primesList.Select(i => (int)i).ToArray(); //it should fit
            products = Enumerable.Repeat(0, fib.Last() + 1).Select(i => new mpz_t(i)).ToArray();
        }

        static int sum = 0;
        static mpz_t product = 1;
        static Stack<int> pindexes = new Stack<int> (new[] { 0 });
        static int depth = 0;
        static int r = 0;
        static int[] V = new[] { 1, 3, 5 };

        static int[] Min = Enumerable.Repeat(int.MaxValue, 12).ToArray();

        static void FillIn()
        {
        }

        static void Find(List<int> all, List<int> comb, HashSet<int[]> combs)
        {
            if (comb.Sum() == 10)
            {
                combs.Add(comb.ToArray());
            }
            else if (comb.Sum() < 10)
            {
                foreach (var o in all.Where(i => i > comb.LastOrDefault()).ToArray())
                {
                    all.Remove(o);
                    comb.Add(o);
                    Find(all, comb, combs);
                    comb.Remove(o);
                    all.Add(o);
                }
            }
        }

        static void Main(string[] args)
        {
            const int S = 10;
            int[] w = new int[S + 1];
            w[0] = 1;
            for (int i = 1; i <= S; i++)
            {
                for (int j = S; j >= i; j--)
                {
                    w[j]+=w[j-i];
                }
            }

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
