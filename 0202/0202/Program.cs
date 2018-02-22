using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using Mpir.NET;

namespace _0202
{
    class Program
    {
        static void Main(string[] args)
        {
            long crosses = 12017639147; // 1000001;// 27; // 11;;
            mpz_t row = (crosses + 3) / 2;
            long tot = 0;
            var patterns = new HashSet<long>();
                        
            var xstart = (long)(3 - (row % 3));
            long xupperexc = (long)((row + row % 2) / 2);
            var sw = Stopwatch.StartNew();
            Parallel.For(xstart, xupperexc, x =>
            {
                //if (x / 10000 % 1000 == 0) Console.Write($"{x:0,000}\r");
                mpz_t gcd = mpz_t.Gcd(x, row);
                if (gcd == 1)
                {
                    tot++;
                }
            });
            sw.Stop();

            Console.WriteLine($"\n{tot * 2}");
            Console.WriteLine(sw.Elapsed);
        }
    }
}
