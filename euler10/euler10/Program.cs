using System;
using System.Diagnostics;
using System.Linq;
using Mpir.NET;

namespace euler10
{
    class Program
    {
        static mpz_t P(mpz_t n)
        {
            var r = (int)n.Sqrt();
            if (r > int.MaxValue) throw new ArgumentException($"n too large");
            var V = Enumerable.Range(1, r).Select(i => (n/i)).ToList();
            V.AddRange(Enumerable.Range(1, (int)(V.Last()-1)).Select(i => new mpz_t(i)).Reverse());
            var S = V.ToDictionary(i => new mpz_t(i), i => new mpz_t(i) * (i + 1) / 2 - 1);
            for (int p = 2; p <= r; p++)
            {
                if(S[p] > S[p-1])
                {
                    var sp = S[p - 1];
                    var p2 = p * p;
                    foreach(var v in V)
                    {
                        if (v < p2) break;
                        S[v] -= (S[v / p] - sp) * p;
                    }
                }
            }
            return S[n];
        }

        static void test(mpz_t n)
        {
            var sw = Stopwatch.StartNew();
            var result = P(n);
            sw.Stop();
            Console.WriteLine($"P({n}) = {result}, took {sw.Elapsed}");
        }
        static void Main(string[] args)
        {
            test(2*new mpz_t(10).Power(6)); //puzzle answer
            test(20);
            test(new mpz_t(10).Power(3));
            test(new mpz_t(10).Power(6));
            test(new mpz_t(10).Power(9));
            test(new mpz_t(10).Power(12));

        }
    }
}
