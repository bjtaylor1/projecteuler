using System;
using System.Collections.Concurrent;

namespace _0181
{
    class Program
    {
        public const long MAX_B = 40, MAX_W = 60; 
        static ConcurrentDictionary<(long, long, long, long), long> gcache = new ConcurrentDictionary<(long, long, long, long), long>();
        static long G(long nw, long kw, long nb, long kb) => gcache.GetOrAdd((nw, kw, nb, kb), G);

        static long G((long nw, long kw, long nb, long kb) param)
        {
            long result;
            long nw = param.nw;
            long nb = param.nb;
            long kw = param.kw;
            long kb = param.kb;
            if (nb < 0 || nw < 0) result = 0;
            else if (kb == 0 && kw == 0)
            {
                result = nw == 0 && nb == 0 ? 1 : 0;
            }
            else
            {
                result = 0;
                long gb = kb, gw = kw;
                while(gb >= 0 && gw >= 0)
                {
                    if (nw - gw >= 0 && nb - gb >= 0)
                    {
                        long @this = G(nw - gw, gw, nb - gb, gb);
                        result += @this;
                    }
                    if(--gb < 0)
                    {
                        gb = MAX_B;
                        gw--;
                    }
                }
            }
            return result;
        }

        static long P(long n)
        {
            long result = P(n, n);
            return result;
        }

        static void Main(string[] args)
        {
            Console.WriteLine(G(MAX_W, MAX_W, MAX_B, MAX_B));
        }
    }
}
