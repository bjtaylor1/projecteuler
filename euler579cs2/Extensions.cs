using System.Linq;

namespace euler579cs2
{
    public static class Extensions
    {
        public static long gcd(long a, long b)
        {
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
            var aggregate = longs.Aggregate(0L, gcd);
            return aggregate;
        }
    }
}