using System;
using Mpir.NET;

namespace _0216
{
    class Program
    {
        static void Main(string[] args)
        {
            int tot = 0;
            for(mpz_t i = 0; i <= 50000000; i++)
            {
                if (i.Mod(10000) == 0) Console.Write($"{(int)i:#,##0}   \r");
                var tn = 2 * i.Power(2) - 1;
                if (tn.IsProbablyPrimeRabinMiller(10))
                    tot++;
            }
            Console.WriteLine();
            Console.WriteLine(tot);
        }
    }
}
