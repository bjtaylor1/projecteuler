using System;
using System.Linq;

namespace euler579cs2
{
    public static class Extensions
    {
        public static long gcd(long a, long b)
        {
            a = Math.Abs(a);
            b = Math.Abs(b);
            for (;;)
            {
                if (a == 0)
                    return b;
                b %= a;
                if (b == 0)
                    return a;
                a %= b;
            }
        }
        
        public static long gcd(this long[] longs)
        {
            if (longs.Length == 1) return longs.Single();
            else return gcd(longs.First(), gcd(longs.Skip(1).ToArray()));
        }
    }
}