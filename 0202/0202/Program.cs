using System;
using System.Collections.Generic;
using Mpir.NET;

namespace _0202
{
    class Program
    {
        static void Main(string[] args)
        {
            long crosses = 1000001;// 27; // 11; //  12017639147;
            mpz_t row = (crosses + 3) / 2;
            long tot = 0;
            long x;
            var patterns = new HashSet<long>();
                        
            var xstart = (long)(3 - (row % 3));
            long xupperexc = (long)((row + row % 2) / 2);
            for (x = xstart; x < xupperexc; x += 3)
            {
                mpz_t gcd = mpz_t.Gcd(x, row);
                if (gcd == 1)
                {
                    tot++;
                }
            }
            Console.WriteLine(tot * 2);
        }
    }
}
