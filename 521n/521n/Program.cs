using System;
using System.Collections.Generic;
using System.Linq;
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

        static mpz_t S(mpz_t n)
        {
            mpz_t result = new mpz_t(0);
            for (mpz_t p = 2; p <= n; p = p.NextPrimeGMP())
            {
                
            }
            return result;
        }

        static mpz_t S_bf(int n)
        {
            var dict = new Dictionary<int,int>();
            for (int i = 2; i <= n; i++)
            {
                dict[i] = smpf(i);
            }
            var res = dict.Values.Sum();
            var groups = dict.GroupBy(k => k.Value);
            return res;
        }
        static void Main(string[] args)
        {
            Console.WriteLine(S_bf(100));
        }
    }
}
