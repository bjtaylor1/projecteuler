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
            long row = (crosses + 3) / 2;
            mpz_t prime = 2;
            var primes = new List<long> { (long)prime };
            while (primes.Count < row)
            {
                primes.Add((long)(prime = prime.NextPrimeGMP()));
            }
            Console.WriteLine("Made primes");
            long getXStart(long _row) => 3 - (_row % 3);
            long getHashCode(long _row, long _x)
            {
                unchecked
                {
                    long res = 0;
                    for (long lrow = 1; lrow < row; lrow++)
                    {
                        long dx = ((_x * lrow) / _row);
                        long seq = (getXStart(lrow) + dx) % 3;
                        res += seq * primes[(int)lrow];
                    }
                    return res;
                }
            }
            mpz_t row_t = row;
            long tot = 0;
            long div = 2 - (row % 2); //2 for even rows, 1 for odd rows.
            long x;
            var patterns = new HashSet<long>();
                        
            var xstart = getXStart(row);
            long xupperexc = (row + row % 2) / 2;
            for (x = xstart; x < xupperexc; x += 3)
            {
                Console.Write($"{x}\r");
                mpz_t gcd = mpz_t.Gcd(x /div, row_t);
                if (gcd == 1)
                {
                    var pattern = row / x;
                    if (patterns.Add(getHashCode(row, x)))
                    {
                        tot++;
                    }
                    else
                        Console.WriteLine("\nFound a duplicate!");
                }
            }
            Console.WriteLine();
            Console.WriteLine(tot * 2);
        }

        static string[][] rowPatterns = new string[][]
        {
            new string[]{"B", "C", "A"},
            new string[]{"C", "A", "B"}
        };
        //static string[][] rowPatterns = new string[][]
        //{
        //    new string[]{"AC", "AB", "BC"},
        //    new string[]{"AB", "BC", "AC"}
        //};
        //static Dictionary<string, string> fillers = new Dictionary<string, string>
        //{
        //    {"ABAC", "BC" },
        //    {"ACBC", "AB" },
        //    {"ABBC", "AC" }
        //};

        static string GetPattern(int x, int row)
        {
            var crosses = new List<string>();
            for(long lrow = 1; lrow < row; lrow++)
            {
                var segment = (((lrow * x / row) + (lrow % 2)) / 2) %3;
                var pattern = rowPatterns[lrow % 2][segment];
                //if (crosses.Any())
                //{
                //    var fillerKey = string.Join("", new[] { crosses.Last(), pattern }.OrderBy(s => s));
                //    var filler = fillers[fillerKey];
                //    crosses.Add(filler);
                //}
                crosses.Add(pattern);
            }
            var returnValue = string.Join(" ", crosses);
            return returnValue;
        }
    }
}
