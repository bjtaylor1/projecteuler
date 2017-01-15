namespace euler579cs2
{
    public class mnpq
    {
        public long m { get; }
        public long n { get; }
        public long p { get; }
        public long q { get; }

        public mnpq(long m, long n, long p, long q)
        {
            this.m = m;
            this.n = n;
            this.p = p;
            this.q = q;
        }

        public abcd get_abcd()
        {
            long a = 2 * m * p + 2 * n * q;
            long b = 2 * n * p - 2 * m * q;
            long c = (p * p) + (q * q) - (n * n + m * m);
            long d = (p * p) + (q * q) + (n * n + m * m);
            return new abcd(a, b, c, d);
        }

        public override string ToString()
        {
            return $"{m}{n},{p},{q}";
        }

    }
}