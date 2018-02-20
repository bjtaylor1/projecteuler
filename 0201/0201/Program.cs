using System;
using System.Collections.Concurrent;
using System.Linq;
using Mpir.NET;

namespace _0201
{
    class Program
    {
        static int NormalizeMax(int max)
        {
            if (max < 0) return -1;
            else return max;
        }

        //const int usage = 50;
        const int usage = 3;
        //static int[] set = Enumerable.Range(1, 100).ToArray();
        //static int[] set = Enumerable.Range(1, 100).Select(i => i * i).ToArray();
        static int[] set = new[] { 1, 3, 6, 8, 10, 11 };
        static ConcurrentDictionary<(int tot, int max, int left), mpz_t> cache = new ConcurrentDictionary<(int tot, int max, int left), mpz_t>();
        static mpz_t Ways(int tot, int max, int left) => cache.GetOrAdd((tot, NormalizeMax(max), left), Ways);
        static mpz_t Ways(int tot) => Ways(tot, set.Length - 1, usage);
        static mpz_t Ways((int tot, int max, int left) p)
        {
            if(cache.Count > 100000)
            {
                Console.WriteLine();
            }

            if (p.left > usage) return 0;
            if (p.tot < 0 || p.max < -1) return 0;
            if (p.tot == 0) return p.left == 0 ? 1 : 0;

            mpz_t with = p.max >= 0 ? cache.GetOrAdd((p.tot - set[p.max], p.max - 1, p.left - 1), Ways) : 0;
            mpz_t without = cache.GetOrAdd((p.tot, p.max - 1, p.left), Ways);
            return with + without;
        }
        static void Main(string[] args)
        {

            var minSum = set.Take(usage).Sum();
            Console.WriteLine($"W({minSum}) = {Ways(minSum)}");

            var maxSum = set.Reverse().Take(usage).Sum();
            mpz_t tot = 0;
            for (int sum = minSum; sum <= maxSum; sum++)
            {
                Console.WriteLine(sum);
                if (Ways(sum) == 1) tot += sum;
            }
            Console.WriteLine(tot);
        }
    }
}
