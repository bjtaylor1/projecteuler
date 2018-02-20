using System;
using System.Linq;
using System.Runtime.Caching;
using System.Threading;
using System.Threading.Tasks;
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

        static CacheItemPolicy cap = new CacheItemPolicy();

        const int usage = 50;
        //const int usage = 3;
        static int[] set = Enumerable.Range(1, 100).Select(i => i * i).ToArray();
        //static int[] set = new[] { 1, 3, 6, 8, 10, 11 };
        static mpz_t Ways(int tot) => Ways(tot, set.Length - 1, usage);
        static mpz_t Ways(int tot, int max, int left)
        {
            var cacheKey = $"{tot},{max},{left}";
            var existing = MemoryCache.Default.Get(cacheKey);
            if (existing != null) return (mpz_t)existing;

            if (set.Take(max + 1).Reverse().Take(left).Sum() < tot) return 0;
            if (left > usage) return 0;
            if (tot < 0 || max < -1) return 0;
            if (tot == 0) return left == 0 ? 1 : 0;

            mpz_t with = max >= 0 ? Ways(tot - set[max], max - 1, left - 1) : 0;
            mpz_t without = Ways(tot, max - 1, left);
            MemoryCache.Default.Set(cacheKey, with + without, cap);
            return with + without;
        }
        static void Main(string[] args)
        {
            var minSum = set.Take(usage).Sum();
            var maxSum = set.Reverse().Take(usage).Sum();
            Console.WriteLine($"Sum diff = {(maxSum - minSum)}");
            Console.WriteLine($"W({minSum}) = {Ways(minSum)}");
            Console.WriteLine($"W({maxSum}) = {Ways(maxSum)}");
            long tot = 0;
            Parallel.For(minSum, maxSum + 1, sum =>
            {
                if (sum % 100 == 0) Console.Write($"{((double)(sum - minSum) / (maxSum - minSum)):P1} {sum} {MemoryCache.Default.GetCount()}\r");
                if (Ways(sum) == 1) Interlocked.Add(ref tot, sum);
            });
            Console.WriteLine(tot);
        }
    }
}
