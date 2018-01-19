using System;
using Mpir.NET;

namespace euler35
{
    class Program
    {
        static bool IsCircular(mpz_t p)
        {
            string ps = p.ToString();
            for (int i = 1; i < ps.Length; i++)
            {
                string c = $"{ps.Substring(i)}{ps.Substring(0, i)}";
                var pr = new mpz_t(c);
                if (!pr.IsProbablyPrimeRabinMiller(10))
                    return false;
            }
            return true;
        }

        static void Main(string[] args)
        {
            int count = 0;
            for(mpz_t p = 2; p < 1e6; p =p.NextPrimeGMP())
            {
                if (IsCircular(p)) count++;
            }
            Console.WriteLine(count);
        }
    }
}
