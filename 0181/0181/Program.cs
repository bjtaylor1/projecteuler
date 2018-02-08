using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0181
{
    class Program
    {
        static ConcurrentDictionary<(long n, long firstGroup), long> cache = new ConcurrentDictionary<(long n, long firstGroup), long>();
        static long P(long n, long firstGroup)
        {
            return cache.GetOrAdd((n, firstGroup), P);
        }

        static long P((long n, long firstGroup) param)
        {
            if(param.firstGroup <= 0 || param.n < 0)
            {
                return 0;
            }

            if (param.n <= 1) return 1;
            long with = P((param.n - param.firstGroup, param.firstGroup));
            long without = P((param.n, param.firstGroup - 1));
            long result = with + without;
            return result;
        }

        static long P(long n)
        {
            long result = P((n, n));
            return result;
        }

        static void Main(string[] args)
        {
            Console.WriteLine(P(5));
        }
    }
}
