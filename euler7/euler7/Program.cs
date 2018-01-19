using System;
using Mpir.NET;

namespace euler7
{
    class Program
    {
        static void Main(string[] args)
        {
            var p = new mpz_t(2);
            for(int i = 1; i < 10001; i++)
            {
                p = p.NextPrimeGMP();
            }
            Console.WriteLine(p);
        }
    }
}
