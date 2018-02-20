using System;
using System.Collections.Concurrent;
using Mpir.NET;

namespace _0201
{
    class Program
    {
        const int usage = 3;
        static int[] set = new[] { 1, 3, 6, 8, 10, 11 };
        //static int[] set = new[] { 1,2,3 };
        static ConcurrentDictionary<(int tot, int max, int used), mpz_t> cache = new ConcurrentDictionary<(int tot, int max, int left), mpz_t>();
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
            Console.WriteLine(cache.GetOrAdd((18, set.Length - 1, 0), Ways));
        }
    }
}
