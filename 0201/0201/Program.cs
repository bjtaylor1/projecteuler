using System;
using System.Collections.Concurrent;
using System.Linq;
using Mpir.NET;

namespace _0201
{
    class Program
    {
        const int usage = 50;
        //static int[] nums
        static int[] set = Enumerable.Range(1, 100).Select(i => i * i).ToArray();
        //static int[] set = new[] { 1, 3, 6, 8, 10, 11 };
        //static int[] set = new[] { 1,2,3 };
        static ConcurrentDictionary<(int tot, int max, int used), mpz_t> cache = new ConcurrentDictionary<(int tot, int max, int left), mpz_t>();
        static mpz_t Ways(int tot, int max, int used) => cache.GetOrAdd((tot, max, used), Ways);
        static mpz_t Ways(int tot) => Ways(tot, set.Length - 1, 0);
        static mpz_t Ways((int tot, int max, int used) p)
        {
            if (p.tot < 0 || p.max < -1) return 0;
            if (p.tot == 0) return p.used == usage ? 1 : 0;

            mpz_t with = p.max >= 0 ? cache.GetOrAdd((p.tot - set[p.max], p.max - 1, p.used + 1), Ways) : 0;
            mpz_t without = cache.GetOrAdd((p.tot, p.max - 1, p.used), Ways);
            return with + without;
        }
        static void Main(string[] args)
        {
            var minSum = set.Take(usage).Sum();
            var maxSum = set.Reverse().Take(usage).Sum();
            mpz_t tot = 0;
            for(int sum = minSum; sum <= maxSum; sum++)
            {
                Console.WriteLine(sum);
                if (Ways(sum) == 1) tot += sum;
            }
            Console.WriteLine(tot);
        }
    }
}
