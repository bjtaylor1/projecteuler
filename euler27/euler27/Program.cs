using System;
using Mpir.NET;

namespace euler27
{
    class Program
    {
        static int NumPrimes(int a, int b)
        {
            for(int n = 0; ; n++)
            {
                mpz_t bign = new mpz_t(n);
                mpz_t p = bign.Power(2) + bign.Multiply(a) + b;
                if (!p.IsProbablyPrimeRabinMiller(10))
                    return n;
            }
        }

        static void Main(string[] args)
        {
            var abmax = (a: 0,b: 0);
            var npmax = 0;
            for(int a = -999; a <= 999; a++)
            {
                for(int b = -999; b <= 999; b++)
                {
                    var np = NumPrimes(a, b);
                    if (np > npmax)
                    {
                        abmax = (a, b);
                        npmax = np;
                    }
                }
            }
            Console.WriteLine($"{abmax.a} x {abmax.b} = {abmax.a * abmax.b}");
        }
    }
}
