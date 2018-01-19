using System;
using Mpir.NET;

namespace euler10
{
    class Program
    {
        static void Main(string[] args)
        {
            var tot = new mpz_t(0);
            for(mpz_t p = 2; p < 2e6; p = p.NextPrimeGMP())
            {
                tot += p;
            }
            Console.WriteLine(tot);
        }
    }
}
