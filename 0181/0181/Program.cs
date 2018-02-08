using System;
using System.Collections.Concurrent;

namespace _0181
{
    class Program
    {
        static ConcurrentDictionary<(long, long, long, long), long> gcache = new ConcurrentDictionary<(long, long, long, long), long>();
        static long G(long nw, long kw, long nb, long kb) => gcache.GetOrAdd((nw, kw, nb, kb), G);
        static ConcurrentDictionary<(long, long), long> pcache = new ConcurrentDictionary<(long n, long firstGroup), long>();
        static long P(long n, long firstGroup) => pcache.GetOrAdd((n, firstGroup), P);

        static long G((long nw, long kw, long nb, long kb) param)
        {
            long result;
            long nw = param.nw;
            long nb = param.nb;
            long kw = param.kw;
            long kb = param.kb;
            if (kb <= 0 || kw <= 0 || nb < 0 || nw < 0) return 0;

            if(nb == 0)
            {
                result = 1;
            }
            else if(nw == 0)
            {
                result = 1;
            }
            else
            {
                result = P(nb + nw, nb + nw) ;
                for(long gw = 1; gw <= nw; gw++)
                {
                    for(long gb = 1; gb <= nb; gb++)
                    {
                        if (gb + gw < nb + nw)
                        {
                            result += G(nw - gw, kw, nb - gb, kb);
                        }
                    }
                }

            }
            return result;
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
            long result = P(n, n);
            return result;
        }

        static void Main(string[] args)
        {
            Console.WriteLine(G(1,1,3,3));
        }
    }
}
