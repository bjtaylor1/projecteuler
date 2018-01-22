using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Mpir.NET;

namespace _521n
{
    class Program
    {
        static int smpf(mpz_t n)
        {
            for (mpz_t p = 2; ; p = p.NextPrimeGMP())
                if (n % p == 0) return (int) p;
        }

        static mpz_t S_bf(int n)
        {
            return Enumerable.Range(2, n - 1).ToArray().Sum(i => smpf(i));
        }
        static void Main(string[] args)
        {
            Console.WriteLine(S_bf(100));
        }
    }
}
