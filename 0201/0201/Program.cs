using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0201
{
    class Program
    {
#if DEBUG
        static int[] set = { 1, 3, 6, 8, 10, 11 };
#else
        static int[] set = Enumerable.Range(1, 100).Select(i => i * i).ToArray();
#endif
        static int use = set.Length / 2;
        static readonly ConcurrentDictionary<(int tot, int last, int use), long> cache = new ConcurrentDictionary<(int tot, int last, int use), long>();

        static long W(int tot) => W(tot, set.Length - 1, use);
        static long W(int tot, int last, int use) => cache.GetOrAdd((tot, last, use), W);
        static long W((int tot, int last, int use) p)
        {
            long result;

            if (p.tot < 0 || p.last < 0 || p.use < 0) result = 0;
            else if (p.use == 0) result = p.tot == 0 ? 1 : 0;
            else if (p.last == 0) result = set[p.last] == p.tot && p.use == 1 ? 1 : 0;
            else
            {
                long wo = W(p.tot, p.last - 1, p.use);
                long wi = W(p.tot - set[p.last], p.last - 1, p.use - 1);
                result = wo + wi;
            }
            return result;
        }
        static void Main(string[] args)
        {
            long tot = 0;
            var minSum = set.Take(use).Sum();
            var maxSum = set.Reverse().Take(use).Sum();
            //Console.WriteLine(W(17));
            for (int sum = minSum; sum <= maxSum; sum++)
            {
                long wsum = W(sum);
                Console.WriteLine($"W({sum}) = {wsum}");
                if (wsum == 1) tot += sum;
            }
            Console.WriteLine(tot);
        }
    }
}
