using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _618
{
    class Program
    {
        private static int[] primes = new[] { 2, 3, 5, 7 };
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

            int[] values = subprimes.SelectMany(p =>
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
            }).ToArray();
            
            var result = values.Where(v => v > 0).Distinct().ToArray();
            return result;
        }

        static void Main(string[] args)
        {
            int max = 0;
            var s8 = S(8);

            Console.WriteLine(s8);
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
