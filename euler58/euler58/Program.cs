using System;
using System.Linq;
using Mpir.NET;

namespace euler58
{
    class Program
    {
        static void Main(string[] args)
        {
            mpz_t[] diags = { 1 };
            double totPrimes = 0, totDiags = 1;
            double ratio = 1;
            int sideLengthExc = 0; // == sideLength - 1
            do
            {
                sideLengthExc += 2;
                diags = Enumerable.Range(1, 4).Select(i => new mpz_t(diags.Last() + (sideLengthExc * i))).ToArray();
                totPrimes += diags.Count(n => n.IsProbablyPrimeRabinMiller(100));
                totDiags += 4;
                ratio = totPrimes / totDiags;
            } while (totPrimes / totDiags >= 0.1);
            Console.WriteLine(sideLengthExc + 1);
        }
    }
}
